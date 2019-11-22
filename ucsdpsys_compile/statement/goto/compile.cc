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

#include <ucsdpsys_compile/statement/goto/compile.h>


statement_goto_compile::~statement_goto_compile()
{
}


statement_goto_compile::statement_goto_compile(
    const symbol::pointer &a_sym,
    translator_compile *a_code
) :
    statement_goto(a_sym),
    code(*a_code)
{
}


statement_goto_compile::pointer
statement_goto_compile::create(const symbol::pointer &a_sym,
    translator_compile *a_code)
{
    return pointer(new statement_goto_compile(a_sym, a_code));
}


// vim: set ts=8 sw=4 et :
