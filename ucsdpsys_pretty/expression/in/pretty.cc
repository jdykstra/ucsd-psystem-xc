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

#include <ucsdpsys_pretty/expression/in/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_in_pretty::~expression_in_pretty()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in_pretty::expression_in_pretty(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_pretty *a_pretty
) :
    expression_in(a_lhs, a_rhs),
    pretty(*a_pretty)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in_pretty::expression_in_pretty(const expression_in_pretty &arg) :
    expression_in(arg),
    pretty(arg.pretty)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in_pretty::pointer
expression_in_pretty::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_pretty *a_pretty)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_in_pretty(a_lhs, a_rhs, a_pretty));
}


void
expression_in_pretty::pre_order_traversal(int pprec)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (need_parens(pprec))
        pretty.print_token("(");
}


void
expression_in_pretty::infix_order_traversal(int)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    pretty.print_space();
    pretty.print_token("in");
    pretty.print_space();
}


void
expression_in_pretty::post_order_traversal(int pprec)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (need_parens(pprec))
        pretty.print_token(")");
}


void
expression_in_pretty::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    traversal(pprec);
}


expression::pointer
expression_in_pretty::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_lhs(), get_rhs(), &pretty);
}


expression::pointer
expression_in_pretty::clone(const location &locn)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &pretty);
}


// vim: set ts=8 sw=4 et :
