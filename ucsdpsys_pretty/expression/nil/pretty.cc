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

#include <ucsdpsys_pretty/expression/nil/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_nil_pretty::~expression_nil_pretty()
{
}


expression_nil_pretty::expression_nil_pretty(
    const location & locn,
    translator_pretty *a_pretty
) :
    expression_nil(locn),
    pretty(*a_pretty)
{
}


expression_nil_pretty::pointer
expression_nil_pretty::create(const location &locn, translator_pretty *a_pretty)
{
    return pointer(new expression_nil_pretty(locn, a_pretty));
}


void
expression_nil_pretty::traversal(int)
    const
{
    pretty.print_token("nil");
}


expression::pointer
expression_nil_pretty::optimize(void)
    const
{
    return create(get_location(), &pretty);
}


expression::pointer
expression_nil_pretty::clone(const location &locn)
    const
{
    return create(locn, &pretty);
}


// vim: set ts=8 sw=4 et :
