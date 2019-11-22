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

#include <lib/debug.h>
#include <lib/pascal/lex.h>
#include <lib/segment/builder/interface.h>
#include <lib/symbol/expression/constant.h>

#include <ucsdpsys_compile/scope/interface/compile.h>
#include <ucsdpsys_compile/symbol/variable/external/compile.h>


scope_interface_compile::~scope_interface_compile()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


scope_interface_compile::scope_interface_compile(
    const rcstring &a_name,
    int a_code_seg,
    int a_data_seg,
    segkind_t a_segkind,
    translator_compile *a_context
) :
    scope_interface(a_name),
    code_seg(a_code_seg),
    data_seg(a_data_seg),
    segkind(a_segkind),
    context(a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(code_seg > 0);
    assert(data_seg > 0 || data_seg == -1);
}


scope_interface_compile::pointer
scope_interface_compile::create(const rcstring &a_name, int a_code_seg,
    int a_data_seg, segkind_t a_segkind, translator_compile *a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return
        pointer
        (
            new scope_interface_compile
            (
                a_name,
                a_code_seg,
                a_data_seg,
                a_segkind,
                a_context
            )
        );
}


segment_builder::pointer
scope_interface_compile::segment_builder_factory()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return segment_builder_interface::create(get_name(), code_seg);
}


symbol::pointer
scope_interface_compile::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return symbol_expression_constant::create(vname, lex_level, value);
}


symbol_variable::pointer
scope_interface_compile::parameter_factory(const variable_name &,
    const type::pointer &, int)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(!"units don't have parameters");
    return symbol_variable::pointer();
}


symbol_variable::pointer
scope_interface_compile::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return
        symbol_variable_external_compile::create
        (
            vname,
            vtype,
            data_seg,
            context
        );
}


bit_address
scope_interface_compile::allocate_space(const type::pointer &tp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (data_seg < 0)
        return bit_address();
    // we are dealing with a DATASEG
    unsigned nbits = tp->get_size_in_words() * 16;
    return get_next_bit_address(nbits);
}


bit_address
scope_interface_compile::get_next_bit_address(unsigned nbits)
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
