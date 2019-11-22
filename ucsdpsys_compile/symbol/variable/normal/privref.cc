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

#include <lib/debug.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/reference.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/address/privref.h>
#include <ucsdpsys_compile/expression/load/indirect.h>
#include <ucsdpsys_compile/expression/load/string.h>
#include <ucsdpsys_compile/symbol/variable/normal/privref.h>


symbol_variable_normal_privref::~symbol_variable_normal_privref()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_variable_normal_privref::symbol_variable_normal_privref(
    const variable_name &a_name,
    const type::pointer &a_tp,
    translator_compile *a_context
) :
    symbol_variable_normal(a_name, a_tp, 0, kind_variable),
    context(a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_variable_normal_privref::pointer
symbol_variable_normal_privref::create(const variable_name &a_name,
    const type::pointer &a_tp, translator_compile *a_context)
{
    return pointer(new symbol_variable_normal_privref(a_name, a_tp, a_context));
}


expression::pointer
symbol_variable_normal_privref::name_expression_factory(
    const symbol::pointer &sp, const location &where, int)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(this == sp.get());
    expression::pointer addr;
    type::pointer tp = get_type();
    if (type_reference::is_a(tp))
    {
        //
        // This variable is a reference to the actual variable, we have
        // to dereference a pointer to get the address of the variable.
        //
        tp = tp->get_subtype();
        assert(!type_reference::is_a(tp));
        assert(tp->get_size_in_words() > 0);

        // privref variable
        addr =
            expression_load_indirect::create
            (
                expression_address_privref::create
                (
                    where,
                    type_pointer_unpacked::create
                    (
                        type_pointer_unpacked::create(tp)
                    ),
                    sp,
                    context
                ),
                context
            );
    }
    else
    {
        //
        // This is a simple variable, rather than a reference
        //
        assert(tp->get_size_in_words() > 0);

        // privref variable
        addr =
            expression_address_privref::create
            (
                where,
                type_pointer_unpacked::create(tp),
                sp,
                context
            );
    }
    // Strings are different...
    if (type_string::is_a(tp))
        return expression_load_string::create(addr, context);
    return expression_load_indirect::create(addr, context);
}


// vim: set ts=8 sw=4 et :
