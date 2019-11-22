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

#include <lib/expression/empty.h>
#include <lib/type/nothing.h>


expression_empty::~expression_empty()
{
}


expression_empty::expression_empty(const location &locn) :
    expression(locn)
{
}


expression_empty::pointer
expression_empty::create(const location &locn)
{
    return pointer(new expression_empty(locn));
}


type::pointer
expression_empty::get_type()
    const
{
    return type_nothing::create();
}


void
expression_empty::traversal(int)
    const
{
}


void
expression_empty::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
}


int
expression_empty::get_precedence(void)
    const
{
    return 0;
}


bool
expression_empty::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_empty::has_side_effect(void)
    const
{
    return side_effect_no;
}


expression::pointer
expression_empty::optimize(void)
    const
{
    return create(get_location());
}


rcstring
expression_empty::lisp_representation(void)
    const
{
    return "(empty)";
}


expression::pointer
expression_empty::clone(const location &locn)
    const
{
    return create(locn);
}


// vim: set ts=8 sw=4 et :
