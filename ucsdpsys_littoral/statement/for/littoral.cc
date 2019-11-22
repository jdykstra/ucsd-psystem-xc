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

#include <ucsdpsys_littoral/statement/for/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_for_littoral::~statement_for_littoral()
{
}


statement_for_littoral::statement_for_littoral(
    const expression::pointer &a_var,
    const expression::pointer &a_limit1,
    bool a_descending,
    const expression::pointer &a_limit2,
    const statement::pointer &a_body,
    translator_littoral *a_littoral
) :
    statement_for(a_var, a_limit1, a_descending, a_limit2, a_body),
    littoral(*a_littoral)
{
}


statement_for_littoral::pointer
statement_for_littoral::create(const expression::pointer &a_var,
    const expression::pointer &a_limit1, bool a_descending,
    const expression::pointer &a_limit2, const statement::pointer &a_body,
    translator_littoral *a_littoral)
{
    return
        pointer
        (
            new statement_for_littoral
            (
                a_var,
                a_limit1,
                a_descending,
                a_limit2,
                a_body,
                a_littoral
            )
        );
}


void
statement_for_littoral::traversal(void)
    const
{
    littoral.print_token("for");
    littoral.print_space();
    littoral.print_token("(");
    get_var()->traversal(0);
    littoral.print_space();
    littoral.print_token("=");
    littoral.print_space();
    get_limit1()->traversal(0);
    littoral.print_token(";");
    littoral.print_space();
    get_var()->traversal(0);
    littoral.print_space();
    littoral.print_token(is_descending() ? ">=" : "<=");
    littoral.print_space();
    get_limit2()->traversal(0);
    littoral.print_token(";");
    littoral.print_space();
    littoral.print_token("++");
    get_var()->traversal(0);
    littoral.print_token(")");
    littoral.print_eoln();

    if (get_body()->is_compound())
    {
        get_body()->traversal();
    }
    else
    {
        littoral.indent_more();
        get_body()->traversal();
        littoral.indent_less();
    }
}


// vim: set ts=8 sw=4 et :
