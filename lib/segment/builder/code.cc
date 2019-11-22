//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at
// you option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#include <lib/ac/assert.h>

#include <lib/debug.h>
#include <lib/opformat.h>
#include <lib/segment/builder/code.h>
#include <lib/segment.h>


segment_builder_code::~segment_builder_code()
{
    delete [] data;
}


segment_builder_code::segment_builder_code
(
    const segment::pointer &a_sp,
    codefile *a_cfp
) :
    sp(a_sp),
    data(new unsigned char [1 << 16]),
    address(0),
    high_water(1u << 16),
    maximum_procedure_number(0),
    param_size(0),
    data_size(0),
    enter_ic(0),
    enter_ic_set(false),
    exit_ic(0),
    jump_table_size(0),
    cfp(a_cfp)
{
    // codeleng is not allowed to be exactly 1<<16
    high_water -= 2;

    // we have to allow 2 bytes for segnum and numprocs
    high_water -= 2;

    for (unsigned j = 0; j < 256; ++j)
        procedure_dictionary[j] = 0;

    assert(cfp);
}


segment_builder_code::pointer
segment_builder_code::create(const segment::pointer &a_sp, codefile *a_cfp)
{
    return pointer(new segment_builder_code(a_sp, a_cfp));
}


bool
segment_builder_code::emit_byte(unsigned char c)
{
    if (address >= high_water)
        return false;
    data[address++] = c;
    return true;
}


bool
segment_builder_code::emit_word(unsigned n)
{
    assert(int(n) < int(1L << 16));
    assert(int(n) >= -int(1L << 15));
    unsigned char buf[2];
    sp->put_word(buf, n);
    return (emit_byte(buf[0]) && emit_byte(buf[1]));
}


bool
segment_builder_code::emit_big(unsigned n)
{
    assert(n < (1uL << 15));
    if (n < 128)
        return emit_byte(n);
    return (emit_byte((n >> 8) | 0x80) && emit_byte(n));
}


bool
segment_builder_code::emit_word_alignment(unsigned char value)
{
    if ((address & 1) == 0)
        return true;
    return emit_byte(value);
}


bool
segment_builder_code::emit_odd_alignment(unsigned char value)
{
    if (address & 1)
        return true;
    return emit_byte(value);
}


int
segment_builder_code::next_procedure_number(void)
{
    //
    // There maximum number of procedures is 255, limited by the 1-byte
    // field used to hold the size of the procedure table in the
    // segment.
    //
    if (maximum_procedure_number >= 255)
        return -1;

    //
    // Adjust the high water level downwards, to allow for 2 bytes for
    // the procedure dictionary entry, and 10 bytes for the procedure
    // attributes.
    //
    high_water -= 12;

    ++maximum_procedure_number;
    return maximum_procedure_number;
}


void
segment_builder_code::procedure_parameter_size(unsigned nbytes)
{
    assert((nbytes & 1) == 0);
    param_size = nbytes;
}


void
segment_builder_code::procedure_variable_size(unsigned nbytes)
{
    assert((nbytes & 1) == 0);
    data_size = nbytes;
}


void
segment_builder_code::procedure_enter_ic(void)
{
    assert((address & 1) == 0);
    enter_ic = address;
    enter_ic_set = true;
}


void
segment_builder_code::procedure_exit_ic(void)
{
    exit_ic = address;
}


void
segment_builder_code::put_reloc(reloc_t &addrs)
{
    for (reloc_t::const_iterator it = addrs.begin(); it != addrs.end(); ++it)
    {
        // These are all self-relative pointers.
        emit_word(address - *it);
    }
    emit_word(addrs.size());
    addrs.clear();
}


void
segment_builder_code::procedure_end(int proc_num, int lex_level,
    const code_source_map::pointer &listing, mtype_t mtype)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "proc_num = %d", proc_num);
    DEBUG(1, "lex_level = %d", lex_level);
    DEBUG(1, "segname = %s", sp->get_name().quote_c().c_str());
    if (proc_num < 0)
    {
        // previously reported error
        assert(maximum_procedure_number == 255);
        DEBUG(1, "}");
        return;
    }
    assert(proc_num <= maximum_procedure_number);
    assert(enter_ic_set);

    bool machine_code = true;
    switch (mtype)
    {
    case mtype_undefined:
    case mtype_pcode_be:
    case mtype_pcode_le:
        machine_code = false;
        break;

    case mtype_pdp11:
    case mtype_8080:
    case mtype_z80:
    case mtype_ga440:
    case mtype_6502:
    case mtype_6800:
    case mtype_ti9900:
    default:
        break;
    }

    // It is tempting to say
    //     assert(exit_ic != 0);
    // but if a procedure body is empty, it is possible for the enter_ic
    // and the exit_ic to be the same.

    assert(enter_ic <= exit_ic);

    // We need to be on an even byte boundary.
    if (address & 1)
        data[address++] = 0;

    //
    // Emit the jump table.
    //
    if (machine_code)
    {
        assert(jump_table_size == 0);
        // Emit relocation information.
        for (size_t j = 0; j < 4; ++j)
            put_reloc(reloc[j]);
    }
    else
    {
        while (jump_table_size > 0)
        {
            --jump_table_size;
            unsigned dest = jump_table[jump_table_size];
            emit_word(address - dest);
        }
    }

    //
    // Now emit the procedure attributes.
    //
    if (!machine_code)
    {
        emit_word(data_size);
        data_size = 0;

        emit_word(param_size);
        param_size = 0;

        emit_word(address - exit_ic);
        exit_ic = 0;
    }

    emit_word(address - enter_ic);

    assert(proc_num > 0);
    assert(proc_num < 256);
    assert(procedure_dictionary[proc_num] == 0);
    procedure_dictionary[proc_num] = address;

    emit_byte(machine_code ? 0 : proc_num);
    emit_byte(lex_level);

    if (listing)
    {
        //
        // Disassemble the procedure, and interleave the relevant source
        // code lines.  Very useful for seeing what the compiler is
        // doing, especially when debugging.
        //
        listing->disassemble
        (
            mtype,
            data,
            enter_ic,
            address,
            sp->get_segment_number(),
            proc_num,
            cfp
        );

        //
        // You can do this after each procedure,
        // or not at all.
        //
        listing->forget_associations();
    }
    enter_ic = 0;
    enter_ic_set = false;
    DEBUG(1, "}");
}


void
segment_builder_code::unit_interface_text(const rcstring &text)
{
    if (text.empty())
        return;
    sp->get_text_address(text);
}


void
segment_builder_code::complete(void)
{
    high_water = 1u << 16;

    //
    // Generate the procedure dictionary into the segment data.
    //
    assert((address & 1) == 0);
    assert(maximum_procedure_number != 0);
    for (unsigned j = maximum_procedure_number; j > 0; --j)
    {
        unsigned n = procedure_dictionary[j];
        emit_word(n ? address - n : 0);
    }
    emit_byte(sp->get_segment_number());
    assert(maximum_procedure_number < 256);
    emit_byte(maximum_procedure_number);

    // In theory, there was always enough room left, because the
    // emit_byte calls took note if high_water.
    assert(address < (1uL << 16));

    //
    // allocate code address and code length, and space in the codefile
    sp->get_code_address(address);

    //
    // Pad the data to a whole multiple of 512 byte,
    // because it looks nicer.
    //
    while (address & 511)
        emit_byte(0);

    //
    // Now we can write the data to the codefile.
    // (relative to codeaddr, no need to mention it here).
    //
    sp->write_block(0, data, address >> 9);

    // emit the linker relocation data
    int nextlc = get_nextlc();
    assert(nextlc > 0);
    sp->write_link_information(nextlc);
}


int
segment_builder_code::get_nextlc(void)
    const
{
    //
    // The nextlc value, stashed in the EOFMARK link into record,
    // is where to start allocating space in the global variables,
    // if the assembler or unit code wants to.
    //
    int jtab = procedure_dictionary[1];
    if (!jtab)
    {
        // no proceure one (?!?)
        return 1;
    }
    if (data[jtab] != 1)
    {
        // assembler
        return 1;
    }
    int dsize = sp->get_word(data + jtab - 8);
    int psize = sp->get_word(data + jtab - 6);
    int nextlc = (dsize + psize) / 2 + 1;
    assert(nextlc > 0);
    assert(nextlc < 32768);
    return nextlc;
}


unsigned
segment_builder_code::get_address(void)
    const
{
    return address;
}


void
segment_builder_code::poke(unsigned addr, unsigned char value)
{
    assert(addr < address);
    data[addr] = value;
}


void
segment_builder_code::poke_word(unsigned addr, unsigned value)
{
    assert(addr + 2 <= address);
    unsigned char buf[2];
    sp->put_word(buf, value);
    data[addr] = buf[0];
    data[addr + 1] = buf[1];
}


int
segment_builder_code::new_jump_table_entry(unsigned addr)
{
    assert(enter_ic_set);
    if (addr == enter_ic)
        return -2;
    for (size_t j = 0; j < jump_table_size; ++j)
        if (jump_table[j] == addr)
            return (-10 - 2 * j);
    // FIXME: check this, somehow
    assert(jump_table_size * 2 + 8 < 128);
    jump_table[jump_table_size] = addr;
    jump_table_size++;
    high_water -= 2;
    return (-8 - 2 * jump_table_size);
}


unsigned
segment_builder_code::get_segment_number(void)
    const
{
    return sp->get_segment_number();
}


rcstring
segment_builder_code::get_segment_name(void)
    const
{
    return sp->get_name();
}


void
segment_builder_code::segment_not_linked(const link_info::pointer &lip)
{
    sp->segment_not_linked(lip);
}


void
segment_builder_code::link_globref_big(const rcstring &name)
{
    link_info::pointer lip =
        link_info::create(name, GLOBREF, (int)opformat_big);
    lip->add_reference(address);
    sp->add_link_information(lip);
}


void
segment_builder_code::link_publref_big(const rcstring &name)
{
    link_info::pointer lip =
        link_info::create(name, PUBLREF, (int)opformat_big);
    lip->add_reference(address);
    sp->add_link_information(lip);
}


void
segment_builder_code::link_publdef(const rcstring &name, int base_offset)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(base_offset >= 0);
    assert(base_offset < 32768);
    link_info::pointer lip = link_info::create(name, PUBLDEF, base_offset + 1);
    sp->add_link_information(lip);
}


void
segment_builder_code::link_privref_big(const rcstring &name, int nwords)
{
    assert(nwords > 0);
    assert(nwords < (1 << 15));
    link_info::pointer lip =
        link_info::create(name, PRIVREF, (int)opformat_big, 0, nwords);
    lip->add_reference(address);
    sp->add_link_information(lip);
}


void
segment_builder_code::reloc_type1_lcrel(unsigned pc)
{
    reloc[1].push_back(pc);
}


void
segment_builder_code::reloc_type2_ref(unsigned pc)
{
    reloc[2].push_back(pc);
}


void
segment_builder_code::reloc_type3_pubpriv(unsigned pc)
{
    reloc[3].push_back(pc);
}


bool
segment_builder_code::is_an_interface(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
