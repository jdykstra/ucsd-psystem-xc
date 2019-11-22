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

#include <lib/debug.h>

#include <ucsdpsys_pretty/expression/dereference/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_dereference_pretty::~expression_dereference_pretty()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_dereference_pretty::expression_dereference_pretty(
    const expression::pointer &arg,
    translator_pretty *cntxt
) :
    expression_dereference(arg),
    pretty(*cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_dereference_pretty::pointer
expression_dereference_pretty::create(const expression::pointer &arg,
    translator_pretty *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_dereference_pretty(arg, cntxt));
}


void
expression_dereference_pretty::pre_order_traversal(int pprec)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    //
    // We don't need the carat for reference (implict pointers), we only
    // need them for explicit pointers.
    //
    if (is_explicit())
    {
        if (need_parens(pprec))
            pretty.print_token("(");
    }
}


void
expression_dereference_pretty::post_order_traversal(int pprec)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    //
    // We don't need the carat for reference (implict pointers), we only
    // need them for explicit pointers.
    //
    if (is_explicit())
    {
        if (need_parens(pprec))
            pretty.print_token(")");
        pretty.print_token("^");
    }
}


expression::pointer
expression_dereference_pretty::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_operand(), &pretty);
}


expression::pointer
expression_dereference_pretty::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &pretty);
}


// vim: set ts=8 sw=4 et :
