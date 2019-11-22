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

#include <ucsdpsys_littoral/expression/set2set/littoral.h>


expression_set2set_littoral::~expression_set2set_littoral()
{
}


expression_set2set_littoral::expression_set2set_littoral(
    const type::pointer &a_to,
    const expression::pointer &a_from,
    translator_littoral *cntxt
) :
    expression_set2set(a_to, a_from),
    littoral(*cntxt)
{
}


expression_set2set_littoral::pointer
expression_set2set_littoral::create(const type::pointer &a_to,
    const expression::pointer &a_from, translator_littoral *cntxt)
{
    return pointer(new expression_set2set_littoral(a_to, a_from, cntxt));
}


expression::pointer
expression_set2set_littoral::optimize(void)
    const
{
    return create(get_type(), get_from(), &littoral);
}


expression::pointer
expression_set2set_littoral::clone(const location &locn)
    const
{
    return create(get_type(), get_from()->clone(locn), &littoral);
}


// vim: set ts=8 sw=4 et :
