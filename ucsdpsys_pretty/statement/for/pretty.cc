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

#include <ucsdpsys_pretty/statement/for/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_for_pretty::~statement_for_pretty()
{
}


statement_for_pretty::statement_for_pretty(
    const expression::pointer &a_var,
    const expression::pointer &a_limit1,
    bool a_descending,
    const expression::pointer &a_limit2,
    const statement::pointer &a_body,
    translator_pretty *a_pretty
) :
    statement_for(a_var, a_limit1, a_descending, a_limit2, a_body),
    pretty(*a_pretty)
{
}


statement_for_pretty::pointer
statement_for_pretty::create(const expression::pointer &a_var,
    const expression::pointer &a_limit1, bool a_descending,
    const expression::pointer &a_limit2, const statement::pointer &a_body,
    translator_pretty *a_pretty)
{
    return
        pointer
        (
            new statement_for_pretty
            (
                a_var,
                a_limit1,
                a_descending,
                a_limit2,
                a_body,
                a_pretty
            )
        );
}


void
statement_for_pretty::traversal(void)
    const
{
    pretty.print_token("for");
    pretty.print_space();
    get_var()->traversal(0);
    pretty.print_space();
    pretty.print_token(":=");
    pretty.print_space();
    get_limit1()->traversal(0);
    pretty.print_space();
    pretty.print_token(is_descending() ? "downto" : "to");
    pretty.print_space();
    get_limit2()->traversal(0);
    pretty.print_space();
    pretty.print_token("do");
    pretty.print_eoln();

    pretty.indent_more();
    get_body()->traversal();
    pretty.indent_less();
}


// vim: set ts=8 sw=4 et :
