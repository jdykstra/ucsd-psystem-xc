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

#include <ucsdpsys_littoral/expression/real_from_integer/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_real_from_integer_littoral::~expression_real_from_integer_littoral()
{
}


expression_real_from_integer_littoral::expression_real_from_integer_littoral(
    const expression::pointer &a_arg,
    translator_littoral *cntxt
) :
    expression_real_from_integer(a_arg),
    littoral(*cntxt)
{
}


expression_real_from_integer_littoral::pointer
expression_real_from_integer_littoral::create(const expression::pointer &a_arg,
    translator_littoral *cntxt)
{
    return pointer(new expression_real_from_integer_littoral(a_arg, cntxt));
}


void
expression_real_from_integer_littoral::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token("(");
}


void
expression_real_from_integer_littoral::post_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token(")");
}


void
expression_real_from_integer_littoral::logical_traversal(int pprec,
    label::pointer &, label::pointer &, bool) const
{
    assert(0);
    traversal(pprec);
}


expression::pointer
expression_real_from_integer_littoral::optimize(void)
    const
{
    return create(get_operand(), &littoral);
}


expression::pointer
expression_real_from_integer_littoral::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &littoral);
}


// vim: set ts=8 sw=4 et :
