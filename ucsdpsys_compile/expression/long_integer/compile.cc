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

#include <lib/type/long_integer/sized.h>

#include <ucsdpsys_compile/expression/long_integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_long_integer_compile::~expression_long_integer_compile()
{
}


expression_long_integer_compile::expression_long_integer_compile(
    const location &a_locn,
    const type::pointer &a_tp,
    const long_integer &a_value,
    translator_compile *a_code
) :
    expression_long_integer(a_locn, a_tp, a_value),
    code(*a_code)
{
}


expression::pointer
expression_long_integer_compile::create(const location &a_locn,
    const type::pointer &a_tp, const long_integer &a_value,
    translator_compile *a_code)
{
    return
        pointer
        (
            new expression_long_integer_compile(a_locn, a_tp, a_value, a_code)
        );
}


expression::pointer
expression_long_integer_compile::create(const location &a_locn,
    const rcstring &str_value, translator_compile *a_code)
{
    long_integer a_value = long_integer::pascal_lex_lint_constant(str_value);
    type::pointer a_tp = type_long_integer_sized::create();
    return
        pointer
        (
            new expression_long_integer_compile(a_locn, a_tp, a_value, a_code)
        );
}


void
expression_long_integer_compile::traversal(int)
    const
{
    code.breakpoint(get_location());

    // FIXME: just like set constants, we could maybe use a smaller
    // constant, and then a DECOPS ADJUST opcode.
    code.emit_ldc(get_value().get_data(), get_value().get_data_size());

    if (type_long_integer_sized::is_a(get_type()))
        code.emit_ldci(get_value().get_data_size());
    else
        assert(get_type()->get_size_in_bytes() == get_value().get_data_size());
}


expression::pointer
expression_long_integer_compile::optimize(void)
    const
{
    return create(get_location(), get_type(), get_value(), &code);
}


expression::pointer
expression_long_integer_compile::clone(const location &locn)
    const
{
    return create(locn, get_type(), get_value(), &code);
}


// vim: set ts=8 sw=4 et :
