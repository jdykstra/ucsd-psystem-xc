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

#include <lib/expression/modulo.h>
#include <lib/type.h>


expression_modulo::~expression_modulo()
{
}


expression_modulo::expression_modulo(const expression::pointer &a_lhs,
        const expression::pointer &a_rhs) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs)
{
}


expression_modulo::expression_modulo(const expression_modulo &arg) :
    expression(arg),
    lhs(arg.lhs),
    rhs(arg.rhs)
{
}


void
expression_modulo::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    lhs->traversal(get_precedence());
    infix_order_traversal(pprec);
    rhs->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_modulo::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_modulo::infix_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_modulo::post_order_traversal(int)
    const
{
    // Do nothing.
}


int
expression_modulo::get_precedence()
    const
{
    return prec_mul;
}


bool
expression_modulo::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_modulo::has_side_effect()
    const
{
    return (lhs->has_side_effect() + rhs->has_side_effect());
}


type::pointer
expression_modulo::get_type()
    const
{
    // Both are supposed to be of the same type.
    if (rhs->is_error())
        return rhs->get_type();
    return lhs->get_type();
}


bool
expression_modulo::is_constant()
    const
{
    return (lhs->is_constant() && rhs->is_constant());
}


rcstring
expression_modulo::lisp_representation(void)
    const
{
    return
        (
            "(modulo {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            lhs->lisp_representation()
        +
            " "
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
