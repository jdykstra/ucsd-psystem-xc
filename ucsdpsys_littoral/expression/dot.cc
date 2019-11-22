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

#include <ucsdpsys_littoral/expression/array_index.h>
#include <ucsdpsys_littoral/expression/assignment/littoral.h>
#include <ucsdpsys_littoral/expression/dot.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_dot::~expression_dot()
{
}


expression_dot::expression_dot(
    const expression::pointer &a_lhs,
    const symbol::pointer &a_rhs,
    translator_littoral *a_littoral
) :
    expression(a_lhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs),
    littoral(*a_littoral)
{
}


expression_dot::pointer
expression_dot::create(const expression::pointer &a_lhs,
    const symbol::pointer &a_rhs, translator_littoral *a_littoral)
{
    return pointer(new expression_dot(a_lhs, a_rhs, a_littoral));
}


void
expression_dot::traversal(int)
    const
{
    expression::pointer lhs2 = lhs->strip_indirection();
    if (lhs2)
    {
        lhs2->traversal(get_precedence());
        littoral.print_token("->");
    }
    else
    {
        lhs->traversal(get_precedence());
        littoral.print_token(".");
    }
    littoral.print_token(rhs->get_name().downcase());
}


void
expression_dot::logical_traversal(int pprec, label::pointer &, label::pointer &,
    bool) const
{
    traversal(pprec);
}


int
expression_dot::get_precedence(void)
    const
{
    return prec_id;
}


type::pointer
expression_dot::get_type(void)
    const
{
    return rhs->get_type();
}


bool
expression_dot::is_lvalue(void)
    const
{
    return true;
}


expression::side_effect_t
expression_dot::has_side_effect(void) const
{
    return side_effect_no + lhs->has_side_effect();
}


expression::pointer
expression_dot::optimize(void)
    const
{
    return create(lhs, rhs, &littoral);
}


rcstring
expression_dot::lisp_representation(void)
    const
{
    return
        (
            "(dot "
        +
            lhs->lisp_representation()
        +
            " "
        +
            rhs->get_name()
        +
            ")"
        );
}


expression::pointer
expression_dot::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs, &littoral);
}


expression::pointer
expression_dot::clone(void)
    const
{
    return create(lhs, rhs, &littoral);
}


expression::pointer
expression_dot::assignment_expression_factory(const expression::pointer &rhs2)
    const
{
    return expression_assignment_littoral::create(clone(), rhs2, &littoral);
}



expression::pointer
expression_dot::dot_expression_factory(const location &,
    const symbol::pointer &rhs2) const
{
    return expression_dot::create(clone(), rhs2, &littoral);
}


bool
expression_dot::is_constant(void)
    const
{
    return false;
}


expression::pointer
expression_dot::array_index_expression_factory(
    const expression::pointer &rhs2) const
{
    return expression_array_index::create(clone(), rhs2, &littoral);
}


bool
expression_dot::is_simple_with_stmt(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
