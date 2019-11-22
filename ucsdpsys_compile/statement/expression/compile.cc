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

#include <lib/expression.h>
#include <ucsdpsys_compile/statement/expression/compile.h>


statement_expression_compile::~statement_expression_compile()
{
}


statement_expression_compile::statement_expression_compile(
    const expression::pointer &a_arg,
    translator_compile *cntxt
) :
    statement_expression(a_arg->optimize()),
    code(*cntxt)
{
}


statement_expression_compile::pointer
statement_expression_compile::create(const expression::pointer &a_arg,
    translator_compile *cntxt)
{
    return pointer(new statement_expression_compile(a_arg, cntxt));
}


// vim: set ts=8 sw=4 et :
