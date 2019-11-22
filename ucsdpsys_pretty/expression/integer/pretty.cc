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

#include <ucsdpsys_pretty/expression/integer/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_integer_pretty::~expression_integer_pretty()
{
}


expression_integer_pretty::expression_integer_pretty(
    const location &locn,
    const rcstring &arg1,
    translator_pretty *arg2
) :
    expression_integer(arg1, locn),
    pretty(*arg2),
    original(arg1)
{
}


expression_integer_pretty::pointer
expression_integer_pretty::create(const location &locn, const rcstring &arg1,
    translator_pretty *arg2)
{
    return pointer(new expression_integer_pretty(locn, arg1, arg2));
}


void
expression_integer_pretty::traversal(int)
    const
{
    pretty.print_token(original);
}


void
expression_integer_pretty::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


expression::pointer
expression_integer_pretty::optimize(void)
    const
{
    return create(get_location(), original, &pretty);
}


expression::pointer
expression_integer_pretty::clone(const location &locn)
    const
{
    return create(locn, original, &pretty);
}


// vim: set ts=8 sw=4 et :
