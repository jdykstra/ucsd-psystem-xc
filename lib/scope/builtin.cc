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

#include <lib/scope/builtin.h>
#include <lib/pascal/lex.h>


scope_builtin::~scope_builtin()
{
}


scope_builtin::scope_builtin() :
    scope("built:in")
{
}


void
scope_builtin::define(const symbol::pointer &dsp, bool isa_param)
{
    // Check that it's defined from the correct place.
    {
        if (dsp->get_declaration_location() != location::builtin())
        {
            pascal_lex_error
            (
                dsp->get_declaration_location(),
                "symbol %s should have been defined at %s instead (bug)",
                dsp->get_name().quote_c().c_str(),
                location::builtin().get_file_name().quote_c().c_str()
            );
        }
    }

    // Check that it's defined at the correct lex level.
    if (dsp->get_lex_level() != -1)
    {
        pascal_lex_error
        (
            dsp->get_declaration_location(),
            "symbol %s should have lex level -1, but has %d instead (bug)",
            dsp->get_name().quote_c().c_str(),
            dsp->get_lex_level()
        );
    }

    scope::define(dsp, isa_param);
}


void
scope_builtin::notify_function_parameters(const name_type_list &)
{
    assert(!"this should not happen");
}


void
scope_builtin::constant_declaration_notify(const variable_name &,
    const expression::pointer &)
{
    // This happens when the builtins are defined.
}


void
scope_builtin::type_declaration_notify(const variable_name &,
    const type::pointer &)
{
    // This happens when the builtins are defined.
}


void
scope_builtin::variable_declaration_notify(const symbol_variable_p &)
{
    assert(!"should never reach here");
}


int
scope_builtin::get_lex_level_height(void)
    const
{
    return 0;
}


void
scope_builtin::label_declaration_notify(const variable_name_list &)
{
    assert(!"this should be impossible");
}


// vim: set ts=8 sw=4 et :
