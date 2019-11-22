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

#include <ucsdpsys_littoral/expression/ternary/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_ternary_littoral::~expression_ternary_littoral()
{
}


expression_ternary_littoral::expression_ternary_littoral(
    const expression::pointer &a_cond,
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_littoral *a_littoral
) :
    expression_ternary(a_cond, a_lhs, a_rhs, a_littoral),
    littoral(*a_littoral)
{
}


expression_ternary_littoral::pointer
expression_ternary_littoral::create(const expression::pointer &a_cond,
    const expression::pointer &a_lhs, const expression::pointer &a_rhs,
    translator_littoral *a_littoral)
{
    return
        pointer
        (
            new expression_ternary_littoral(a_cond, a_lhs, a_rhs, a_littoral)
        );
}


void
expression_ternary_littoral::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token("(");
}


void
expression_ternary_littoral::quest_order_traversal(int)
    const
{
    littoral.print_space();
    littoral.print_token("?");
    littoral.print_space();
}


void
expression_ternary_littoral::colon_order_traversal(int)
    const
{
    littoral.print_space();
    littoral.print_token(":");
    littoral.print_space();
}


void
expression_ternary_littoral::post_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token(")");
}


expression::pointer
expression_ternary_littoral::optimize(void)
    const
{
    return create(get_cond(), get_lhs(), get_rhs(), &littoral);
}


expression::pointer
expression_ternary_littoral::clone(const location &locn)
    const
{
    return
        create
        (
            get_cond()->clone(locn),
            get_lhs()->clone(locn),
            get_rhs()->clone(locn),
            &littoral
        );
}


bool
expression_ternary_littoral::is_constant(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
