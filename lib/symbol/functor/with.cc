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
#include <lib/pascal/lex.h>
#include <lib/scope/stack.h>
#include <lib/symbol/functor/with.h>


symbol_functor_with::~symbol_functor_with()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_functor_with::symbol_functor_with(
    scope_stack &a_scopes,
    const location &a_locn
) :
    scopes(a_scopes),
    locn(a_locn)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
symbol_functor_with::operator()(const symbol::pointer &sp)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);

    //
    // Check for shadowing earlier symbols.
    //
    symbol::pointer earlier = scopes.lookup(sp->get_name());
    if (earlier)
    {
        pascal_lex_warning
        (
            locn,
            "record field %s shadows an earlier symbol...",
            sp->get_name().quote_c().c_str()
        );
        pascal_lex_warning
        (
            earlier->get_declaration_location(),
            "... here is the earlier %s %s declaration",
            earlier->get_name().quote_c().c_str(),
            earlier->get_kind()
        );
    }
    DEBUG(1, "}");
}


// vim: set ts=8 sw=4 et :
