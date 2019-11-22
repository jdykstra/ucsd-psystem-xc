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

#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/expression/name.h>
#include <lib/pascal/lex.h>
#include <lib/symbol/expression/constant.h>
#include <lib/symbol/function.h>
#include <lib/symbol/variable.h>
#include <lib/type/error.h>


expression_name::~expression_name()
{
}


expression_name::expression_name(
    const location &locn,
    const symbol::pointer &arg
) :
    expression(locn),
    sp(arg)
{
}


expression_name::expression_name(const expression_name &arg) :
    expression(arg),
    sp(arg.sp)
{
}


int
expression_name::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_name::get_type()
    const
{
    return get_sp()->get_type();
}


bool
expression_name::is_lvalue()
    const
{
    symbol_variable::pointer svp =
        boost::dynamic_pointer_cast<symbol_variable>(get_sp());
    return (svp != 0);
}


expression::side_effect_t
expression_name::has_side_effect()
    const
{
    return side_effect_no;
}


expression::pointer
expression_name::function_call_expression_factory(const expression_list &rhs)
    const
{
    if (type_error::is_a(get_type()))
        return expression_error::create(get_location());

    //
    // Make sure this is a valid function expression.
    //
    symbol_function::pointer fsp =
        boost::dynamic_pointer_cast<symbol_function>(get_sp());
    if (!fsp)
    {
        // If it isn't valid, use the parent class's generic error.
        return expression::function_call_expression_factory(rhs);
    }

    //
    // Have the function object generate a function call to itself.
    //
    return fsp->function_call_expression_factory(rhs);
}


bool
expression_name::is_constant(void)
    const
{
    bool result = !!boost::dynamic_pointer_cast<symbol_expression_constant>(sp);
    DEBUG(1, "%d", result);
    return result;
}


rcstring
expression_name::lisp_representation(void)
    const
{
    return
        (
            "(name {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            sp->get_name().quote_c()
        +
            ")"
        );
}


rcstring
expression_name::get_description(void)
    const
{
    return
        (
            get_type()->get_name()
        +
            " "
        +
            sp->get_kind()
        +
            " "
        +
            sp->get_name().quote_c()
        );
}


// vim: set ts=8 sw=4 et :
