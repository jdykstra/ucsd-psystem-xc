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

#include <lib/ac/assert.h>

#include <lib/scope/unit.h>


scope_unit::~scope_unit()
{
}


scope_unit::scope_unit(
    const rcstring &a_name,
    int a_code_seg,
    int a_data_seg,
    int a_llht
) :
    scope(a_name),
    code_seg(a_code_seg),
    data_seg(a_data_seg),
    llht(a_llht)
{
}


void
scope_unit::notify_function_parameters(const name_type_list &)
{
    assert(!"the grammar is supposed to prevent this");
}


int
scope_unit::get_lex_level_height(void)
    const
{
    // If the unit is the outer-most scope, lex level height = 0;
    // otherwise lex level height = 1.
    return llht;
}


void
scope_unit::label_declaration_notify(const variable_name_list &)
{
    // Ignore.
}


bool
scope_unit::is_a_unit(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
