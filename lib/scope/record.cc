//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#include <lib/anonymous_name.h>
#include <lib/scope/record.h>


scope_record::~scope_record()
{
}


scope_record::scope_record(bool a_packed) :
    scope(anonymous_name(), a_packed)
{
}


void
scope_record::notify_function_parameters(const name_type_list &)
{
    // Do nothing.  Not relevant.
    assert(0);
}


void
scope_record::constant_declaration_notify(const variable_name &,
    const expression::pointer &)
{
    // Do nothing.  Not relevant.
    assert(0);
}


void
scope_record::type_declaration_notify(const variable_name &,
    const type::pointer &)
{
    // Do nothing.  Not relevant.
    assert(0);
}


void
scope_record::variable_declaration_notify(const symbol_variable_p &)
{
    // Do nothing.
}


int
scope_record::get_lex_level_height(void)
    const
{
    return 0;
}


void
scope_record::label_declaration_notify(const variable_name_list &)
{
    // Do nothing.
}


bool
scope_record::will_accept_enums(void)
    const
{
    return false;
}


symbol::pointer
scope_record::constant_factory(const variable_name &, int,
    const expression::pointer &)
{
    assert(!"you can't declare constants within records");
}


symbol_variable::pointer
scope_record::parameter_factory(const variable_name &, const type::pointer &,
    int)
{
    assert(!"you can't declare parameters within records");
}


// vim: set ts=8 sw=4 et :
