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

#include <lib/debug.h>
#include <lib/scope/program.h>


scope_program::~scope_program()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


scope_program::scope_program(const rcstring &a_name) :
    scope(a_name)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
scope_program::notify_function_parameters(const name_type_list &)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(!"the grammar is supposed to prevent this");
}


int
scope_program::get_lex_level_height(void)
    const
{
    return 0;
}


void
scope_program::label_declaration_notify(const variable_name_list &)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    // Ignore.
}


// vim: set ts=8 sw=4 et :
