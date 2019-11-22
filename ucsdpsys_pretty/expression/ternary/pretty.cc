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

#include <ucsdpsys_pretty/expression/ternary/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_ternary_pretty::~expression_ternary_pretty()
{
}


expression_ternary_pretty::expression_ternary_pretty(
    const expression::pointer &a_cond,
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_pretty *a_pretty
) :
    expression_ternary(a_cond, a_lhs, a_rhs, a_pretty),
    pretty(*a_pretty)
{
}


expression_ternary_pretty::pointer
expression_ternary_pretty::create(const expression::pointer &a_cond,
    const expression::pointer &a_lhs, const expression::pointer &a_rhs,
    translator_pretty *a_pretty)
{
    return
        pointer(new expression_ternary_pretty(a_cond, a_lhs, a_rhs, a_pretty));
}


void
expression_ternary_pretty::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        pretty.print_token("(");
}


void
expression_ternary_pretty::quest_order_traversal(int)
    const
{
    pretty.print_space();
    pretty.print_token("?");
    pretty.print_space();
}


void
expression_ternary_pretty::colon_order_traversal(int)
    const
{
    pretty.print_space();
    pretty.print_token(":");
    pretty.print_space();
}


void
expression_ternary_pretty::post_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        pretty.print_token(")");
}


expression::pointer
expression_ternary_pretty::optimize(void)
    const
{
    return create(get_cond(), get_lhs(), get_rhs(), &pretty);
}


expression::pointer
expression_ternary_pretty::clone(const location &locn)
    const
{
    return
        create
        (
            get_cond()->clone(locn),
            get_lhs()->clone(locn),
            get_rhs()->clone(locn),
            &pretty
        );
}


// vim: set ts=8 sw=4 et :
