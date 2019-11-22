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

#include <ucsdpsys_littoral/expression/paoc_from_string/littoral.h>


expression_paoc_from_string_littoral::~expression_paoc_from_string_littoral()
{
}


expression_paoc_from_string_littoral::expression_paoc_from_string_littoral(
    const expression::pointer &a_rhs,
    const type::pointer &a_tp
) :
    expression_paoc_from_string(a_rhs, a_tp)
{
}


expression_paoc_from_string_littoral::pointer
expression_paoc_from_string_littoral::create(const expression::pointer &a_rhs,
    const type::pointer &a_tp)
{
    return pointer(new expression_paoc_from_string_littoral(a_rhs, a_tp));
}


void
expression_paoc_from_string_littoral::traversal(int pprec)
    const
{
    get_rhs()->traversal(pprec);
}


expression::pointer
expression_paoc_from_string_littoral::optimize()
    const
{
    return create(get_rhs(), get_tp());
}


expression::pointer
expression_paoc_from_string_littoral::clone(const location &locn)
    const
{
    return create(get_rhs()->clone(locn), get_tp());
}


bool
expression_paoc_from_string_littoral::is_constant(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
