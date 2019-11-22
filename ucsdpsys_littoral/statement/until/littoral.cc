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

#include <ucsdpsys_littoral/statement/until/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_until_littoral::~statement_until_littoral()
{
}


statement_until_littoral::statement_until_littoral(
    const statement_list &a_body,
    const expression::pointer &a_condition,
    translator_littoral *cntxt
) :
    statement_until(a_body, a_condition, cntxt),
    littoral(*cntxt)
{
}


statement_until_littoral::pointer
statement_until_littoral::create(const statement_list &a_body,
    const expression::pointer &a_condition, translator_littoral *cntxt)
{
    return pointer(new statement_until_littoral(a_body, a_condition, cntxt));
}


void
statement_until_littoral::pre_order_traversal()
    const
{
    littoral.print_token("for (;;)");
    littoral.print_eoln();
    littoral.print_token("{");
    littoral.print_eoln();
    littoral.indent_more();
}


void
statement_until_littoral::until_order_traversal()
    const
{
    littoral.print_token("if");
    littoral.print_space();
    littoral.print_token("(");
}


void
statement_until_littoral::post_order_traversal()
    const
{
    littoral.print_token(")");
    littoral.print_eoln();

    littoral.indent_more();
    littoral.print_token("break;");
    littoral.print_eoln();
    littoral.indent_less();

    littoral.indent_less();
    littoral.print_token("}");
    littoral.print_eoln();
}


// vim: set ts=8 sw=4 et :
