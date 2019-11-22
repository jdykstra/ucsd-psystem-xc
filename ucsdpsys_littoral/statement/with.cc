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

#include <ucsdpsys_littoral/statement/with.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_with::~statement_with()
{
}


statement_with::statement_with(
    const expression::pointer &a_ep,
    const statement::pointer &a_sp,
    translator_littoral *cntxt
) :
    ep(a_ep),
    sp(a_sp),
    littoral(*cntxt)
{
}


statement_with::pointer
statement_with::create(const expression::pointer &a_ep,
    const statement::pointer &a_sp, translator_littoral *cntxt)
{
    return pointer(new statement_with(a_ep, a_sp, cntxt));
}


void
statement_with::traversal()
    const
{
    littoral.print_token("{");
    littoral.print_eoln();
    littoral.indent_more();

    sp->traversal();

    littoral.indent_less();
    littoral.print_token("}");
    littoral.print_eoln();
}


bool
statement_with::execution_flows_out()
    const
{
    return sp->execution_flows_out();
}


// vim: set ts=8 sw=4 et :
