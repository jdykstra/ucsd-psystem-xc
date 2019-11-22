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

#include <lib/ac/assert.h>
#include <lib/ac/stdio.h>
#include <lib/ac/string.h>

#include <lib/type.h>

#include <ucsdpsys_littoral/scope/unit/littoral.h>
#include <ucsdpsys_littoral/symbol/expression/constant/littoral.h>
#include <ucsdpsys_littoral/symbol/variable/normal/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


scope_unit_littoral::~scope_unit_littoral()
{
}


scope_unit_littoral::scope_unit_littoral(
    const rcstring &a_name,
    int a_code_seg,
    int a_data_seg,
    int a_llht,
    translator_littoral *cntxt
) :
    scope_unit(a_name, a_code_seg, a_data_seg, a_llht),
    littoral(*cntxt),
    have_printed_const_keyword(false),
    have_printed_type_keyword(false),
    have_printed_var_keyword(false)
{
    littoral.print_token("unit");
    littoral.print_space();
    littoral.print_token(get_name());
    littoral.print_token(";");
    littoral.print_eoln();
    if (a_code_seg)
    {
        char text[10];
        littoral.print_token("intrinsic");
        littoral.print_space();
        littoral.print_token("code");
        littoral.print_space();
        snprintf(text, sizeof(text), "%d", a_code_seg);
        littoral.print_token(text);
        if (a_data_seg)
        {
            littoral.print_space();
            littoral.print_token("data");
            littoral.print_space();
            snprintf(text, sizeof(text), "%d", a_data_seg);
            littoral.print_token(text);
        }
        littoral.print_token(";");
        littoral.print_eoln();
    }
}


scope_unit_littoral::pointer
scope_unit_littoral::create(const rcstring &a_name, int a_code_seg,
    int a_data_seg, int a_llht, translator_littoral *cntxt)
{
    return
        pointer
        (
            new scope_unit_littoral
            (
                a_name,
                a_code_seg,
                a_data_seg,
                a_llht,
                cntxt
            )
        );
}


void
scope_unit_littoral::begin_notify()
{
}


void
scope_unit_littoral::constant_declaration_notify(const variable_name &cname,
    const expression::pointer &ep)
{
    if (!have_printed_const_keyword)
    {
        littoral.print_token("const");
        littoral.print_eoln();
        have_printed_const_keyword = true;
        have_printed_type_keyword = false;
        have_printed_var_keyword = false;
    }

    littoral.indent_more();
    littoral.print_token(cname.get_name());
    littoral.print_space();
    littoral.print_token("=");
    littoral.print_space();
    expression::pointer ep2 = ep;
    ep2->traversal(0);
    littoral.print_token(";");
    littoral.print_eoln();
    littoral.indent_less();
}


void
scope_unit_littoral::type_declaration_notify(const variable_name &tname,
    const type::pointer &tp)
{
    if (!have_printed_type_keyword)
    {
        littoral.print_token("type");
        littoral.print_eoln();
        have_printed_const_keyword = false;
        have_printed_type_keyword = true;
        have_printed_var_keyword = false;
    }

    littoral.indent_more();
    littoral.print_token(tname.get_name());
    littoral.print_space();
    littoral.print_token("=");
    littoral.print_space();
    littoral.print_token(tp->get_pascal_name());
    littoral.print_token(";");
    littoral.print_eoln();
    littoral.indent_less();
}


void
scope_unit_littoral::variable_declaration_notify(
    const symbol_variable::pointer &svp)
{
    if (strchr(svp->get_name().c_str(), ':'))
        return;
    if (!have_printed_var_keyword)
    {
        littoral.print_token("var");
        littoral.print_eoln();
        have_printed_const_keyword = false;
        have_printed_type_keyword = false;
        have_printed_var_keyword = true;
    }

    littoral.indent_more();
    littoral.print_token(svp->get_name());
    littoral.print_token(":");
    littoral.print_space();
    littoral.print_token(svp->get_type()->get_pascal_name());
    littoral.print_token(";");
    littoral.print_eoln();
    littoral.indent_less();
}


void
scope_unit_littoral::pre_order_traversal()
    const
{
}


void
scope_unit_littoral::post_order_traversal()
    const
{
    littoral.print_token(".");
    littoral.print_eoln();
}


void
scope_unit_littoral::label_declaration_notify(const variable_name_list &names)
{
    if (names.empty())
        return;
    if (strchr(names[0].get_name().c_str(), ':'))
        return;
    littoral.print_token("label");
    littoral.print_eoln();
    have_printed_const_keyword = false;
    have_printed_type_keyword = false;
    have_printed_var_keyword = false;
    littoral.indent_more();
    for (size_t j = 0; j < names.size(); ++j)
    {
        if (j)
        {
            littoral.print_token(",");
            littoral.print_space();
        }
        littoral.print_token(names[j].get_name());
    }
    littoral.print_token(";");
    littoral.print_eoln();
    littoral.indent_less();
}


symbol::pointer
scope_unit_littoral::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    return
        symbol_expression_constant_littoral::create
        (
            vname,
            lex_level,
            value,
            &littoral
        );
}


symbol_variable::pointer
scope_unit_littoral::parameter_factory(const variable_name &,
    const type::pointer &, int)
{
    assert(!"units can't have parameters");
}


symbol_variable::pointer
scope_unit_littoral::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    return
        symbol_variable_normal_littoral::create
        (
            vname,
            vtype,
            lex_level,
            symbol_variable::kind_variable,
            &littoral
        );
}


// vim: set ts=8 sw=4 et :
