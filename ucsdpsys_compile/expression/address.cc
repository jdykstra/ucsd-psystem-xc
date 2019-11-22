//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/type/pointer.h>
#include <ucsdpsys_compile/expression/address.h>


expression_address::~expression_address()
{
}


expression_address::expression_address(const expression_address &arg) :
    expression(arg),
    offset(arg.offset),
    ptype(arg.ptype)
{
}


expression_address::expression_address(
    const location &a_locn,
    int a_off,
    const type::pointer &a_ptype
) :
    expression(a_locn),
    offset(a_off),
    ptype(a_ptype)
{
    assert(type_pointer::is_a(a_ptype));
    assert(a_ptype->get_subtype()->get_size_in_words() > 0);
}


void
expression_address::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"shouldn't happen, not boolean");
}


int
expression_address::get_precedence()
    const
{
    return prec_unary;
}


type::pointer
expression_address::get_type()
    const
{
    return ptype;
}


bool
expression_address::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_address::has_side_effect()
    const
{
    return side_effect_no;
}


// vim: set ts=8 sw=4 et :
