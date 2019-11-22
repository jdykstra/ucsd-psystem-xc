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

#include <ucsdpsys_littoral/expression/negate/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_negate_littoral::~expression_negate_littoral()
{
}


expression_negate_littoral::expression_negate_littoral(
   const expression_negate_littoral &arg
) :
    expression_negate(arg),
    littoral(arg.littoral)
{
}


expression_negate_littoral::expression_negate_littoral(
   const expression::pointer &arg,
   translator_littoral *cntxt
) :
    expression_negate(arg),
    littoral(*cntxt)
{
}


expression_negate_littoral::pointer
expression_negate_littoral::create(const expression::pointer &arg,
   translator_littoral *cntxt)
{
    return pointer(new expression_negate_littoral(arg, cntxt));
}


void
expression_negate_littoral::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token("(");
    littoral.print_token("-");
}


void
expression_negate_littoral::post_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token(")");
}


expression::pointer
expression_negate_littoral::optimize(void)
    const
{
    return create(get_operand(), &littoral);
}


expression::pointer
expression_negate_littoral::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &littoral);
}


long
expression_negate_littoral::get_integer_value(void)
    const
{
    long n = get_operand()->get_integer_value();
    return -n;
}


// vim: set ts=8 sw=4 et :
