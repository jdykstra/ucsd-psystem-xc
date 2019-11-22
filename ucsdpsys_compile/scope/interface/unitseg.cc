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

#include <ucsdpsys_compile/scope/interface/unitseg.h>
#include <ucsdpsys_compile/symbol/variable/normal/publref.h>
#include <ucsdpsys_compile/translator/compile.h>


scope_interface_unitseg::~scope_interface_unitseg()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


scope_interface_unitseg::scope_interface_unitseg(
    const rcstring &a_name,
    int a_code_seg,
    int a_data_seg,
    const scope::pointer &a_parent,
    translator_compile *a_context
) :
    scope_interface(a_name),
    code_seg(a_code_seg),
    data_seg(a_data_seg),
    parent(a_parent),
    context(a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(code_seg > 0);
    assert(parent);
}


scope_interface_unitseg::pointer
scope_interface_unitseg::create(const rcstring &a_name, int a_code_seg,
    int a_data_seg, const scope::pointer &a_parent,
    translator_compile *a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return
        pointer
        (
            new scope_interface_unitseg
            (
                a_name,
                a_code_seg,
                a_data_seg,
                a_parent,
                a_context
            )
        );
}


segment_builder::pointer
scope_interface_unitseg::segment_builder_factory()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return segment_builder_interface::create(get_name(), code_seg);
}


symbol::pointer
scope_interface_unitseg::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return symbol_expression_constant::create(vname, lex_level, value);
}


symbol_variable::pointer
scope_interface_unitseg::parameter_factory(const variable_name &,
    const type::pointer &, int)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(!"unit interfaces don't have parameters");
    return symbol_variable::pointer();
}


symbol_variable::pointer
scope_interface_unitseg::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    lex_level = 0;
    return parent->variable_factory(vname, vtype, lex_level);
}


void
scope_interface_unitseg::variable_declaration_notify(
    const symbol_variable::pointer &svp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    context->link_publdef
    (
        svp->get_name(),
        svp->get_bit_offset().get_word_addr()
    );
}


bit_address
scope_interface_unitseg::allocate_space(const type::pointer &tp)
{
    // See comment in scope_unit_compile::variable_factory for why.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return parent->allocate_space(tp);
}


// vim: set ts=8 sw=4 et :
