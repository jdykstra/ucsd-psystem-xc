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

#include <lib/ac/stdio.h>

#include <lib/codefile/file.h>
#include <lib/debug.h>
#include <lib/expression/dispatcher/binary/functor/method.h>
#include <lib/expression/error.h>
#include <lib/segment/builder/code.h>
#include <lib/pascal_id.h>
#include <lib/type/boolean.h>
#include <lib/type/integer.h>
#include <lib/type/boolean.h>
#include <lib/type/string.h>

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/expression/addition/asm.h>
#include <ucsdpsys_assemble/expression/addition/linksym.h>
#include <ucsdpsys_assemble/expression/addition/segrel.h>
#include <ucsdpsys_assemble/expression/bitwise_and/asm.h>
#include <ucsdpsys_assemble/expression/bitwise_not/asm.h>
#include <ucsdpsys_assemble/expression/bitwise_or/asm.h>
#include <ucsdpsys_assemble/expression/dereference/asm.h>
#include <ucsdpsys_assemble/expression/eq/boolean.h>
#include <ucsdpsys_assemble/expression/eq/integer.h>
#include <ucsdpsys_assemble/expression/eq/string.h>
#include <ucsdpsys_assemble/expression/ge/boolean.h>
#include <ucsdpsys_assemble/expression/ge/integer.h>
#include <ucsdpsys_assemble/expression/ge/string.h>
#include <ucsdpsys_assemble/expression/gt/boolean.h>
#include <ucsdpsys_assemble/expression/gt/integer.h>
#include <ucsdpsys_assemble/expression/gt/string.h>
#include <ucsdpsys_assemble/expression/integer/asm.h>
#include <ucsdpsys_assemble/expression/integer_division/asm.h>
#include <ucsdpsys_assemble/expression/le/boolean.h>
#include <ucsdpsys_assemble/expression/le/integer.h>
#include <ucsdpsys_assemble/expression/le/string.h>
#include <ucsdpsys_assemble/expression/lt/boolean.h>
#include <ucsdpsys_assemble/expression/lt/integer.h>
#include <ucsdpsys_assemble/expression/lt/string.h>
#include <ucsdpsys_assemble/expression/modulo/asm.h>
#include <ucsdpsys_assemble/expression/multiplication/asm.h>
#include <ucsdpsys_assemble/expression/name/asm.h>
#include <ucsdpsys_assemble/expression/ne/boolean.h>
#include <ucsdpsys_assemble/expression/ne/integer.h>
#include <ucsdpsys_assemble/expression/ne/string.h>
#include <ucsdpsys_assemble/expression/negate/asm.h>
#include <ucsdpsys_assemble/expression/segment_relative.h>
#include <ucsdpsys_assemble/expression/string/asm.h>
#include <ucsdpsys_assemble/expression/subtraction/asm.h>
#include <ucsdpsys_assemble/expression/subtraction/segrel.h>
#include <ucsdpsys_assemble/expression/xor/asm.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/error.h>
#include <ucsdpsys_assemble/opcode/pseudo/macro.h>
#include <ucsdpsys_assemble/personality/pcode.h>
#include <ucsdpsys_assemble/personality/pseudo.h>
#include <ucsdpsys_assemble/symbol/expression/asm.h>
#include <ucsdpsys_assemble/type/linksym_relative.h>
#include <ucsdpsys_assemble/type/segment_relative.h>


assembler::~assembler()
{
}


assembler::assembler(
    const rcstring &a_code_file_name,
    const output::pointer &a_listing_file
) :
    code_file_name(a_code_file_name),
    lout(a_listing_file),
    mpseudo(personality_pseudo::create(this)),
    procnum(0),
    exit_ic_set(false),
    op_addition("addition"),
    op_subtraction("subtraction"),
    op_eq("equality"),
    op_ne("inequality"),
    op_lt("less than"),
    op_le("less than or equal"),
    op_gt("greater than"),
    op_ge("greater than or equal")
{
#define glue2(x) \
    expression_dispatcher_binary_functor_method<assembler>::create( \
        this, &assembler::x)
    op_addition.push_back
    (
        type_integer::is_a,
        glue2(integer_plus_integer_expression_factory),
        type_integer::is_a
    );
    op_addition.push_back
    (
        type_segment_relative::is_a,
        glue2(segrel_plus_integer_expression_factory),
        type_integer::is_a
    );
    op_addition.push_back
    (
        type_integer::is_a,
        glue2(integer_plus_segrel_expression_factory),
        type_segment_relative::is_a
    );
    op_addition.push_back
    (
        type_linksym_relative::is_a,
        glue2(linksym_plus_integer_expression_factory),
        type_integer::is_a
    );
    op_addition.push_back
    (
        type_integer::is_a,
        glue2(integer_plus_linksym_expression_factory),
        type_linksym_relative::is_a
    );

    op_subtraction.push_back
    (
        type_integer::is_a,
        glue2(integer_minus_integer_expression_factory),
        type_integer::is_a
    );
    op_subtraction.push_back
    (
        type_segment_relative::is_a,
        glue2(segrel_minus_integer_expression_factory),
        type_integer::is_a
    );
    op_subtraction.push_back
    (
        type_segment_relative::is_a,
        glue2(segrel_minus_segrel_expression_factory),
        type_segment_relative::is_a
    );

    op_eq.push_back
    (
        type_integer::is_a,
        glue2(integer_eq_integer_expression_factory),
        type_integer::is_a
    );
    op_eq.push_back
    (
        type_segment_relative::is_a,
        glue2(integer_eq_integer_expression_factory),
        type_segment_relative::is_a
    );
    op_eq.push_back
    (
        type_boolean::is_a,
        glue2(boolean_eq_boolean_expression_factory),
        type_boolean::is_a
    );
    op_eq.push_back
    (
        type_string::is_a,
        glue2(string_eq_string_expression_factory),
        type_string::is_a
    );

    op_ne.push_back
    (
        type_integer::is_a,
        glue2(integer_ne_integer_expression_factory),
        type_integer::is_a
    );
    op_ne.push_back
    (
        type_segment_relative::is_a,
        glue2(integer_ne_integer_expression_factory),
        type_segment_relative::is_a
    );
    op_ne.push_back
    (
        type_boolean::is_a,
        glue2(boolean_ne_boolean_expression_factory),
        type_boolean::is_a
    );
    op_ne.push_back
    (
        type_string::is_a,
        glue2(string_ne_string_expression_factory),
        type_string::is_a
    );

    op_lt.push_back
    (
        type_integer::is_a,
        glue2(integer_lt_integer_expression_factory),
        type_integer::is_a
    );
    op_lt.push_back
    (
        type_segment_relative::is_a,
        glue2(integer_lt_integer_expression_factory),
        type_segment_relative::is_a
    );
    op_lt.push_back
    (
        type_boolean::is_a,
        glue2(boolean_lt_boolean_expression_factory),
        type_boolean::is_a
    );
    op_lt.push_back
    (
        type_string::is_a,
        glue2(string_lt_string_expression_factory),
        type_string::is_a
    );

    op_le.push_back
    (
        type_integer::is_a,
        glue2(integer_le_integer_expression_factory),
        type_integer::is_a
    );
    op_le.push_back
    (
        type_segment_relative::is_a,
        glue2(integer_le_integer_expression_factory),
        type_segment_relative::is_a
    );
    op_le.push_back
    (
        type_boolean::is_a,
        glue2(boolean_le_boolean_expression_factory),
        type_boolean::is_a
    );
    op_le.push_back
    (
        type_string::is_a,
        glue2(string_le_string_expression_factory),
        type_string::is_a
    );

    op_gt.push_back
    (
        type_integer::is_a,
        glue2(integer_gt_integer_expression_factory),
        type_integer::is_a
    );
    op_gt.push_back
    (
        type_segment_relative::is_a,
        glue2(integer_gt_integer_expression_factory),
        type_segment_relative::is_a
    );
    op_gt.push_back
    (
        type_boolean::is_a,
        glue2(boolean_gt_boolean_expression_factory),
        type_boolean::is_a
    );
    op_gt.push_back
    (
        type_string::is_a,
        glue2(string_gt_string_expression_factory),
        type_string::is_a
    );

    op_ge.push_back
    (
        type_integer::is_a,
        glue2(integer_ge_integer_expression_factory),
        type_integer::is_a
    );
    op_ge.push_back
    (
        type_segment_relative::is_a,
        glue2(integer_ge_integer_expression_factory),
        type_segment_relative::is_a
    );
    op_ge.push_back
    (
        type_boolean::is_a,
        glue2(boolean_ge_boolean_expression_factory),
        type_boolean::is_a
    );
    op_ge.push_back
    (
        type_string::is_a,
        glue2(string_ge_string_expression_factory),
        type_string::is_a
    );
}


void
assembler::delayed_open(const rcstring &segname)
{
    // If the machine isn't defined, default to assembling p-code.
    if (!machine)
    {
        grammar_error
        (
            "the machine architecture must be explicitly defined, "
            "either with the .arch pseudo-op, or with the --arch "
            "command line option"
        );
        machine = personality_pcode::create(this);
        lex_default_radix_set(machine->get_default_radix());
    }

    if (!cfp)
    {
        cfp = codefile_file::create(code_file_name, machine->get_byte_sex());
    }
    if (!seg)
    {
        rcstring uc_segname = segname.substring(0, 8).upcase();
        int segnum = cfp->next_free_segment_number(false);
        mtype_t mtype = machine->get_mtype();
        segkind_t segkind = SEPRTSEG;
        switch (mtype)
        {
        case mtype_undefined:
            // Usually segment 0, native code of the operating system.
            // Fall through...

        case mtype_pcode_le:
        case mtype_pcode_be:
            segkind = HOSTSEG;
            break;

        case mtype_pdp11:
        case mtype_8080:
        case mtype_z80:
        case mtype_ga440:
        case mtype_6502:
        case mtype_6800:
        case mtype_ti9900:
            segkind = SEPRTSEG;
            break;
        }
        segversion_t version = segversion_3_0;
        seg = cfp->new_segment(uc_segname, segnum, segkind, mtype, version);
    }
    if (!sbp)
    {
        sbp = segment_builder_code::create(seg, cfp.get());
    }
}


void
assembler::dot_architecture(const rcstring &arch_name)
{
    // p-code is hermaphroditic
    rcstring tname(arch_name);
    rcstring lc_tname = tname.downcase();
    if (lc_tname == "p-code" || lc_tname == "pcode")
    {
        if (!cfp || cfp->get_byte_sex() == little_endian)
            tname = "p-code-le";
        else
            tname = "p-code-be";
    }
    mtype_t mtype = mtype_from_name(tname);
    if (mtype == mtype_undefined)
    {
        mtype = mtype_from_name_fuzzy(tname);
        if (mtype != mtype_undefined)
        {
            grammar_error
            (
                "architecture %s unknown, did you mean %s instead?",
                arch_name.quote_c().c_str(),
                mtype_name(mtype).downcase().quote_c().c_str()
            );
            return;
        }
        grammar_error("architecture %s unknown", arch_name.quote_c().c_str());
        return;
    }

    // If the machine name isn't found, the factory will issue the error.
    personality::pointer p = personality::factory_by_mtype(mtype, this);
    if (!p)
        return;

    machine = p;

    // Make sure the byte sexes agree.
    if (cfp && cfp->get_byte_sex() != machine->get_byte_sex())
    {
        grammar_error
        (
            "byte sex mismatch: given %s, expected %s",
            byte_sex_name(machine->get_byte_sex()).c_str(),
            byte_sex_name(cfp->get_byte_sex()).c_str()
        );
    }

    // Set the preferred radix.
    lex_default_radix_set(machine->get_default_radix());
}


void
assembler::dot_host(const rcstring &host_name)
{
    mtype_t mtype = mtype_from_host(host_name);
    if (mtype == mtype_undefined)
    {
        grammar_error("host %s unknown", host_name.quote_c().c_str());
        return;
    }

    // If the machine name isn't found, the factory will issue the error.
    personality::pointer p = personality::factory_by_mtype(mtype, this);
    if (!p)
        return;

    machine = p;

    // Make sure the byte sexes agree.
    if (cfp && cfp->get_byte_sex() != machine->get_byte_sex())
    {
        grammar_error
        (
            "byte sex mismatch: given %s, expected %s",
            byte_sex_name(machine->get_byte_sex()).c_str(),
            byte_sex_name(cfp->get_byte_sex()).c_str()
        );
    }

    // Set the preferred radix.
    lex_default_radix_set(machine->get_default_radix());
}


void
assembler::proc_begin(const variable_name &name,
    const expression::pointer &nparam, bool is_func)
{
    delayed_open(name.get_name());

    assert(procnum == 0);
    procnum = sbp->next_procedure_number();
    assert(procnum != 0);
    if (procnum < 0)
        grammar_error("too many %ss", (is_func ? "function" : "procedure"));
    sbp->procedure_enter_ic();
    enter_ic = sbp->get_address();
    exit_ic_set = false;

    int nparms = 0;
    if (nparam)
    {
        expression_integer::pointer eip =
            boost::dynamic_pointer_cast<expression_integer>(nparam->optimize());
        if (eip)
        {
            int max = 1024;
            nparms = eip->get_integer_value();
            if (nparms < 0 || nparms > max)
            {
                lex_error
                (
                    nparam->get_location(),
                    "opcode \"%s\": parameter two: "
                        "number of word of  parameters (%d) "
                        "is outside the range 0..%d",
                    (is_func ? ".func" : ".proc"),
                    nparms,
                    max
                );
                nparms = (nparms < 0 ? 0 : max);
            }
        }
        else
        {
            lex_error
            (
                nparam->get_location(),
                "opcode \"%s\": parameter two: integer constant required, "
                    "not a %s",
                    (is_func ? ".func" : ".proc"),
                nparam->get_description().c_str()
            );
        }
    }

    //
    // Linker information
    //
    seg->add_link_information
    (
        link_info::create
        (
            pascal_id(name.get_name()).upcase(),
            (is_func ? SEPFUNC : SEPPROC),
            procnum,
            nparms
        )
    );

    //
    // Define the symbol as a label.
    // and then export as if .def was seen.
    //
    define_label(name, true);
}


void
assembler::export_def(const rcstring &name, unsigned short value)
{
    seg->add_link_information
    (
        link_info::create
        (
            pascal_id(name).upcase(),
            GLOBDEF,
            procnum,
            value
        )
    );
}


void
assembler::emit_globref(const rcstring &name, int nwords)
{
    link_info::pointer lip =
        link_info::create
        (
            pascal_id(name).upcase(),
            GLOBREF,
            opformat_word,
            0,
            nwords
        );
    unsigned addr = sbp->get_address();
    lip->add_reference(addr);
    seg->add_link_information(lip);

    // This seems redundant, but it iss what the Apple Pascal assembler
    // does, so we must do it as well.
    reloc_type2_ref(addr);
}


void
assembler::export_def(const rcstring &name, const expression::pointer &value)
{
    expression::pointer ep = value->optimize();


    {
        // FIXME: This would actually be an absolute symbol
        // and exported differently.  (?CONSTANT_DEF?)
        expression_integer::pointer eip =
            boost::dynamic_pointer_cast<expression_integer>(ep);
        if (eip)
        {
            export_def(name, eip->get_integer_value());
            return;
        }
    }

    {
        expression_segment_relative::pointer esrp =
            boost::dynamic_pointer_cast<expression_segment_relative>(ep);
        if (esrp)
        {
            export_def(name, esrp->get_integer_value());
            return;
        }
    }

    DEBUG(1, "tpye is \"%s\"\n", typeid(*ep).name());
    lex_error
    (
        ep->get_location(),
        "value of %s should be an integer constant, not a %s",
        name.quote_c().c_str(),
        value->get_description().c_str()
    );
}


void
assembler::proc_end(void)
{
    emit_even_alignment();
    if (!exit_ic_set)
    {
        // FIXME: this is an error if it's p-code
        sbp->procedure_exit_ic();
    }

    for (symbols_t::iterator it = symbols.begin(); it != symbols.end(); ++it)
    {
        symbol::pointer sp = **it;
        symbol_expression_asm::pointer sep =
            boost::dynamic_pointer_cast<symbol_expression_asm>(sp);
        if (sep)
        {
            if (!sep->get_value())
            {
                lex_error
                (
                    sep->get_declaration_location(),
                    "symbol %s used but not defined",
                    sep->get_name().quote_c().c_str()
                );
                sep->set_value
                (
                    expression_error::create(sep->get_declaration_location())
                );
            }
        }
    }

    //
    // Resolve all the outstanding code patch requests.
    //
    // FIXME: how does this work for p-code jump tables?
    for
    (
        patches_t::const_iterator it = patches.begin();
        it != patches.end();
        ++it
    )
    {
        patch::pointer pp = *it;
        assert(pp);
        DEBUG(2, "patch = %s", pp->representation().c_str());
        if (!pp->apply(sbp))
        {
            // FIXME: Need to turn this one into linker information,
            // that goes between the exit-ic and the procedure attributes
            // (but it's an error for p-code)
            pp->error_too_complex();
        }
    }
    patches.clear();
    symbols.remove(local_symbol_names);
    local_symbol_names.clear();

    code_source_map::pointer listing;
    sbp->procedure_end(procnum, 0, listing, machine->get_mtype());
    procnum = 0;
}


void
assembler::write_code_file(void)
{
    cfp->write_segment_dictionary();
}


void
assembler::emit_byte(unsigned char n)
{
    if (lout.addr_empty())
        lout.addr_printf("%04X:", get_address());
    assert(sbp);
    sbp->emit_byte(n);
    lout.data_printf("%02X ", n);
}


void
assembler::emit_one_byte_nop(void)
{
    sbp->emit_byte(machine ? machine->get_one_byte_nop() : 0);
}


void
assembler::emit_byte(const expression::pointer &value)
{
    // If at all possible, emit the value immediately, that way there is
    // less to remember, and checking afterwards is faster, too.
    expression::pointer ep = value->optimize();
    expression_integer::pointer eip =
        boost::dynamic_pointer_cast<expression_integer>(ep);
    if (eip)
    {
        emit_byte((unsigned char)eip->get_integer_value());
        return;
    }

    assert(sbp);
    // Note: we use sbp->get_address() intentionally, because
    // assembler::get_address() has had enter_ic subtracted from it.
    patch::kind_t kind = patch::kind_byte;
    patches.push_back(patch::create(sbp->get_address(), kind, value));
    emit_byte(0);
}


void
assembler::emit_word(unsigned short n)
{
    if (lout.addr_empty())
        lout.addr_printf("%04X:", get_address());
    assert(sbp);
    sbp->emit_word(n);
    lout.data_printf("%04X ", n);
}


void
assembler::emit_word(const expression::pointer &value)
{
    // If at all possible, emit the value immediately, that way there is
    // less to remember, and checking afterwards is faster, too.
    expression::pointer ep = value->optimize();
    expression_integer::pointer eip =
        boost::dynamic_pointer_cast<expression_integer>(ep);
    if (eip)
    {
        emit_word((unsigned short)eip->get_integer_value());
        return;
    }

    patch::kind_t kind = patch::kind_word_le;
    assert(machine);
    if (machine->get_byte_sex() == big_endian)
        kind = patch::kind_word_be;
    // Note: we use sbp->get_address() intentionally, because
    // assembler::get_address() has had enter_ic subtracted from it.
    patches.push_back(patch::create(sbp->get_address(), kind, value));
    emit_word(0);
}


void
assembler::emit_big(unsigned short n)
{
    assert(sbp);
    sbp->emit_big(n);

    if (lout.addr_empty())
        lout.addr_printf("%04X:", get_address());
    if (n < 128)
        lout.data_printf("%02X ", n);
    else
    {
        lout.data_printf("%02X ", (0x80 | (n >> 8)));
        lout.data_printf("%02X ", (n & 255));
    }
}


void
assembler::emit_big(const expression::pointer &value)
{
    // If at all possible, emit the value immediately, that way there is
    // less to remember, and checking afterwards is faster, too.
    expression::pointer ep = value->optimize();
    expression_integer::pointer eip =
        boost::dynamic_pointer_cast<expression_integer>(ep);
    if (eip)
    {
        // do it immediately
        emit_big((unsigned short)eip->get_integer_value());
        return;
    }

    // Note: we use sbp->get_address() intentionally, because
    // assembler::get_address() has had enter_ic subtracted from it.
    patch::kind_t kind = patch::kind_big;
    patches.push_back(patch::create(sbp->get_address(), kind, value));
    emit_byte(0);

    // If the big value is only one byte, this NOP will actually be used.
    emit_one_byte_nop();
}


void
assembler::emit_even_alignment(void)
{
    assert(sbp);
    sbp->emit_word_alignment();
}


void
assembler::define_label(const variable_name &name, bool dot_def)
{
    expression::pointer value = dollar_expression_factory(name.get_location());
    define_symbol(name, value, dot_def);
}


expression::pointer
assembler::dollar_expression_factory(const location &locn, int offset)
{
    long addr = get_address() + offset;
    return expression_segment_relative::create(locn, addr);
}


expression::pointer
assembler::enter_ic_expression_factory(const location &locn)
{
    // see assembler::get_address, below
    // enter_ic - enter_ic = 0
    return expression_segment_relative::create(locn, 0);
}


unsigned
assembler::get_address(void)
    const
{
    return sbp->get_address() - enter_ic;
}


void
assembler::define_symbol(const variable_name &name,
    const expression::pointer &value, bool dot_def)
{
    rcstring lc_name = name.get_name().downcase();
    if (name.get_name()[0] != '$')
    {
        // FIXME: make sure all temporary symbols (start with '$') have
        // been defined.
        symbols.remove(temporary_symbol_names);
        temporary_symbol_names.clear();
    }

    // We have to be careful to issue an error if they redefine a symbol.
    // Defining a value for a previously seen, but undefined, symbol is OK.
    {
        symbol_expression_asm::pointer sp =
            boost::dynamic_pointer_cast<symbol_expression_asm>(
                symbols.lookup(lc_name));
        if (sp)
        {
            if (sp->get_value())
            {
                lex_error
                (
                    value->get_location(),
                    "symbol %s has already been defined...",
                    name.get_name().quote_c().c_str()
                );
                lex_error
                (
                    sp->get_value()->get_location(),
                    "... here is the earlier %s definition",
                    sp->get_name().quote_c().c_str()
                );
            }
            else
            {
                if (sp->get_type() != value->get_type())
                {
                    lex_error
                    (
                        value->get_location(),
                        "forward symbol %s type mismatch: given a %s, "
                            "expected a %s",
                        sp->get_name().quote_c().c_str(),
                        value->get_type()->get_name().c_str(),
                        sp->get_type()->get_name().c_str()
                    );
                }
                else
                {
                    sp->set_value(value);
                    if (dot_def)
                        sp->export_def();
                }
            }
            return;
        }
    }

    symbol_expression_asm::pointer sp =
        symbol_expression_asm::create(name, value->get_type(), value, this);
    symbols.assign(sp->get_name().downcase(), sp);
    if (dot_def)
        sp->export_def();

    //
    // Remember temporary symbols, so we can nuke them later.
    //
    if (name.get_name()[0] == '$')
        temporary_symbol_names.push_back(sp->get_name());
    else if (procnum > 0)
        local_symbol_names.push_back(lc_name);

    //
    // Special handling for the "exit_ic" label, it tells the assmbler
    // wher the exit code starts, for p-code.  For natiev assembler, the
    // exit-ic in the procedure attributes table means somethign else.
    //
    if (name.get_name() == "exit_ic")
    {
        // this is an error if it's not p-code
        if
        (
            !machine
        ||
            (
                machine->get_mtype() != mtype_pcode_le
            &&
                machine->get_mtype() != mtype_pcode_le
            )
        )
        {
            lex_error
            (
                value->get_location(),
                "the exit_ic name is reserved for p-code"
            );
        }
        else
        {
            assert(sbp);
            sbp->procedure_exit_ic();
            exit_ic_set = true;
        }
    }
}


expression::pointer
assembler::name_expression_factory(const variable_name &name)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "name = %s", name.get_name().quote_c().c_str());
    DEBUG(2, "locn = %s", name.get_location().representation().c_str());
    symbol::pointer sp = symbols.lookup(name.get_name().downcase());
    if (!sp)
    {
        expression::pointer value;
        type::pointer tp = type_segment_relative::create();
        sp = symbol_expression_asm::create(name, tp, value, this);

        rcstring lc_name = sp->get_name().downcase();
        symbols.assign(lc_name, sp);

        //
        // Remember temporary symbols, so we can nuke them later.
        //
        if (lc_name[0] == '$')
            temporary_symbol_names.push_back(lc_name);
        else if (procnum > 0)
            local_symbol_names.push_back(lc_name);
    }
    return expression_name_asm::create(name.get_location(), sp);
}


expression::pointer
assembler::integer_expression_factory(const location &locn, long value)
{
    return expression_integer_asm::create(value, locn);
}


expression::pointer
assembler::string_expression_factory(const location &locn,
    const rcstring &value)
{
    return expression_string_asm::create(locn, value);
}


expression::pointer
assembler::negate_expression_factory(const expression::pointer &operand)
{
    // check the operand
    if (!type_integer::is_a(operand))
    {
        lex_error
        (
            operand->get_location(),
            "unary minus expects an integer, not a %s",
            operand->get_description().c_str()
        );
        return expression_error::create(operand->get_location());
    }
    return expression_negate_asm::create(operand, this);
}


expression::pointer
assembler::unary_plus_expression_factory(const expression::pointer &operand)
{
    // check the operand
    if (!type_integer::is_a(operand))
    {
        lex_error
        (
            operand->get_location(),
            "unary plus expects an integer, not a %s",
            operand->get_description().c_str()
        );
        return expression_error::create(operand->get_location());
    }
    return operand;
}


expression::pointer
assembler::bitwise_not_expression_factory(const expression::pointer &operand)
{
    if (operand->is_error())
        return operand;
    if (!type_integer::is_a(operand))
    {
        lex_error
        (
            operand->get_location(),
            "bit-wise not expects an integer, not a %s",
            operand->get_description().c_str()
        );
        return expression_error::create(operand->get_location());
    }
    return expression_bitwise_not_asm::create(operand, this);
}


opcode::pointer
assembler::lookup_opcode(const rcstring &name)
{
    // First check to see if it is a pseudo-op.
    {
        assert(mpseudo);
        opcode::pointer op = mpseudo->lookup_opcode(name);
        if (op)
            return op;
        if (name[0] == '.')
        {
            grammar_error("pseudo-op %s unknown", name.quote_c().c_str());
            return opcode_error::create(this, name);
        }
    }

    // If the machine yet to be defined defined, fail here.
    if (!machine)
        return opcode::pointer();

    // Does the given machine have the opcode named?
    return machine->lookup_opcode(name);
}


expression::pointer
assembler::lookup_special(const rcstring &name)
{
    if (machine)
    {
        expression::pointer ep = machine->lookup_special(name);
        if (ep)
            return ep;
    }

    // just in case they said
    //     oh   equ 123.
    //     crap equ x
    //          sta (oh), crap
    symbol_expression::pointer sep =
        boost::dynamic_pointer_cast<symbol_expression>(symbols.lookup(name));
    if (!sep)
        return expression::pointer();
    expression_special::pointer esp =
        boost::dynamic_pointer_cast<expression_special>(sep->get_value());
    return esp;
}


void
assembler::dot_end(void)
{
    sbp->complete();
    sbp.reset();
    // Should we seg.reset(); here as well?
}


expression::pointer
assembler::addition_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return op_addition.dispatch(lhs, rhs);
}


expression::pointer
assembler::integer_plus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_asm::create(lhs, rhs);
}


expression::pointer
assembler::segrel_plus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_segrel::create(lhs, rhs);
}


expression::pointer
assembler::integer_plus_segrel_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    // note: swapped operands
    return expression_addition_segrel::create(rhs, lhs);
}


expression::pointer
assembler::linksym_plus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_linksym::create(lhs, rhs);
}


expression::pointer
assembler::integer_plus_linksym_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    // note: swapped operands
    return expression_addition_linksym::create(rhs, lhs);
}


expression::pointer
assembler::dereference_expression_factory(const expression::pointer &opnd)
{
    return expression_dereference_asm::create(opnd);
}


expression::pointer
assembler::subtraction_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return op_subtraction.dispatch(lhs, rhs);
}


expression::pointer
assembler::integer_minus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_asm::create(lhs, rhs);
}


expression::pointer
assembler::segrel_minus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_segrel::create(lhs, rhs);
}


expression::pointer
assembler::segrel_minus_segrel_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_asm::create(lhs, rhs);
}


expression::pointer
assembler::division_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_integer_division_asm::create(lhs, rhs);
}


expression::pointer
assembler::modulo_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_modulo_asm::create(lhs, rhs, this);
}


expression::pointer
assembler::multiplication_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_multiplication_asm::create(lhs, rhs);
}


expression::pointer
assembler::bitwise_and_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    if (lhs->is_error())
        return lhs;
    if (rhs->is_error())
        return rhs;
    if (!type_integer::is_a(lhs) || !type_integer::is_a(rhs))
    {
        lex_error
        (
            lhs->get_location(),
            "bitwise-and requires two integers, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(lhs->get_location());
    }
    return expression_bitwise_and_asm::create(lhs, rhs, this);
}


expression::pointer
assembler::bitwise_or_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    if (lhs->is_error())
        return lhs;
    if (rhs->is_error())
        return rhs;
    if (!type_integer::is_a(lhs) || !type_integer::is_a(rhs))
    {
        lex_error
        (
            lhs->get_location(),
            "bitwise-or requires two integers, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(lhs->get_location());
    }
    return expression_bitwise_or_asm::create(lhs, rhs, this);
}


expression::pointer
assembler::xor_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_xor_asm::create(lhs, rhs, this);
}


void
assembler::set_host(const char *name)
{
    dot_host(name);
}


void
assembler::set_architecture(const char *name)
{
    dot_architecture(name);
}


void
assembler::define_macro(const variable_name &name,
    const rcstring_list &contents)
{
    // FIXME: the macro should probably exist in the symbol table, if it
    // is in the .proc section, and should be forgotten at the end of
    // the procedure.
    rcstring body = contents.unsplit("");
    DEBUG(2, "body = %s", body.quote_c().c_str());
    opcode::pointer op =
        opcode_pseudo_macro::create(name.get_name(), body, this);
    mpseudo->register_opcode(op);
}


void
assembler::dot_if(const expression::pointer &arg)
{
    expression::pointer ep = arg->optimize();
    if (type_boolean::is_a(ep) && ep->is_constant())
    {
        lex_conditional_push(ep->get_boolean_value());
    }
    else if (type_integer::is_a(ep) && ep->is_constant())
    {
        int n = ep->get_integer_value();
        lex_conditional_push(n != 0);
    }
    else
    {
        lex_error
        (
            ep->get_location(),
            "expected a constant boolean expression, not a %s",
            ep->get_description().c_str()
        );
        lex_conditional_push(true);
    }
}


expression::pointer
assembler::eq_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return op_eq.dispatch(lhs, rhs);
}


expression::pointer
assembler::boolean_eq_boolean_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_boolean::create(lhs, rhs);
}


expression::pointer
assembler::integer_eq_integer_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_integer::create(lhs, rhs);
}


expression::pointer
assembler::string_eq_string_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_string::create(lhs, rhs);
}


void
assembler::reloc_type1_lcrel(unsigned pc)
{
    sbp->reloc_type1_lcrel(pc);
}


void
assembler::reloc_type1_lcrel(void)
{
    sbp->reloc_type1_lcrel(sbp->get_address());
}


void
assembler::reloc_type2_ref(unsigned pc)
{
    sbp->reloc_type2_ref(pc);
}


void
assembler::reloc_type3_pubpriv(unsigned pc)
{
    sbp->reloc_type3_pubpriv(pc);
}


expression::pointer
assembler::ne_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return op_ne.dispatch(lhs, rhs);
}


expression::pointer
assembler::boolean_ne_boolean_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_ne_boolean::create(lhs, rhs);
}


expression::pointer
assembler::integer_ne_integer_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_ne_integer::create(lhs, rhs);
}


expression::pointer
assembler::string_ne_string_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_ne_string::create(lhs, rhs);
}


expression::pointer
assembler::lt_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return op_lt.dispatch(lhs, rhs);
}


expression::pointer
assembler::boolean_lt_boolean_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_lt_boolean::create(lhs, rhs);
}


expression::pointer
assembler::integer_lt_integer_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_lt_integer::create(lhs, rhs);
}


expression::pointer
assembler::string_lt_string_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_lt_string::create(lhs, rhs);
}


expression::pointer
assembler::le_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return op_le.dispatch(lhs, rhs);
}


expression::pointer
assembler::boolean_le_boolean_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_le_boolean::create(lhs, rhs);
}


expression::pointer
assembler::integer_le_integer_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_le_integer::create(lhs, rhs);
}


expression::pointer
assembler::string_le_string_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_le_string::create(lhs, rhs);
}


expression::pointer
assembler::gt_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return op_gt.dispatch(lhs, rhs);
}


expression::pointer
assembler::boolean_gt_boolean_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_gt_boolean::create(lhs, rhs);
}


expression::pointer
assembler::integer_gt_integer_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_gt_integer::create(lhs, rhs);
}


expression::pointer
assembler::string_gt_string_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_gt_string::create(lhs, rhs);
}


expression::pointer
assembler::ge_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return op_ge.dispatch(lhs, rhs);
}


expression::pointer
assembler::boolean_ge_boolean_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_ge_boolean::create(lhs, rhs);
}


expression::pointer
assembler::integer_ge_integer_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_ge_integer::create(lhs, rhs);
}


expression::pointer
assembler::string_ge_string_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_ge_string::create(lhs, rhs);
}


void
assembler::listing_eoln(void)
{
    lout.eoln();
}


void
assembler::dot_title(const rcstring &text)
{
    lout.set_heading1(text);
}


void
assembler::dot_sbttl(const rcstring &text)
{
    lout.set_heading2(text);
}


// vim: set ts=8 sw=4 et :
