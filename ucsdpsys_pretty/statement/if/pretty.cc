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

#include <ucsdpsys_pretty/statement/if/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_if_pretty::~statement_if_pretty()
{
}


statement_if_pretty::statement_if_pretty(
    const expression::pointer &a_cond,
    const statement::pointer &a_thn,
    const statement::pointer &a_els,
    translator_pretty *cntxt
) :
    statement_if(a_cond, a_thn, a_els, cntxt),
    pretty(*cntxt)
{
}


statement_if_pretty::pointer
statement_if_pretty::create(const expression::pointer &a_cond,
    const statement::pointer &a_thn, const statement::pointer &a_els,
    translator_pretty *cntxt)
{
    return pointer(new statement_if_pretty(a_cond, a_thn, a_els, cntxt));
}


void
statement_if_pretty::pre_order_traversal()
    const
{
    pretty.print_token("if");
    pretty.print_space();
}


void
statement_if_pretty::then_order_traversal()
    const
{
    pretty.print_space();
    pretty.print_token("then");
    pretty.print_eoln();
    pretty.indent_more();
}


void
statement_if_pretty::else_order_traversal()
    const
{
    pretty.print_eoln();
    pretty.indent_less();
    pretty.print_token("else");
    pretty.print_eoln();
    pretty.indent_more();
}


void
statement_if_pretty::post_order_traversal()
    const
{
    // This will "take" after the next eoln.
    pretty.indent_less();
}


// vim: set ts=8 sw=4 et :
