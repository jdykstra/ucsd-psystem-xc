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

#include <ucsdpsys_littoral/statement/if/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_if_littoral::~statement_if_littoral()
{
}


statement_if_littoral::statement_if_littoral(
    const expression::pointer &a_cond,
    const statement::pointer &a_thn,
    const statement::pointer &a_els,
    translator_littoral *cntxt
) :
    statement_if(a_cond, a_thn, a_els, cntxt),
    littoral(*cntxt)
{
}


statement_if_littoral::pointer
statement_if_littoral::create(const expression::pointer &a_cond,
    const statement::pointer &a_thn, const statement::pointer &a_els,
    translator_littoral *cntxt)
{
    return pointer(new statement_if_littoral(a_cond, a_thn, a_els, cntxt));
}


void
statement_if_littoral::pre_order_traversal()
    const
{
    littoral.print_token("if");
    littoral.print_space();
    littoral.print_token("(");
}


void
statement_if_littoral::then_order_traversal()
    const
{
    littoral.print_token(")");
    littoral.print_eoln();
    if (!get_then_clause()->is_compound())
        littoral.indent_more();
}


void
statement_if_littoral::else_order_traversal()
    const
{
    if (!get_then_clause()->is_compound())
        littoral.indent_less();
    littoral.print_token("else");
    littoral.print_eoln();
    if (!get_else_clause()->is_compound())
        littoral.indent_more();
}


void
statement_if_littoral::post_order_traversal()
    const
{
    statement::pointer sp = get_else_clause();
    if (!sp)
        sp = get_then_clause();
    if (!sp->is_compound())
        littoral.indent_less();
}


// vim: set ts=8 sw=4 et :
