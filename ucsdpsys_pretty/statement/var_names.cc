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

#include <ucsdpsys_pretty/statement/var_names.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_var_names::~statement_var_names()
{
}


statement_var_names::statement_var_names(
    const variable_name_list &a_names,
    const type::pointer &a_tp,
    translator_pretty *a_pretty
) :
    names(a_names),
    tp(a_tp),
    pretty(*a_pretty)
{
}


statement_var_names::pointer
statement_var_names::create(const variable_name_list &a_names,
    const type::pointer &a_tp, translator_pretty *a_pretty)
{
    return pointer(new statement_var_names(a_names, a_tp, a_pretty));
}


void
statement_var_names::traversal()
    const
{
    pretty.print_token("var");
    for (size_t j = 0; j < names.size(); ++j)
    {
        if (j)
            pretty.print_token(",");
        pretty.print_space();
        pretty.print_token(names[j].get_name());
    }
    pretty.print_token(":");
    pretty.print_space();
    pretty.print_token(tp->get_pascal_name());
}


bool
statement_var_names::execution_flows_out()
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
