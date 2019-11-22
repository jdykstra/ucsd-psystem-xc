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

#include <ucsdpsys_pretty/statement/goto/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_goto_pretty::~statement_goto_pretty()
{
}


statement_goto_pretty::statement_goto_pretty(
    const symbol::pointer &a_sp,
    translator_pretty *a_pretty
) :
    statement_goto(a_sp),
    pretty(*a_pretty)
{
}


statement_goto_pretty::pointer
statement_goto_pretty::create(const symbol::pointer &a_sp,
    translator_pretty *a_pretty)
{
    return pointer(new statement_goto_pretty(a_sp, a_pretty));
}


void
statement_goto_pretty::post_order_traversal(const symbol::pointer &sp)
    const
{
    pretty.print_token("goto");
    pretty.print_space();
    pretty.print_token(sp->get_name());
}


// vim: set ts=8 sw=4 et :
