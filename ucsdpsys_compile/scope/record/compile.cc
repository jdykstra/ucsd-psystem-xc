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

#include <ucsdpsys_compile/scope/record/compile.h>
#include <ucsdpsys_compile/symbol/variable/normal/compile.h>


scope_record_compile::~scope_record_compile()
{
}


scope_record_compile::scope_record_compile(
    bool a_is_packed,
    translator_compile *a_context
) :
    scope_record(a_is_packed),
    context(a_context)
{
}


scope_record_compile::pointer
scope_record_compile::create(bool a_is_packed, translator_compile *a_context)
{
    return pointer(new scope_record_compile(a_is_packed, a_context));
}


symbol_variable::pointer
scope_record_compile::variable_factory(const variable_name &vname,
     const type::pointer &vtype, int lxlvl)
{
    return
        symbol_variable_normal_compile::create
        (
            vname,
            vtype,
            lxlvl,
            symbol_variable::kind_record_field,
            context
        );
}


// vim: set ts=8 sw=4 et :
