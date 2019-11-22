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

#include <ucsdpsys_pretty/statement/until/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_until_pretty::~statement_until_pretty()
{
}


statement_until_pretty::statement_until_pretty(
    const statement_list &a_body,
    const expression::pointer &a_condition,
    translator_pretty *cntxt
) :
    statement_until(a_body, a_condition, cntxt),
    pretty(*cntxt)
{
}


statement_until_pretty::pointer
statement_until_pretty::create(const statement_list &a_body,
    const expression::pointer &a_condition, translator_pretty *cntxt)
{
    return pointer(new statement_until_pretty(a_body, a_condition, cntxt));
}


void
statement_until_pretty::pre_order_traversal()
    const
{
    pretty.print_token("repeat");
    pretty.print_eoln();
    pretty.indent_more();
}


void
statement_until_pretty::until_order_traversal()
    const
{
    pretty.print_eoln();
    pretty.indent_less();
    pretty.print_token("until");
    pretty.print_space();
}


void
statement_until_pretty::post_order_traversal()
    const
{
}


// vim: set ts=8 sw=4 et :
