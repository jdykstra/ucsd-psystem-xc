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

#include <ucsdpsys_littoral/expression/logical_and/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_logical_and_littoral::~expression_logical_and_littoral()
{
}


expression_logical_and_littoral::expression_logical_and_littoral(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_littoral *cntxt
) :
    expression_logical_and(a_lhs, a_rhs, cntxt),
    littoral(*cntxt)
{
}


expression_logical_and_littoral::pointer
expression_logical_and_littoral::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_littoral *cntxt)
{
    return pointer(new expression_logical_and_littoral(a_lhs, a_rhs, cntxt));
}


void
expression_logical_and_littoral::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token("(");
}


void
expression_logical_and_littoral::infix_order_traversal(int)
    const
{
    littoral.print_space();
    littoral.print_token("&&");
    littoral.print_space();
}


void
expression_logical_and_littoral::post_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token(")");
}


expression::pointer
expression_logical_and_littoral::optimize()
    const
{
    return create(get_lhs(), get_rhs(), &littoral);
}


expression::pointer
expression_logical_and_littoral::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &littoral);
}


// vim: set ts=8 sw=4 et :
