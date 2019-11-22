//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/pascal/lex.h>
#include <lib/translator.h>
#include <lib/type/enumerated.h>
#include <lib/variable/name.h>


type::pointer
translator::enumerated_begin()
{
    assert(!current_enum);
    current_enum = type_enumerated::create();
    return current_enum;
}


void
translator::enumerated_tag(const variable_name &name)
{
    assert(current_enum);
    scope::pointer sp_scope;
    symbol::pointer sp = scopes.lookup(name.get_name(), &sp_scope);
    if (sp)
    {
        if (sp_scope == scopes.back())
        {
            pascal_lex_error
            (
                name.get_location(),
                "enumerated constant %s conflicts with an earlier symbol...",
                name.get_name().quote_c().c_str()
            );
            pascal_lex_error
            (
                sp->get_declaration_location(),
                "... here is the earlier %s %s declaration",
                sp->get_name().quote_c().c_str(),
                sp->get_kind()
            );
        }
        else if (warning_shadow_flag)
        {
            pascal_lex_warning
            (
                name.get_location(),
                "enumerated constant %s shadows an earlier symbol...",
                name.get_name().quote_c().c_str()
            );
            pascal_lex_warning
            (
                sp->get_declaration_location(),
                "... here is the earlier %s %s declaration",
                sp->get_name().quote_c().c_str(),
                sp->get_kind()
            );
        }
    }
    int value = current_enum->size();
    sp = enumerated_symbol_factory(name, current_enum, get_lex_level(), value);
    current_enum->push_back(sp);
    scopes.define(sp, false);
}


void
translator::enumerated_end()
{
    assert(current_enum);
    current_enum.reset();
}


// vim: set ts=8 sw=4 et :
