//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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
#include <lib/segment/builder/stack.h>
#include <lib/segment/builder.h>


segment_builder_stack::~segment_builder_stack()
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    delete [] list;
    list = 0;
    DEBUG(1, "}");
}


segment_builder_stack::segment_builder_stack() :
    length(0),
    maximum(0),
    list(0)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


rcstring
segment_builder_stack::representation(void)
    const
{
    rcstring result = "[";
    for (size_t j = 0; j < length; ++j)
    {
        if (j)
            result += ", ";
        result +=
            rcstring::printf
            (
                "%s %d",
                list[j]->get_segment_name().c_str(),
                list[j]->get_segment_number()
            );
    }
    result += "]";
    return result;
}


void
segment_builder_stack::push_back(segment_builder::pointer sbp)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", representation().c_str());
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 8;
        segment_builder::pointer *new_list =
            new segment_builder::pointer [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_list[j] = list[j];
        delete [] list;
        list = new_list;
        maximum = new_maximum;
    }
    list[length++] = sbp;
    DEBUG(2, "%s", representation().c_str());
    DEBUG(1, "}");
}


bool
segment_builder_stack::emit_byte(unsigned char value)
{
    assert(length != 0);
    return list[length - 1]->emit_byte(value);
}


bool
segment_builder_stack::emit_word_alignment(unsigned char value)
{
    return list[length - 1]->emit_word_alignment(value);
}


bool
segment_builder_stack::emit_odd_alignment(unsigned char value)
{
    return list[length - 1]->emit_odd_alignment(value);
}


bool
segment_builder_stack::emit_word(unsigned value)
{
    assert(length != 0);
    return list[length - 1]->emit_word(value);
}


int
segment_builder_stack::next_procedure_number()
{
    assert(length != 0);
    return list[length - 1]->next_procedure_number();
}


void
segment_builder_stack::procedure_parameter_size(unsigned nbytes)
{
    assert(length != 0);
    list[length - 1]->procedure_parameter_size(nbytes);
}


void
segment_builder_stack::procedure_variable_size(unsigned nbytes)
{
    assert(length != 0);
    list[length - 1]->procedure_variable_size(nbytes);
}


void
segment_builder_stack::procedure_enter_ic()
{
    assert(length != 0);
    list[length - 1]->procedure_enter_ic();
}


void
segment_builder_stack::procedure_exit_ic()
{
    assert(length != 0);
    list[length - 1]->procedure_exit_ic();
}


void
segment_builder_stack::procedure_end(int proc_num, int lex_level,
    const code_source_map::pointer &listing, mtype_t mtype)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "proc_num = %d", proc_num);
    DEBUG(2, "lex_level = %d", lex_level);
    DEBUG(2, "%s", representation().c_str());
    assert(length != 0);
    list[length - 1]->procedure_end(proc_num, lex_level, listing, mtype);
    DEBUG(2, "stack depth = %ld", (long)length);
    // Note: segment_end is not to be called from here.
    DEBUG(1, "}");
}


void
segment_builder_stack::segment_end()
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", representation().c_str());
    assert(length > 0);
    list[length - 1]->complete();
    pop_back();
    DEBUG(2, "stack depth = %ld", (long)length);
    DEBUG(2, "%s", representation().c_str());
    DEBUG(1, "}");
}


unsigned
segment_builder_stack::get_address()
    const
{
    assert(length != 0);
    return list[length - 1]->get_address();
}


void
segment_builder_stack::poke(unsigned address, unsigned char value)
{
    assert(length != 0);
    list[length - 1]->poke(address, value);
}


void
segment_builder_stack::poke_word(unsigned address, unsigned value)
{
    assert(length != 0);
    list[length - 1]->poke_word(address, value);
}


int
segment_builder_stack::new_jump_table_entry(unsigned address)
{
    assert(length != 0);
    return list[length - 1]->new_jump_table_entry(address);
}


unsigned
segment_builder_stack::get_segment_number()
    const
{
    if (length == 0)
    {
        //
        // This happens when the compiler is adding the segment 0
        // procedures (as builtins) to the symbol table.  There is no
        // segment being built.
        //
        return 0;
    }
    return list[length - 1]->get_segment_number();
}


segment_builder::pointer
segment_builder_stack::back(void)
    const
{
    assert(length > 0);
    return list[length - 1];
}


void
segment_builder_stack::pop_back(void)
{
    assert(length > 0);
    --length;
    list[length].reset();
}


void
segment_builder_stack::unit_interface_text(const rcstring &text)
{
    assert(length != 0);
    list[length - 1]->unit_interface_text(text);
}


void
segment_builder_stack::segment_not_linked(const link_info::pointer &lip)
{
    assert(length > 0);
    list[length - 1]->segment_not_linked(lip);
}


void
segment_builder_stack::link_globref_big(const rcstring &name)
{
    assert(length > 0);
    list[length - 1]->link_globref_big(name);
}


void
segment_builder_stack::link_publref_big(const rcstring &name)
{
    assert(length > 0);
    list[length - 1]->link_publref_big(name);
}


void
segment_builder_stack::link_privref_big(const rcstring &name, int nwords)
{
    assert(length > 0);
    list[length - 1]->link_privref_big(name, nwords);
}


void
segment_builder_stack::link_publdef(const rcstring &name, int base_offset)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(base_offset >= 0);
    assert(base_offset < 32768);
    assert(length > 0);
    // skip interface definitions
    int n = length - 1;
    while (n > 0 && list[n]->is_an_interface())
        --n;
    list[n]->link_publdef(name, base_offset);
}


// vim: set ts=8 sw=4 et :
