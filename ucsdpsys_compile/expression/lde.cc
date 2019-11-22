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

#include <lib/pcode.h>

#include <ucsdpsys_compile/expression/lde.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_lde::~expression_lde()
{
}


expression_lde::expression_lde(
    const location &a_locn,
    const type::pointer &a_ptype,
    int a_segnum,
    int a_offset,
    translator_compile *a_context
) :
    expression(a_locn),
    ptype(a_ptype),
    segnum(a_segnum),
    offset(a_offset),
    context(a_context)
{
}


expression_lde::pointer
expression_lde::create(const location &a_locn, const type::pointer &a_ptype,
    int a_segnum, int a_offset, translator_compile *a_context)
{
    return
        pointer
        (
            new expression_lde(a_locn, a_ptype, a_segnum, a_offset, a_context)
        );
}


void
expression_lde::traversal(int)
    const
{
    context->emit_byte(LDE);
    context->emit_byte(segnum);
    context->emit_big(offset);
}


int
expression_lde::get_precedence(void) const
{
    return prec_id;
}


type::pointer
expression_lde::get_type(void) const
{
    return ptype;
}


bool
expression_lde::is_lvalue(void) const
{
    return false;
}


expression::side_effect_t
expression_lde::has_side_effect(void) const
{
    return side_effect_no;
}


expression::pointer
expression_lde::optimize(void)
    const
{
    return create(get_location(), ptype, segnum, offset, context);
}


rcstring
expression_lde::lisp_representation(void) const
{
    return rcstring::printf("(lde %d %d)", segnum, offset);
}


expression::pointer
expression_lde::clone(const location &where)
    const
{
    return create(where, ptype, segnum, offset, context);
}


// vim: set ts=8 sw=4 et :
