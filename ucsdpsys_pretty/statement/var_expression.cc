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

#include <ucsdpsys_pretty/statement/var_expression.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_var_expression::~statement_var_expression()
{
}


statement_var_expression::statement_var_expression(
    const variable_name &a_name,
    const expression::pointer &a_ep,
    translator_pretty *a_pretty
) :
    name(a_name),
    ep(a_ep),
    pretty(*a_pretty)
{
}


statement_var_expression::pointer
statement_var_expression::create(const variable_name &a_name,
    const expression::pointer &a_ep, translator_pretty *a_pretty)
{
    return pointer(new statement_var_expression(a_name, a_ep, a_pretty));
}


void
statement_var_expression::traversal(void)
    const
{
    pretty.print_token("var");
    pretty.print_space();
    pretty.print_token(name.get_name());
    pretty.print_space();
    pretty.print_token(":=");
    pretty.print_space();
    ep->traversal(0);
}


bool
statement_var_expression::execution_flows_out(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
