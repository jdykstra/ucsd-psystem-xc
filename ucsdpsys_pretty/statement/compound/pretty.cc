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

#include <ucsdpsys_pretty/statement/compound/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_compound_pretty::~statement_compound_pretty()
{
}


statement_compound_pretty::statement_compound_pretty(
    const statement_list &sl,
    translator_pretty *cntxt
) :
    statement_compound(sl),
    pretty(*cntxt)
{
}


statement_compound_pretty::pointer
statement_compound_pretty::create(const statement_list &sl,
    translator_pretty *cntxt)
{
    return pointer(new statement_compound_pretty(sl, cntxt));
}


void
statement_compound_pretty::traversal_begin()
    const
{
    pretty.print_token("begin");
    pretty.print_eoln();
    pretty.indent_more();
}


void
statement_compound_pretty::traversal_semicolon()
    const
{
    pretty.print_token(";");
    pretty.print_eoln();
}


void
statement_compound_pretty::traversal_end()
    const
{
    if (!empty())
        pretty.print_eoln();
    pretty.indent_less();
    pretty.print_token("end");
}


// vim: set ts=8 sw=4 et :
