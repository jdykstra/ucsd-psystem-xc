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

#include <ucsdpsys_compile/expression/enum_tag/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_enum_tag_compile::~expression_enum_tag_compile()
{
}


expression_enum_tag_compile::expression_enum_tag_compile(
    const variable_name &a_name,
    const type::pointer &a_typ,
    int a_value,
    translator_compile *a_context
) :
    expression_enum_tag(a_name, a_typ, a_value),
    context(a_context)
{
}


expression_enum_tag_compile::pointer
expression_enum_tag_compile::create(const variable_name &a_name,
    const type::pointer &a_typ, int a_value, translator_compile *a_context)
{
    return
        pointer
        (
            new expression_enum_tag_compile(a_name, a_typ, a_value, a_context)
        );
}


void
expression_enum_tag_compile::traversal(int)
    const
{
    context->breakpoint(get_location());
    long val = get_integer_value();
    assert(val >= 0);
    assert(val < 32768);
    context->emit_ldci(val);
}


expression::pointer
expression_enum_tag_compile::clone(const location &locn)
    const
{
    return
        create
        (
            variable_name(get_name(), locn),
            get_type(),
            get_integer_value(),
            context
        );
}


expression::pointer
expression_enum_tag_compile::optimize(void)
    const
{
    return
        create
        (
            variable_name(get_name(), get_location()),
            get_type(),
            get_integer_value(),
            context
        );
}


// vim: set ts=8 sw=4 et :
