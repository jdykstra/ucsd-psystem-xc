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

#include <lib/expression/nil.h>
#include <lib/type/anything.h>
#include <lib/type/pointer/unpacked.h>


expression_nil::~expression_nil()
{
}


expression_nil::expression_nil(const location &locn) :
    expression(locn)
{
}


void
expression_nil::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"this isn't supposed to happen");
}


int
expression_nil::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_nil::get_type()
    const
{
    return type_pointer_unpacked::create(type_anything::create());
}


bool
expression_nil::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_nil::has_side_effect()
    const
{
    return side_effect_no;
}


bool
expression_nil::is_constant()
    const
{
    return true;
}


rcstring
expression_nil::lisp_representation(void)
    const
{
    return "nil";
}


// vim: set ts=8 sw=4 et :
