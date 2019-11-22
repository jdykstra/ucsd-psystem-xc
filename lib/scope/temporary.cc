//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#include <lib/anonymous_name.h>
#include <lib/scope/temporary.h>
#include <lib/scope/stack.h>


scope_temporary::~scope_temporary()
{
    ss.set_data_mark(mark);
}


scope_temporary::scope_temporary(scope_stack &stk) :
    scope(anonymous_name(), false),
    ss(stk),
    mark(stk.get_data_mark())
{
}


bool
scope_temporary::is_temporary(void)
    const
{
    return true;
}


void
scope_temporary::attach_return_type(const type::pointer &)
{
    assert(!"grammatically impossible");
}


void
scope_temporary::attach_function_parameters(name_type_list *)
{
    assert(!"grammatically impossible");
}


bool
scope_temporary::was_forward(void)
    const
{
    assert(!"grammatically impossible");
    return false;
}


void
scope_temporary::set_forward(bool)
{
    assert(!"grammatically impossible");
}


void
scope_temporary::drop_locals(void)
{
    assert(!"grammatically impossible");
}


int
scope_temporary::get_lex_level_height(void)
    const
{
    return 0;
}


void
scope_temporary::label_declaration_notify(const variable_name_list &)
{
    // Ignore.
}


void
scope_temporary::notify_function_parameters(const name_type_list &)
{
    // Ignore.
}


void
scope_temporary::constant_declaration_notify(const variable_name &,
    const expression::pointer &)
{
    // Ignore.
}


void
scope_temporary::type_declaration_notify(const variable_name &,
    const type::pointer &)
{
    // Ignore.
}


void
scope_temporary::variable_declaration_notify(const symbol_variable_p &)
{
    // Ignore.
}


// vim: set ts=8 sw=4 et :
