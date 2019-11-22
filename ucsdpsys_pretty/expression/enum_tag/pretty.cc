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

#include <lib/variable/name.h>

#include <ucsdpsys_pretty/expression/enum_tag/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_enum_tag_pretty::~expression_enum_tag_pretty()
{
}


expression_enum_tag_pretty::expression_enum_tag_pretty(
    const variable_name &a_name,
    const type::pointer &a_typ,
    int a_value,
    translator_pretty *a_context
) :
    expression_enum_tag(a_name, a_typ, a_value),
    context(a_context)
{
}


expression_enum_tag_pretty::pointer
expression_enum_tag_pretty::create(const variable_name &a_name,
    const type::pointer &a_typ, int a_value, translator_pretty *a_context)
{
    return
        pointer
        (
            new expression_enum_tag_pretty(a_name, a_typ, a_value, a_context)
        );
}


expression::pointer
expression_enum_tag_pretty::clone(const location &locn)
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
expression_enum_tag_pretty::optimize(void)
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


void
expression_enum_tag_pretty::traversal(int)
    const
{
    context->print_token(get_name());
}


// vim: set ts=8 sw=4 et :
