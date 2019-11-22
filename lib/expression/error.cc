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

#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/type/error.h>


expression_error::~expression_error()
{
}


expression_error::expression_error(const location &arg) :
    expression(arg)
{
}


expression_error::pointer
expression_error::create(const location &arg)
{
    return pointer(new expression_error(arg));
}


expression_error::expression_error(const expression_error &rhs) :
    expression(rhs)
{
}


void
expression_error::traversal(int)
    const
{
    // Do nothing.
}


int
expression_error::get_precedence()
    const
{
    return 0;
}


type::pointer
expression_error::get_type()
    const
{
    return type_error::create();
}


bool
expression_error::is_lvalue()
    const
{
    // Return true, it side-steps all of the "not an lvalue" secondary
    // warnings.
    return true;
}


expression::side_effect_t
expression_error::has_side_effect()
    const
{
    // Return yes, it side-steps all of the "statement has no effect"
    // secondary warnings.
    return side_effect_yes;
}


void
expression_error::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    // Do nothing.
}


expression::pointer
expression_error::assignment_expression_factory(
    const expression::pointer &)
{
    return clone(get_location());
}


expression::pointer
expression_error::function_call_expression_factory(const expression_list &)
{
    return clone(get_location());
}


expression::pointer
expression_error::dot_expression_factory(const location &,
    const variable_name &)
{
    return clone(get_location());
}


expression::pointer
expression_error::optimize()
    const
{
    return clone(get_location());
}


rcstring
expression_error::lisp_representation(void)
    const
{
    return "error";
}


expression::pointer
expression_error::clone(const location &locn)
    const
{
    return create(locn);
}


// vim: set ts=8 sw=4 et :
