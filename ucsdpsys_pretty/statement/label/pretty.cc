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

#include <ucsdpsys_pretty/statement/label/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_label_pretty::~statement_label_pretty()
{
}


statement_label_pretty::statement_label_pretty(
    const symbol::pointer &a_sym,
    const statement::pointer &a_body,
    translator_pretty *a_pretty
) :
    statement_label(a_sym, a_body),
    pretty(*a_pretty)
{
}


statement_label_pretty::pointer
statement_label_pretty::create(const symbol::pointer &a_sym,
    const statement::pointer &a_body, translator_pretty *a_pretty)
{
    return pointer(new statement_label_pretty(a_sym, a_body, a_pretty));
}


void
statement_label_pretty::pre_order_traversal(const symbol_label::pointer &name)
    const
{
    pretty.print_token(name->get_name());
    pretty.print_token(":");
    pretty.print_eoln();
}


// vim: set ts=8 sw=4 et :
