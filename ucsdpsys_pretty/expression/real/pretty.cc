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

#include <ucsdpsys_pretty/expression/real/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_real_pretty::~expression_real_pretty()
{
}


expression_real_pretty::expression_real_pretty(
    const location &locn,
    const rcstring &arg1,
    translator_pretty *arg2
) :
    expression_real(locn, arg1),
    pretty(*arg2),
    original(arg1)
{
}


expression_real_pretty::pointer
expression_real_pretty::create(const location &locn, const rcstring &a_value,
    translator_pretty *a_pretty)
{
    return pointer(new expression_real_pretty(locn, a_value, a_pretty));
}


void
expression_real_pretty::traversal(int)
    const
{
    pretty.print_token(original);
}


void
expression_real_pretty::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


expression::pointer
expression_real_pretty::optimize(void)
    const
{
    return create(get_location(), original, &pretty);
}


expression::pointer
expression_real_pretty::clone(const location &locn)
    const
{
    return create(locn, original, &pretty);
}


// vim: set ts=8 sw=4 et :
