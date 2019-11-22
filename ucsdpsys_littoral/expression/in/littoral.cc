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

#include <lib/debug.h>

#include <ucsdpsys_littoral/expression/in/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_in_littoral::~expression_in_littoral()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in_littoral::expression_in_littoral(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_littoral *a_littoral
) :
    expression_in(a_lhs, a_rhs),
    littoral(*a_littoral)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in_littoral::expression_in_littoral(
    const expression_in_littoral &arg
) :
    expression_in(arg),
    littoral(arg.littoral)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in_littoral::pointer
expression_in_littoral::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_littoral *a_littoral)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_in_littoral(a_lhs, a_rhs, a_littoral));
}


void
expression_in_littoral::pre_order_traversal(int pprec)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (need_parens(pprec))
        littoral.print_token("(");
}


void
expression_in_littoral::infix_order_traversal(int)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    littoral.print_space();
    littoral.print_token("in");
    littoral.print_space();
}


void
expression_in_littoral::post_order_traversal(int pprec)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (need_parens(pprec))
        littoral.print_token(")");
}


void
expression_in_littoral::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    traversal(pprec);
}


expression::pointer
expression_in_littoral::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_lhs(), get_rhs(), &littoral);
}


expression::pointer
expression_in_littoral::clone(const location &locn)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &littoral);
}


// vim: set ts=8 sw=4 et :
