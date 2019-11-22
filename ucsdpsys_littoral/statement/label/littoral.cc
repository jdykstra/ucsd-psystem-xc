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

#include <ucsdpsys_littoral/statement/label/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_label_littoral::~statement_label_littoral()
{
}


statement_label_littoral::statement_label_littoral(
    const symbol::pointer &a_sym,
    const statement::pointer &a_body,
    translator_littoral *a_littoral
) :
    statement_label(a_sym, a_body),
    littoral(*a_littoral)
{
}


statement_label_littoral::pointer
statement_label_littoral::create(const symbol::pointer &a_sym,
    const statement::pointer &a_body, translator_littoral *a_littoral)
{
    return pointer(new statement_label_littoral(a_sym, a_body, a_littoral));
}


void
statement_label_littoral::pre_order_traversal(const symbol_label::pointer &name)
    const
{
    littoral.print_token(name->get_name());
    littoral.print_token(":");
    littoral.print_eoln();
}


// vim: set ts=8 sw=4 et :
