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

#include <ucsdpsys_littoral/statement/writeln/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_writeln_littoral::~statement_writeln_littoral()
{
}


statement_writeln_littoral::statement_writeln_littoral(
    const expression_list &a_elp,
    bool a_eoln,
    translator_littoral *cntxt
) :
    statement_writeln(a_elp),
    eoln(a_eoln),
    littoral(*cntxt)
{
}


statement_writeln_littoral::pointer
statement_writeln_littoral::create(const expression_list &a_elp, bool a_eoln,
    translator_littoral *cntxt)
{
    return pointer(new statement_writeln_littoral(a_elp, a_eoln, cntxt));
}


void
statement_writeln_littoral::pre_order_traversal()
    const
{
    littoral.print_token(eoln ? "writeln" : "write");
    littoral.print_token("(");
}


void
statement_writeln_littoral::infix_order_traversal(const expression::pointer &,
    bool last) const
{
    if (!last)
    {
        littoral.print_token(",");
        littoral.print_space();
    }
}


void
statement_writeln_littoral::post_order_traversal()
    const
{
    littoral.print_token(")");
    littoral.print_token(";");
    littoral.print_eoln();
}


// vim: set ts=8 sw=4 et :
