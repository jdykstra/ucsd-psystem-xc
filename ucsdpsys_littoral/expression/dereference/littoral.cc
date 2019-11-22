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

#include <ucsdpsys_littoral/expression/array_index.h>
#include <ucsdpsys_littoral/expression/assignment/littoral.h>
#include <ucsdpsys_littoral/expression/dereference/littoral.h>
#include <ucsdpsys_littoral/expression/dot.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_dereference_littoral::~expression_dereference_littoral()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_dereference_littoral::expression_dereference_littoral(
    const expression::pointer &arg,
    translator_littoral *cntxt
) :
    expression_dereference(arg),
    littoral(*cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_dereference_littoral::pointer
expression_dereference_littoral::create(const expression::pointer &arg,
    translator_littoral *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_dereference_littoral(arg, cntxt));
}


void
expression_dereference_littoral::pre_order_traversal(int pprec)
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
            littoral.print_token("(");
        littoral.print_token("*");
    }
}


void
expression_dereference_littoral::post_order_traversal(int pprec)
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
            littoral.print_token(")");
    }
}


expression::pointer
expression_dereference_littoral::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_operand(), &littoral);
}


expression::pointer
expression_dereference_littoral::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &littoral);
}


expression::pointer
expression_dereference_littoral::clone(void)
    const
{
    return clone(get_location());
}


expression::pointer
expression_dereference_littoral::dot_expression_factory(const location &,
    const symbol::pointer &rhs) const
{
    return expression_dot::create(clone(), rhs, &littoral);
}


expression::pointer
expression_dereference_littoral::array_index_expression_factory(
    const expression::pointer &rhs2) const
{
    return expression_array_index::create(clone(), rhs2, &littoral);
}



expression::pointer
expression_dereference_littoral::assignment_expression_factory(
    const expression::pointer &rhs2) const
{
    return expression_assignment_littoral::create(clone(), rhs2, &littoral);
}


bool
expression_dereference_littoral::is_simple_with_stmt(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
