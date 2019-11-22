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

#include <lib/scope/function/unit.h>


scope_function_unit::~scope_function_unit()
{
}


scope_function_unit::scope_function_unit(
    const rcstring &a_name,
    unsigned a_segnum
) :
    scope_function(a_name),
    segnum(a_segnum),
    next_proc_num(2)
{
}


int
scope_function_unit::get_lex_level_height(void)
    const
{
    return 0;
}


void
scope_function_unit::notify_function_parameters(const name_type_list &)
{
}


void
scope_function_unit::constant_declaration_notify(const variable_name &,
    const expression::pointer &)
{
}


void
scope_function_unit::type_declaration_notify(const variable_name &,
    const type::pointer &)
{
}


void
scope_function_unit::variable_declaration_notify(const symbol_variable_p &)
{
    // There is a chance we actually care about this one.
}


unsigned
scope_function_unit::get_segment_number(void)
    const
{
    return segnum;
}


void
scope_function_unit::label_declaration_notify(const variable_name_list &)
{
    // Do nothing.
}


// vim: set ts=8 sw=4 et :
