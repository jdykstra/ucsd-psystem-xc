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
#include <ucsdpsys_pretty/expression/array_index.h>
#include <ucsdpsys_pretty/expression/assignment/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_array_index::~expression_array_index()
{
}


expression_array_index::expression_array_index(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_pretty *cntxt
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    pretty(*cntxt),
    lhs(a_lhs),
    rhs(a_rhs)
{
}


expression_array_index::pointer
expression_array_index::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_pretty *cntxt)
{
    return pointer(new expression_array_index(a_lhs, a_rhs, cntxt));
}


void
expression_array_index::traversal(int pprec)
    const
{
    if (need_parens(pprec))
        pretty.print_token("(");
    lhs->traversal(get_precedence());
    pretty.print_token("[");
    rhs->traversal(get_precedence());
    pretty.print_token("]");
    if (need_parens(pprec))
        pretty.print_token(")");
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
    assert(type_array::is_a(lhs->get_type()));
    return lhs->get_type()->get_subtype();
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
    return create(lhs, rhs, &pretty);
}


expression::pointer
expression_array_index::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn), &pretty);
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
    return expression_assignment_pretty::create(clone(), rhs2, &pretty);
}


// vim: set ts=8 sw=4 et :
