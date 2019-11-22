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

#include <lib/pcode.h>

#include <ucsdpsys_compile/expression/nil/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_nil_compile::~expression_nil_compile()
{
}


expression_nil_compile::expression_nil_compile(
    const location &locn,
    translator_compile *a_code
) :
    expression_nil(locn),
    code(*a_code)
{
}


expression_nil_compile::pointer
expression_nil_compile::create(const location &locn, translator_compile *a_code)
{
    return pointer(new expression_nil_compile(locn, a_code));
}


void
expression_nil_compile::traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(LDCN);
}


expression::pointer
expression_nil_compile::optimize(void)
    const
{
    return create(get_location(), &code);
}


expression::pointer
expression_nil_compile::clone(const location &locn)
    const
{
    return create(locn, &code);
}


// vim: set ts=8 sw=4 et :
