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

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/symbol/expression/asm.h>
#include <ucsdpsys_assemble/type/segment_relative.h>


symbol_expression_asm::~symbol_expression_asm()
{
}


symbol_expression_asm::symbol_expression_asm(
    const variable_name &a_name,
    const type::pointer &a_type,
    const expression::pointer &a_value,
    assembler *a_context
) :
    symbol_expression(a_name, a_type, 0, a_value),
    context(*a_context),
    export_me(false),
    export_done(false)
{
}


symbol_expression_asm::pointer
symbol_expression_asm::create(const variable_name &a_name,
    const type::pointer &a_type, const expression::pointer &a_value,
    assembler *a_context)
{
    return
        pointer(new symbol_expression_asm(a_name, a_type, a_value, a_context));
}


void
symbol_expression_asm::set_value(const expression::pointer &new_value)
{
    symbol_expression::set_value(new_value);

    // if its to be exported, do that now.
    if (export_me)
        export_def();
}


const char *
symbol_expression_asm::get_kind(void)
    const
{
    if (!get_value())
        return "forward";
    if (type_segment_relative::is_a(get_value()))
        return "label";
    return "absolute";
}


void
symbol_expression_asm::export_def(void)
{
    export_me = true;
    if (get_value())
    {
        if (!export_done)
        {
            // this is conditional so that if the say .def multiplt
            // times for the one symbol, we only export it once.
            context.export_def(get_name(), get_value());
            export_done = true;
        }
    }
}


// vim: set ts=8 sw=4 et :
