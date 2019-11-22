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

#include <ucsdpsys_pretty/statement/infinite_loop/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_infinite_loop_pretty::~statement_infinite_loop_pretty()
{
}


statement_infinite_loop_pretty::statement_infinite_loop_pretty(
    const statement::pointer &a_body,
    translator_pretty *cntxt
) :
    statement_infinite_loop(a_body, cntxt),
    pretty(*cntxt)
{
}


statement_infinite_loop_pretty::pointer
statement_infinite_loop_pretty::create(const statement::pointer &a_body,
    translator_pretty *cntxt)
{
    return pointer(new statement_infinite_loop_pretty(a_body, cntxt));
}


void
statement_infinite_loop_pretty::pre_order_traversal()
    const
{
    pretty.print_token("while");
    pretty.print_space();
    pretty.print_token("true");
    pretty.print_space();
    pretty.print_token("do");
    pretty.print_eoln();
    pretty.indent_more();
}


void
statement_infinite_loop_pretty::post_order_traversal()
    const
{
    pretty.indent_less();
}


// vim: set ts=8 sw=4 et :
