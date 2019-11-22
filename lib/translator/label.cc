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

#include <lib/pascal/lex.h>
#include <lib/symbol/label.h>
#include <lib/translator.h>
#include <lib/type/label.h>


void
translator::label_declaration(const variable_name_list &names)
{
    if (!names.empty())
        complain_about_goto(names[0].get_location());
    type::pointer tp = type_label::create();
    for (size_t j = 0; j < names.size(); ++j)
    {
        symbol::pointer sp =
            symbol_label::create
            (
                names[j],
                tp,
                get_lex_level(),
                label_factory()
            );
        scopes.define(sp, false);
    }
    scopes.label_declaration_notify(names);
}


statement::pointer
translator::label_statement(const variable_name &name,
    const statement::pointer &body)
{
    complain_about_goto(name.get_location());
    type::pointer tp = type_label::create();
    symbol::pointer sp = scopes.lookup(name.get_name());
    if (!sp)
    {
        sp = scopes.lookup_fuzzy(name.get_name());
        if (!sp || sp->get_type() != tp)
        {
            pascal_lex_error
            (
                name.get_location(),
                "label %s not declared in this scope",
                name.get_name().quote_c().c_str()
            );

            // At this point, pretend that the label was properly declared,
            // to stop secondary errors.
            sp =
                symbol_label::create
                (
                    name,
                    tp,
                    get_lex_level(),
                    label_factory()
                );
            scopes.define(sp, false);
        }
        else
        {
            pascal_lex_error
            (
                name.get_location(),
                "label %s unknown, guessing you meant %s instead",
                name.get_name().quote_c().c_str(),
                sp->get_name().quote_c().c_str()
            );
        }
    }

    if (sp->get_type() != tp)
    {
        pascal_lex_error
        (
            name.get_location(),
            "expected a label name, not a %s name",
            sp->get_kind()
        );
        return empty_statement_factory();
    }
    if (sp->get_lex_level() != get_lex_level())
    {
        pascal_lex_error
        (
            name.get_location(),
            "the %s label was not declared in the current function...",
            name.get_name().quote_c().c_str()
        );
        pascal_lex_error
        (
            sp->get_declaration_location(),
            "... here is the %s declaration",
            sp->get_name().quote_c().c_str()
        );
        return empty_statement_factory();
    }

    // track the definition
    {
        symbol_label::pointer slp =
            boost::dynamic_pointer_cast<symbol_label>(sp);
        assert(slp);
        slp->remember_definition(name);
    }

    //
    // Have the derived class manufacture it.
    //
    return label_statement_factory(sp, body);
}


statement::pointer
translator::goto_statement(const variable_name &name)
{
    complain_about_goto(name.get_location());
    type::pointer tp = type_label::create();
    symbol::pointer sp = scopes.lookup(name.get_name());
    if (!sp)
    {
        sp = scopes.lookup_fuzzy(name.get_name());
        if (!sp || sp->get_type() != tp)
        {
            pascal_lex_error
            (
                name.get_location(),
                "label %s not declared in this scope",
                name.get_name().quote_c().c_str()
            );

            // At this point, pretend that the label was properly declared,
            // to stop secondary errors.
            sp =
                symbol_label::create
                (
                    name,
                    tp,
                    get_lex_level(),
                    label_factory()
                );
            scopes.define(sp, false);
        }
        else
        {
            pascal_lex_error
            (
                name.get_location(),
                "label %s unknown, guessing you meant %s instead",
                name.get_name().quote_c().c_str(),
                sp->get_name().quote_c().c_str()
            );
        }
    }

    if (sp->get_type() != tp)
    {
        pascal_lex_error
        (
            name.get_location(),
            "expected a label name, not a %s name",
            sp->get_kind()
        );
        return empty_statement_factory();
    }
    if (sp->get_lex_level() != get_lex_level())
    {
        // FIXME: need to implement non-local GOTO
        pascal_lex_error
        (
            name.get_location(),
            "the %s label was not declared in the current function...",
            name.get_name().quote_c().c_str()
        );
        pascal_lex_error
        (
            sp->get_declaration_location(),
            "... here is the %s declaration",
            sp->get_name().quote_c().c_str()
        );
        return empty_statement_factory();
    }

    // track the references
    {
        symbol_label::pointer slp =
            boost::dynamic_pointer_cast<symbol_label>(sp);
        assert(slp);
        slp->remember_reference(name);
    }

    //
    // Have the derived class manufacture it.
    //
    return goto_statement_factory(sp);
}


void
translator::complain_about_goto(const location &locn)
{
    if (goto_warning)
    {
        pascal_lex_warning
        (
            locn,
            "Go To Statement Considered Harmful (Edsger Dijkstra, 1968); "
            "to silence this message use (*$warning goto false*)"
        );
        goto_warning = false;
    }
}


// vim: set ts=8 sw=4 et :
