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

#include <ucsdpsys_littoral/expression/nil/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_nil_littoral::~expression_nil_littoral()
{
}


expression_nil_littoral::expression_nil_littoral(
    const location & locn,
    translator_littoral *a_littoral
) :
    expression_nil(locn),
    littoral(*a_littoral)
{
}


expression_nil_littoral::pointer
expression_nil_littoral::create(const location &locn,
    translator_littoral *a_littoral)
{
    return pointer(new expression_nil_littoral(locn, a_littoral));
}


void
expression_nil_littoral::traversal(int)
    const
{
    littoral.print_token("NULL");
}


expression::pointer
expression_nil_littoral::optimize(void)
    const
{
    return create(get_location(), &littoral);
}


expression::pointer
expression_nil_littoral::clone(const location &locn)
    const
{
    return create(locn, &littoral);
}


// vim: set ts=8 sw=4 et :
