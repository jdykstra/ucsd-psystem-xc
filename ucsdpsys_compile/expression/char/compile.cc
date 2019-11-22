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

#include <lib/pcode.h>
#include <ucsdpsys_compile/expression/char/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_char_compile::~expression_char_compile()
{
}


expression_char_compile::expression_char_compile(
    const location &a_locn,
    unsigned char a_value,
    translator_compile *cntxt
) :
    expression_char(a_locn, a_value),
    code(*cntxt)
{
}


expression_char_compile::pointer
expression_char_compile::create(const location &a_locn,
    unsigned char a_value, translator_compile *cntxt)
{
    return pointer(new expression_char_compile(a_locn, a_value, cntxt));
}


void
expression_char_compile::traversal(int)
    const
{
    code.breakpoint(get_location());
    // should already be optimized by this point.
    code.emit_ldci(get_integer_value());
}


expression::pointer
expression_char_compile::optimize(void)
    const
{
    return create(get_location(), get_integer_value(), &code);
}


expression::pointer
expression_char_compile::clone(const location &locn)
    const
{
    return create(locn, get_integer_value(), &code);
}


// vim: set ts=8 sw=4 et :
