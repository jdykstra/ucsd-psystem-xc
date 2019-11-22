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

#include <ucsdpsys_littoral/statement/expression/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_expression_littoral::~statement_expression_littoral()
{
}


statement_expression_littoral::statement_expression_littoral(
    const expression::pointer &a_arg,
    translator_littoral *a_littoral
) :
    statement_expression(a_arg),
    littoral(*a_littoral)
{
}


statement_expression_littoral::pointer
statement_expression_littoral::create(const expression::pointer &a_arg,
    translator_littoral *a_littoral)
{
    return pointer(new statement_expression_littoral(a_arg, a_littoral));
}


void
statement_expression_littoral::traversal(void)
    const
{
    statement_expression::traversal();
    littoral.print_token(";");
    littoral.print_eoln();
}


// vim: set ts=8 sw=4 et :
