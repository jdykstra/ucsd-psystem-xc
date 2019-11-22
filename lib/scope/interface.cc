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

#include <lib/debug.h>
#include <lib/scope/interface.h>


scope_interface::~scope_interface()
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
}


scope_interface::scope_interface(const rcstring &a_name) :
    scope(a_name, false)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
}


int
scope_interface::get_lex_level_height()
    const
{
    return 0;
}


void
scope_interface::notify_function_parameters(const name_type_list &)
{
    assert(!"interfaces don't do this");
}


void
scope_interface::constant_declaration_notify(const variable_name &,
    const expression::pointer &)
{
    // Nothing to do.
}


void
scope_interface::type_declaration_notify(const variable_name &,
    const type::pointer &)
{
    // Nothing to do.
}


void
scope_interface::variable_declaration_notify(const symbol_variable_p &)
{
    // Nothing to do.
}


void
scope_interface::label_declaration_notify(const variable_name_list &)
{
    // Nothing to do.
}


bool
scope_interface::is_a_unit(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
