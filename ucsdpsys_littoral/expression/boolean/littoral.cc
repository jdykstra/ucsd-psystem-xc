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

#include <ucsdpsys_littoral/expression/boolean/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_boolean_littoral::~expression_boolean_littoral()
{
}


expression_boolean_littoral::expression_boolean_littoral(
    const location &a_locn,
    bool a_value,
    translator_littoral *cntxt
) :
    expression_boolean(a_locn, a_value),
    littoral(*cntxt)
{
}


expression_boolean_littoral::pointer
expression_boolean_littoral::create(const location &a_locn, bool a_value,
    translator_littoral *cntxt)
{
    return pointer(new expression_boolean_littoral(a_locn, a_value, cntxt));
}


void
expression_boolean_littoral::traversal(int)
    const
{
    littoral.print_token(get_value() ? "true" : "false");
}


void
expression_boolean_littoral::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


expression::pointer
expression_boolean_littoral::optimize(void)
    const
{
    return create(get_location(), get_value(), &littoral);
}


expression::pointer
expression_boolean_littoral::clone(const location &locn)
    const
{
    return create(locn, get_value(), &littoral);
}


// vim: set ts=8 sw=4 et :
