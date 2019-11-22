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

#include <ucsdpsys_pretty/expression/logical_or/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_logical_or_pretty::~expression_logical_or_pretty()
{
}


expression_logical_or_pretty::expression_logical_or_pretty(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_pretty *cntxt
) :
    expression_logical_or(a_lhs, a_rhs, cntxt),
    pretty(*cntxt)
{
}


expression_logical_or_pretty::pointer
expression_logical_or_pretty::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_pretty *cntxt)
{
    return pointer(new expression_logical_or_pretty(a_lhs, a_rhs, cntxt));
}


void
expression_logical_or_pretty::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        pretty.print_token("(");
}


void
expression_logical_or_pretty::infix_order_traversal(int)
    const
{
    pretty.print_space();
    pretty.print_token("or");
    pretty.print_space();
}


void
expression_logical_or_pretty::post_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        pretty.print_token(")");
}


expression::pointer
expression_logical_or_pretty::optimize(void)
    const
{
    return create(get_lhs(), get_rhs(), &pretty);
}


expression::pointer
expression_logical_or_pretty::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &pretty);
}


// vim: set ts=8 sw=4 et :
