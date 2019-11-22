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
#include <ucsdpsys_pretty/scope/unit/pretty.h>
#include <ucsdpsys_pretty/symbol/expression/constant/pretty.h>
#include <ucsdpsys_pretty/symbol/variable/normal/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


scope_unit_pretty::~scope_unit_pretty()
{
}


scope_unit_pretty::scope_unit_pretty(
    const rcstring &a_name,
    int a_code_seg,
    int a_data_seg,
    int a_llht,
    translator_pretty *cntxt
) :
    scope_unit(a_name, a_code_seg, a_data_seg, a_llht),
    pretty(*cntxt),
    have_printed_const_keyword(false),
    have_printed_type_keyword(false),
    have_printed_var_keyword(false)
{
    pretty.print_token("unit");
    pretty.print_space();
    pretty.print_token(get_name());
    pretty.print_token(";");
    pretty.print_eoln();
    if (a_code_seg)
    {
        char text[10];
        pretty.print_token("intrinsic");
        pretty.print_space();
        pretty.print_token("code");
        pretty.print_space();
        snprintf(text, sizeof(text), "%d", a_code_seg);
        pretty.print_token(text);
        if (a_data_seg)
        {
            pretty.print_space();
            pretty.print_token("data");
            pretty.print_space();
            snprintf(text, sizeof(text), "%d", a_data_seg);
            pretty.print_token(text);
        }
        pretty.print_token(";");
        pretty.print_eoln();
    }
}


scope_unit_pretty::pointer
scope_unit_pretty::create(const rcstring &a_name, int a_code_seg,
    int a_data_seg, int a_llht, translator_pretty *cntxt)
{
    return
        pointer
        (
            new scope_unit_pretty(a_name, a_code_seg, a_data_seg, a_llht, cntxt)
        );
}


void
scope_unit_pretty::begin_notify()
{
}


void
scope_unit_pretty::constant_declaration_notify(const variable_name &cname,
    const expression::pointer &ep)
{
    if (!have_printed_const_keyword)
    {
        pretty.print_token("const");
        pretty.print_eoln();
        have_printed_const_keyword = true;
        have_printed_type_keyword = false;
        have_printed_var_keyword = false;
    }

    pretty.indent_more();
    pretty.print_token(cname.get_name());
    pretty.print_space();
    pretty.print_token("=");
    pretty.print_space();
    expression::pointer ep2 = ep;
    ep2->traversal(0);
    pretty.print_token(";");
    pretty.print_eoln();
    pretty.indent_less();
}


void
scope_unit_pretty::type_declaration_notify(const variable_name &tname,
    const type::pointer &tp)
{
    if (!have_printed_type_keyword)
    {
        pretty.print_token("type");
        pretty.print_eoln();
        have_printed_const_keyword = false;
        have_printed_type_keyword = true;
        have_printed_var_keyword = false;
    }

    pretty.indent_more();
    pretty.print_token(tname.get_name());
    pretty.print_space();
    pretty.print_token("=");
    pretty.print_space();
    pretty.print_token(tp->get_pascal_name());
    pretty.print_token(";");
    pretty.print_eoln();
    pretty.indent_less();
}


void
scope_unit_pretty::variable_declaration_notify(
    const symbol_variable::pointer &svp)
{
    if (strchr(svp->get_name().c_str(), ':'))
        return;
    if (!have_printed_var_keyword)
    {
        pretty.print_token("var");
        pretty.print_eoln();
        have_printed_const_keyword = false;
        have_printed_type_keyword = false;
        have_printed_var_keyword = true;
    }

    pretty.indent_more();
    pretty.print_token(svp->get_name());
    pretty.print_token(":");
    pretty.print_space();
    pretty.print_token(svp->get_type()->get_pascal_name());
    pretty.print_token(";");
    pretty.print_eoln();
    pretty.indent_less();
}


void
scope_unit_pretty::pre_order_traversal()
    const
{
}


void
scope_unit_pretty::post_order_traversal()
    const
{
    pretty.print_token(".");
    pretty.print_eoln();
}


void
scope_unit_pretty::label_declaration_notify(const variable_name_list &names)
{
    if (names.empty())
        return;
    if (strchr(names[0].get_name().c_str(), ':'))
        return;
    pretty.print_token("label");
    pretty.print_eoln();
    have_printed_const_keyword = false;
    have_printed_type_keyword = false;
    have_printed_var_keyword = false;
    pretty.indent_more();
    for (size_t j = 0; j < names.size(); ++j)
    {
        if (j)
        {
            pretty.print_token(",");
            pretty.print_space();
        }
        pretty.print_token(names[j].get_name());
    }
    pretty.print_token(";");
    pretty.print_eoln();
    pretty.indent_less();
}


symbol::pointer
scope_unit_pretty::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    return
        symbol_expression_constant_pretty::create
        (
            vname,
            lex_level,
            value,
            &pretty
        );
}


symbol_variable::pointer
scope_unit_pretty::parameter_factory(const variable_name &,
    const type::pointer &, int)
{
    assert(!"can't have a parameter here");
}


symbol_variable::pointer
scope_unit_pretty::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    return
        symbol_variable_normal_pretty::create
        (
            vname,
            vtype,
            lex_level,
            symbol_variable::kind_variable,
            &pretty
        );
}


// vim: set ts=8 sw=4 et :
