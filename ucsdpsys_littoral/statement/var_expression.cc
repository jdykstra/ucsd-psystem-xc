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

#include <ucsdpsys_littoral/statement/var_expression.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_var_expression::~statement_var_expression()
{
}


statement_var_expression::statement_var_expression(
    const variable_name &a_name,
    const expression::pointer &a_ep,
    translator_littoral *a_littoral
) :
    name(a_name),
    ep(a_ep),
    littoral(*a_littoral)
{
}


statement_var_expression::pointer
statement_var_expression::create(const variable_name &a_name,
    const expression::pointer &a_ep, translator_littoral *a_littoral)
{
    return pointer(new statement_var_expression(a_name, a_ep, a_littoral));
}


void
statement_var_expression::traversal(void)
    const
{
    // Need to get the type right
    littoral.print_token("int");

    littoral.print_space();
    littoral.print_token(name.get_name());
    littoral.print_space();
    littoral.print_token("=");
    littoral.print_space();
    ep->traversal(0);
    littoral.print_token(";");
    littoral.print_eoln();
}


bool
statement_var_expression::execution_flows_out(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
