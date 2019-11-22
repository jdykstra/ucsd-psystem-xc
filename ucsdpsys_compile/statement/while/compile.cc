//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <ucsdpsys_compile/statement/while/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


statement_while_compile::~statement_while_compile()
{
}


statement_while_compile::statement_while_compile(
        const expression::pointer &a_condition,
        const statement::pointer &a_body, translator_compile *cntxt) :
    statement_while(a_condition, a_body, cntxt),
    code(*cntxt)
{
}


statement_while_compile::pointer
statement_while_compile::create(const expression::pointer &a_condition,
    const statement::pointer &a_body, translator_compile *cntxt)
{
    return pointer(new statement_while_compile(a_condition, a_body, cntxt));
}


// vim: set ts=8 sw=4 et :
