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
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/symbol/label.h>


symbol_label::~symbol_label()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_label::symbol_label(
    const variable_name &a_name,
    const type::pointer &a_typ,
    int a_lxlvl,
    const label::pointer &a_lp
) :
    symbol(a_name, a_typ, a_lxlvl),
    lp(a_lp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_label::pointer
symbol_label::create(const variable_name &a_name, const type::pointer &a_typ,
    int a_lxlvl, const label::pointer &a_lp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_label(a_name, a_typ, a_lxlvl, a_lp));
}


const char *
symbol_label::get_kind(void)
    const
{
    return "label";
}


void
symbol_label::check_defined(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (defined_here.empty())
    {
        if (referenced_here.empty())
        {
            pascal_lex_warning
            (
                get_declaration_location(),
                "label %s has not been defined or used",
                get_name().quote_c().c_str()
            );

            // Silence any future warning.
            variable_name vn(get_name(), get_declaration_location());
            referenced_here.push_back(vn);
        }
        else
        {
            pascal_lex_error
            (
                get_declaration_location(),
                "label %s has been declared but not defined",
                get_name().quote_c().c_str()
            );
        }

        // Silence any future warning.
        variable_name vn(get_name(), get_declaration_location());
        defined_here = vn;
        return;
    }
    if (referenced_here.empty())
    {
        pascal_lex_warning
        (
            defined_here.get_location(),
            "label %s is not used ...",
            get_name().quote_c().c_str()
        );
        pascal_lex_warning
        (
            get_declaration_location(),
            "... here is the %s declaration",
            get_name().quote_c().c_str()
        );

        // Silence any future warning.
        variable_name vn(get_name(), get_declaration_location());
        referenced_here.push_back(vn);
    }
    DEBUG(1, "}");
}


void
symbol_label::goto_from_here(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    lp->goto_from_here();
}


void
symbol_label::define_here(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (lp->is_defined())
    {
        pascal_lex_error
        (
            get_declaration_location(),
            "label %s has been defined more than once",
            get_name().quote_c().c_str()
        );
    }
    else
        lp->define_here();
}


bool
symbol_label::remember_definition(const variable_name &dname)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (defined_here.empty())
    {
        defined_here = dname;
        return true;
    }
    else
    {
        pascal_lex_error
        (
            dname.get_location(),
            "label %s has already been defined...",
            dname.get_name().quote_c().c_str()
        );
        pascal_lex_error
        (
            defined_here.get_location(),
            "... here is the earlier %s definition",
            defined_here.get_name().quote_c().c_str()
        );
        return false;
    }
}


void
symbol_label::remember_reference(const variable_name &uname)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    referenced_here.push_back(uname);
}


expression::pointer
symbol_label::name_expression_factory(const symbol::pointer &sp,
    const location &where, int)
{
    assert(this == sp.get());
    pascal_lex_error
    (
        where,
        "may not reference label %s as a variable",
        get_name().quote_c().c_str()
    );
    return expression_error::create(where);
}


// vim: set ts=8 sw=4 et :
