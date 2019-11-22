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

#include <lib/ac/assert.h>

#include <lib/pcode.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_integer_compile::~expression_integer_compile()
{
}


expression_integer_compile::expression_integer_compile(
    const location &a_locn,
    const rcstring &arg,
    translator_compile *cntxt
) :
    expression_integer(arg, a_locn),
    code(*cntxt)
{
}


expression_integer_compile::pointer
expression_integer_compile::create(const location &a_locn, const rcstring &arg,
    translator_compile *cntxt)
{
    return pointer(new expression_integer_compile(a_locn, arg, cntxt));
}


expression_integer_compile::expression_integer_compile(
    const location &a_locn,
    long arg,
    translator_compile *cntxt
) :
    expression_integer(arg, a_locn),
    code(*cntxt)
{
}


expression_integer_compile::pointer
expression_integer_compile::create(const location &a_locn, long arg,
    translator_compile *cntxt)
{
    return pointer(new expression_integer_compile(a_locn, arg, cntxt));
}


void
expression_integer_compile::traversal(int)
    const
{
    code.breakpoint(get_location());
    long val = get_integer_value();
    if (val < -32768)
    {
        code.error(get_location(), "integer constant %ld too small", val);
        val = -32768;
    }
    if (val >= 65536)
    {
        code.error(get_location(), "integer constant %ld too large", val);
        val = 65535;
    }
    code.emit_ldci(val);
}


void
expression_integer_compile::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"integers are not bool");
}


expression::pointer
expression_integer_compile::optimize(void)
    const
{
    return create(get_location(), get_integer_value(), &code);
}


expression::pointer
expression_integer_compile::clone(const location &locn)
    const
{
    return create(locn, get_integer_value(), &code);
}


// vim: set ts=8 sw=4 et :
