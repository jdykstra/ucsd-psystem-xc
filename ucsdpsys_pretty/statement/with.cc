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

#include <ucsdpsys_pretty/statement/with.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_with::~statement_with()
{
}


statement_with::statement_with(
    const expression::pointer &a_ep,
    const statement::pointer &a_sp,
    translator_pretty *cntxt
) :
    ep(a_ep),
    sp(a_sp),
    pretty(*cntxt)
{
}


statement_with::pointer
statement_with::create(const expression::pointer &a_ep,
    const statement::pointer &a_sp, translator_pretty *cntxt)
{
    return pointer(new statement_with(a_ep, a_sp, cntxt));
}


void
statement_with::traversal()
    const
{
    //
    // FIXME: look for nested WITH statements
    // and pretty print them as with-multi
    //
    pretty.print_token("with");
    pretty.print_space();
    ep->traversal(0);
    pretty.print_space();
    pretty.print_token("do");
    pretty.print_eoln();
    pretty.indent_more();
    sp->traversal();
    pretty.indent_less();
}


bool
statement_with::execution_flows_out()
    const
{
    return sp->execution_flows_out();
}


// vim: set ts=8 sw=4 et :
