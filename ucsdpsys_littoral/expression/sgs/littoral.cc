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

#include <ucsdpsys_littoral/expression/sgs/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_sgs_littoral::~expression_sgs_littoral()
{
}


expression_sgs_littoral::expression_sgs_littoral(
    const expression::pointer &a_value,
    translator_littoral *cntxt
) :
    expression_sgs(a_value),
    littoral(*cntxt)
{
}


expression_sgs_littoral::pointer
expression_sgs_littoral::create(const expression::pointer &a_value,
    translator_littoral *cntxt)
{
    return pointer(new expression_sgs_littoral(a_value, cntxt));
}


void
expression_sgs_littoral::pre_order_traversal(int)
    const
{
    littoral.print_token("set(");
}


void
expression_sgs_littoral::post_order_traversal(int)
    const
{
    littoral.print_token(")");
}


expression::pointer
expression_sgs_littoral::optimize(void)
    const
{
    return create(get_value(), &littoral);
}


expression::pointer
expression_sgs_littoral::clone(const location &locn)
    const
{
    return create(get_value()->clone(locn), &littoral);
}


// vim: set ts=8 sw=4 et :
