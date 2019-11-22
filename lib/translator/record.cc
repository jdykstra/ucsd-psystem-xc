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

#include <lib/pascal/lex.h>
#include <lib/scope/record.h>
#include <lib/translator.h>
#include <lib/type/forward.h>
#include <lib/type/integer.h>
#include <lib/type/record.h>


type_record::pointer
translator::record_begin(bool is_packed)
{
    //
    // This is the persistent scope which exists for declaring the
    // record itself, and again for WITH statements.  (Usualy scopes are
    // ephemeral, existing only while a function is being defined.)
    //
    scope::pointer sp = record_scope_factory(is_packed);
    scopes.push_back(sp);

    //
    // This is the type which refers to the persistent scope.
    //
    return type_record::create(sp);
}


void
translator::record_field(variable_name_list &names, type::pointer &tp)
{
    //
    // Barf on forward declared types.
    //
    if (type_forward::is_a(tp))
    {
        pascal_lex_error
        (
            names[0].get_location(),
            "%s type is not sufficiently well specified",
            tp->get_name().c_str()
        );
        tp = type_integer::create();
    }

    //
    // The UCSD compiler declares them backwards, probably because right
    // recursion was used in the grammar.  That uses the parse stack for
    // storage, rather than the explicit list you see here.
    //
    for (size_t j = 0; j < names.size(); ++j)
    {
        size_t k = names.size() - 1 - j;
        symbol::pointer sp =
            scopes.variable_factory(names[k], tp, get_lex_level());
        scopes.define(sp, false);
    }
}


void
translator::record_end()
{
    scopes.pop_back();
}


// vim: set ts=8 sw=4 et :
