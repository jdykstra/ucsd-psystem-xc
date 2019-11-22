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

#include <lib/type/array.h>
#include <lib/type/char.h>
#include <lib/type/string.h>

#include <ucsdpsys_littoral/expression/array_index.h>
#include <ucsdpsys_littoral/expression/assignment/littoral.h>
#include <ucsdpsys_littoral/expression/dot.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_array_index::~expression_array_index()
{
}


expression_array_index::expression_array_index(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_littoral *cntxt
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    littoral(*cntxt),
    lhs(a_lhs),
    rhs(a_rhs)
{
}


expression_array_index::pointer
expression_array_index::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_littoral *cntxt)
{
    return pointer(new expression_array_index(a_lhs, a_rhs, cntxt));
}


void
expression_array_index::traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token("(");
    lhs->traversal(get_precedence());
    littoral.print_token("[");
    rhs->traversal(get_precedence());
    littoral.print_token("]");
    if (need_parens(pprec))
        littoral.print_token(")");
}


void
expression_array_index::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


int
expression_array_index::get_precedence()
    const
{
    return prec_paren;
}


type::pointer
expression_array_index::get_type()
    const
{
    if (type_array::is_a(lhs->get_type()))
        return lhs->get_type()->get_subtype();
    if (type_string::is_a(lhs))
        return type_char::create();
    return lhs->get_type();
}


bool
expression_array_index::is_lvalue()
    const
{
    return lhs->is_lvalue();
}


expression::side_effect_t
expression_array_index::has_side_effect()
    const
{
    return (lhs->has_side_effect() + rhs->has_side_effect());
}


expression::pointer
expression_array_index::optimize(void)
    const
{
    return create(lhs, rhs, &littoral);
}


expression::pointer
expression_array_index::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn), &littoral);
}


expression::pointer
expression_array_index::clone(void)
    const
{
    return clone(get_location());
}


rcstring
expression_array_index::lisp_representation(void)
    const
{
    return
        (
            "(array-index "
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


expression::pointer
expression_array_index::assignment_expression_factory(
    const expression::pointer &rhs2) const
{
    return expression_assignment_littoral::create(clone(), rhs2, &littoral);
}


expression::pointer
expression_array_index::dot_expression_factory(const location &,
    const symbol::pointer &sp) const
{
    return expression_dot::create(clone(), sp, &littoral);
}


bool
expression_array_index::is_constant(void)
    const
{
    return false;
}


bool
expression_array_index::is_simple_with_stmt(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
