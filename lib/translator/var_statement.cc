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

#include <lib/anonymous_name.h>
#include <lib/pascal/lex.h>
#include <lib/scope/temporary/compound.h>
#include <lib/translator.h>
#include <lib/type/file.h>


statement::pointer
translator::var_statement(const variable_name_list &names,
    const type::pointer &tp)
{
    assert(!names.empty());
    for (size_t j = 0; j < names.size(); ++j)
    {
        rcstring name = names[j].get_name();
        scope::pointer sp_scope;
        symbol::pointer sp = scopes.lookup(name, &sp_scope);
        if (sp)
        {
            if (sp_scope == scopes.back())
            {
                pascal_lex_error
                (
                    names[j].get_location(),
                    "variable %s conflicts with an earlier symbol...",
                    name.quote_c().c_str()
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
                    names[j].get_location(),
                    "variable %s shadows an earlier symbol...",
                    name.quote_c().c_str()
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

        sp = allocate_temporary_variable(name, tp);

        // FIXME: if we run the file constructor here, we could allow
        // temporary files.  We would have to run the destructor as well.
        if (type_file::is_a(tp))
        {
            pascal_lex_error
            (
                names[j].get_location(),
                "cannot create temporary files in this way"
            );
        }
    }

    return var_statement_factory(names, tp);
}


symbol::pointer
translator::allocate_temporary_variable(const type::pointer &tp)
{
    return allocate_temporary_variable(anonymous_name(), tp);
}


symbol::pointer
translator::allocate_temporary_variable(const rcstring &name,
    const type::pointer &tp)
{
    symbol::pointer sp = scopes.variable_factory(name, tp, get_lex_level());
    bit_address b = scopes.allocate_space(tp);
    scopes.define(sp, b);
    return sp;
}


statement::pointer
translator::var_statement(const variable_name &name,
    const expression::pointer &ep)
{
    scope::pointer sc_scope;
    symbol::pointer sp = scopes.lookup(name.get_name(), &sc_scope);
    if (sp)
    {
        if (sc_scope == scopes.back())
        {
            pascal_lex_error
            (
                name.get_location(),
                "variable %s conflicts with an earlier symbol...",
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
                "variable %s shadows an earlier symbol...",
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

    type::pointer tp = ep->get_type();
    sp = allocate_temporary_variable(name.get_name(), tp);

    // FIXME: run the constructor for files
    if (type_file::is_a(tp))
    {
        pascal_lex_error
        (
            name.get_location(),
            "cannot create temporary files in this way"
        );
    }

    return var_statement_factory(name, ep);
}


void
translator::compound_scope_begin(void)
{
    scope::pointer sp = compound_temporary_scope_factory(scopes);
    scopes.push_back(sp);
}


void
translator::compound_scope_end(void)
{
    scopes.pop_back();
}


// vim: set ts=8 sw=4 et :
