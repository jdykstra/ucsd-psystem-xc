//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/expression/linker_reference.h>
#include <ucsdpsys_assemble/type/linksym_relative.h>


expression_linker_reference::~expression_linker_reference()
{
}


expression_linker_reference::expression_linker_reference(
    const location &a_locn,
    link_info_type_t a_litype,
    const type::pointer &a_tp,
    int a_offset,
    assembler *a_context
) :
    expression(a_locn),
    litype(a_litype),
    tp(a_tp),
    offset(a_offset),
    context(a_context)
{
}


expression_linker_reference::pointer
expression_linker_reference::create(const location &a_locn,
    link_info_type_t a_litype, const type::pointer &a_tp, int a_offset,
    assembler *a_context)
{
    return
        pointer
        (
            new expression_linker_reference
            (
                a_locn,
                a_litype,
                a_tp,
                a_offset,
                a_context
            )
        );
}


void
expression_linker_reference::traversal(int)
    const
{
    assert(litype == GLOBREF);
    type_linksym_relative::pointer r =
        boost::dynamic_pointer_cast<type_linksym_relative>(tp);
    assert(r);
    context->emit_globref(r->get_symbol_name());
    context->emit_word(offset);
}


int
expression_linker_reference::get_precedence(void)
    const
{
    return prec_id;
}


type::pointer
expression_linker_reference::get_type(void)
    const
{
    return tp;
}


bool
expression_linker_reference::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_linker_reference::has_side_effect(void)
    const
{
    return side_effect_no;
}


expression::pointer
expression_linker_reference::optimize(void)
    const
{
    return create(get_location(), litype, tp, offset, context);
}


rcstring
expression_linker_reference::lisp_representation(void)
    const
{
    return
        rcstring::printf
        (
            "(linker-reference {%s} %s %d)",
            tp->get_name().c_str(),
            link_info_type_name(litype).c_str(),
            offset
        );
}


expression::pointer
expression_linker_reference::clone(const location &locn)
    const
{
    return create(locn, litype, tp, offset, context);
}


expression::pointer
expression_linker_reference::addition_factory(int delta)
    const
{
    return create(get_location(), litype, tp, offset + delta, context);
}


bool
expression_linker_reference::is_constant(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
