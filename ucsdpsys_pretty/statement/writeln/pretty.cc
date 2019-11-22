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

#include <ucsdpsys_pretty/statement/writeln/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_writeln_pretty::~statement_writeln_pretty()
{
}


statement_writeln_pretty::statement_writeln_pretty(
    const expression_list &a_elp,
    bool a_eoln,
    translator_pretty *cntxt
) :
    statement_writeln(a_elp),
    eoln(a_eoln),
    pretty(*cntxt)
{
}


statement_writeln_pretty::pointer
statement_writeln_pretty::create(const expression_list &a_elp, bool a_eoln,
    translator_pretty *cntxt)
{
    return pointer(new statement_writeln_pretty(a_elp, a_eoln, cntxt));
}


void
statement_writeln_pretty::pre_order_traversal()
    const
{
    pretty.print_token(eoln ? "writeln" : "write");
    if (has_args())
        pretty.print_token("(");
}


void
statement_writeln_pretty::infix_order_traversal(const expression::pointer &,
    bool last) const
{
    if (!last)
    {
        pretty.print_token(",");
        pretty.print_space();
    }
}


void
statement_writeln_pretty::post_order_traversal()
    const
{
    if (has_args())
        pretty.print_token(")");
}


// vim: set ts=8 sw=4 et :
