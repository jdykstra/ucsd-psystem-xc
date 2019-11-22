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

#include <ucsdpsys_littoral/statement/while/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_while_littoral::~statement_while_littoral()
{
}


statement_while_littoral::statement_while_littoral(
    const expression::pointer &a_condition,
    const statement::pointer &a_body,
    translator_littoral *cntxt
) :
    statement_while(a_condition, a_body, cntxt),
    littoral(*cntxt)
{
}


statement_while_littoral::pointer
statement_while_littoral::create(const expression::pointer &a_condition,
    const statement::pointer &a_body, translator_littoral *cntxt)
{
    return pointer(new statement_while_littoral(a_condition, a_body, cntxt));
}


void
statement_while_littoral::pre_order_traversal()
    const
{
    littoral.print_token("while");
    littoral.print_space();
    littoral.print_token("(");
}


void
statement_while_littoral::do_order_traversal()
    const
{
    littoral.print_token(")");
    littoral.print_eoln();
    if (!get_body()->is_compound())
        littoral.indent_more();
}


void
statement_while_littoral::post_order_traversal()
    const
{
    if (!get_body()->is_compound())
        littoral.indent_less();
}


// vim: set ts=8 sw=4 et :
