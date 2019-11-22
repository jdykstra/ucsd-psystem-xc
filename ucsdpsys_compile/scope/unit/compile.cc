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
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/symbol/expression/constant.h>
#include <lib/symbol/variable.h>
#include <lib/type/integer.h>

#include <ucsdpsys_compile/scope/unit/compile.h>
#include <ucsdpsys_compile/symbol/functor/constructor.h>
#include <ucsdpsys_compile/symbol/functor/destructor.h>
#include <ucsdpsys_compile/symbol/variable/external/compile.h>
#include <ucsdpsys_compile/symbol/variable/normal/publref.h>
#include <ucsdpsys_compile/symbol/variable/normal/privref.h>
#include <ucsdpsys_compile/translator/compile.h>


scope_unit_compile::~scope_unit_compile()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


scope_unit_compile::scope_unit_compile(
    const rcstring &a_name,
    int a_code_seg,
    int a_data_seg,
    int a_llht,
    segkind_t a_segkind,
    translator_compile *a_code
) :
    scope_unit(a_name, a_code_seg, a_data_seg, a_llht),
    code(*a_code),
    proc_num(0),
    implementation_phase(false),
    segkind(a_segkind)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    code.new_segment(get_name(), a_code_seg, segkind);
    proc_num = code.next_procedure_number();
    assert(proc_num == 1);
    if (proc_num < 0)
        pascal_grammar_error("too many procedures");
}


scope_unit_compile::pointer
scope_unit_compile::create(const rcstring &a_name, int a_code_seg,
    int a_data_seg, int a_llht, segkind_t a_segkind, translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return
        pointer
        (
            new scope_unit_compile
            (
                a_name,
                a_code_seg,
                a_data_seg,
                a_llht,
                a_segkind,
                a_code
            )
        );
}


scope_unit_compile::pointer
scope_unit_compile::create(const rcstring &a_name, int a_code_seg,
    int a_data_seg, int a_llht, translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return
        pointer
        (
            new scope_unit_compile
            (
                a_name,
                a_code_seg,
                a_data_seg,
                a_llht,
                LINKED_INTRINS,
                a_code
            )
        );
}


void
scope_unit_compile::unit_interface_begin(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    implementation_phase = false;
}


void
scope_unit_compile::unit_implementation_begin(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    implementation_phase = true;
}


void
scope_unit_compile::pre_order_traversal()
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    // From this point on, only code for this one procedure is being
    // injected into the segment's code space.  We have also passed the
    // point in the parse where any more variables can be declared.
    code.procedure_parameter_size(get_param_size_in_bytes());
    code.procedure_variable_size(get_data_size_in_bytes());
    code.procedure_enter_ic();
}


void
scope_unit_compile::post_order_traversal()
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    location loc = pascal_lex_location();
    code.procedure_exit_ic();

    //
    // Emit return opcode(s)
    //
    code.breakpoint(loc);
    unsigned words_of_return_value = 0;
    code.emit_byte(RNP);
    code.emit_byte(words_of_return_value);
    code.emit_word_alignment(NOP);

    //
    // Insert details into procedure dictionary.
    //
    code.procedure_end(proc_num, 1);

    //
    // Insert details into segment dictionary.
    //
    code.segment_end();

    //
    // Last of all, create the external DATASEG.
    //
    if (get_data_seg() >= 0)
    {
        DEBUG(2, "abracadabra");
        int nwords = dataseg_size_highwater.get_size_in_words();
        if (nwords <= 0)
        {
            pascal_lex_warning
            (
                get_body()->get_location(),
                "intrinsic unit %s has a data segment declared, "
                    "but there are no unit variables",
                get_name().quote_c().c_str()
            );
        }
        else
        {
            code.create_dataseg(get_name(), get_data_seg(), nwords);
        }
    }
}


void
scope_unit_compile::constant_declaration_notify(const variable_name &,
    const expression::pointer &)
{
    // Do nothing.
}


void
scope_unit_compile::type_declaration_notify(const variable_name &,
    const type::pointer &)
{
    // Do nothing.
}


void
scope_unit_compile::variable_declaration_notify(const symbol_variable_p &)
{
    // Do nothing.
}


symbol::pointer
scope_unit_compile::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return symbol_expression_constant::create(vname, lex_level, value);
}


symbol_variable::pointer
scope_unit_compile::parameter_factory(const variable_name &,
    const type::pointer &, int)
{
    assert(!"units do not have parameters");
}


symbol_variable::pointer
scope_unit_compile::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    // There are several choices:
    // 1. if this is a simple unit, use a symbol_variable_publref derivative
    // 2. if this is an intrinsic unit, use a symbol_variable_external deriv
    // 3. if this is an intrinsic unit w/o data, it is an error
    // 4. if this is a separate unit, it is an error
    switch (segkind)
    {
    case UNITSEG:
        // this is a plain unit
        plain:
        if (implementation_phase)
            return symbol_variable_normal_privref::create(vname, vtype, &code);
        return symbol_variable_normal_publref::create(vname, vtype, &code);

    case LINKED_INTRINS:
    case UNLINKED_INTRINS:
    case DATASEG:
        // this is an instrinsic unit
        if (get_data_seg() < 0)
        {
            pascal_lex_error
            (
                vname.get_location(),
                "intrinsic unit %s has not declared a data segment number",
                get_name().quote_c().c_str()
            );
        }
        return
            symbol_variable_external_compile::create
            (
                vname,
                vtype,
                get_data_seg(),
                &code
            );

    case SEPRTSEG:
        // this is a separate unit
        pascal_lex_error
        (
            vname.get_location(),
            "separate unit %s is not permitted to have global variables",
            get_name().quote_c().c_str()
        );
        goto plain;

    case SEGPROC:
    case LINKED:
    case HOSTSEG:
    default:
        pascal_lex_error
        (
            vname.get_location(),
            "%s unit %s does not know how to have global variables",
            segkind_name(segkind).c_str(),
            get_name().quote_c().c_str()
        );
        assert(!"should not be possible");
        goto plain;
    }
}


bit_address
scope_unit_compile::allocate_space(const type::pointer &tp)
{
    // See comment in scope_unit_compile::variable_factory for why.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);

    switch (segkind)
    {
    case UNITSEG:
        // unit
        //
        // We don't actually allocate any space nere, because it
        // actually gets allocated by USES declarations, when the unit
        // is actually used.
        //
        return bit_address();

    case LINKED_INTRINS:
    case UNLINKED_INTRINS:
    case DATASEG:
        // intrinsic unit
        {
            if (get_data_seg() < 0)
                return bit_address();
            // we are dealing with a DATASEG
            unsigned nbits = tp->get_size_in_words() * 16;
            return get_next_bit_address(nbits);
        }

    case SEPRTSEG:
        // separate unit
        return bit_address();

    case LINKED:
    case HOSTSEG:
    case SEGPROC:
    default:
        return bit_address();
    }
}


bit_address
scope_unit_compile::get_next_bit_address(unsigned nbits)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(nbits > 0);
    assert(!(nbits & 15) || nbits <= 16);
    nbits = (nbits + 15) & ~15;
    bit_address result = dataseg_size_current;
    dataseg_size_current.advance_by_bits(nbits);
    if (dataseg_size_highwater < dataseg_size_current)
        dataseg_size_highwater = dataseg_size_current;
    return result;
}


// vim: set ts=8 sw=4 et :
