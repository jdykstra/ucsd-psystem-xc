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
#include <lib/ac/string.h>

#include <lib/name_type/list.h>
#include <lib/symbol.h>
#include <lib/type/nothing.h>
#include <lib/type/reference.h>
#include <lib/variable/name/list.h>

#include <ucsdpsys_pretty/scope/function/pretty.h>
#include <ucsdpsys_pretty/symbol/expression/constant/pretty.h>
#include <ucsdpsys_pretty/symbol/variable/normal/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


scope_function_pretty::~scope_function_pretty()
{
    pretty.indent_less();
}


scope_function_pretty::scope_function_pretty(
    const rcstring &a_name,
    bool is_func,
    bool isa_seg,
    translator_pretty *cntxt
) :
    scope_function(a_name),
    pretty(*cntxt),
    print_name_needed(true),
    print_lp_needed(true),
    print_rp_needed(false),
    print_return_type_needed(true),
    print_const_needed(true),
    print_type_needed(true),
    print_var_needed(true)
{
    pretty.indent_more();
    if (isa_seg)
    {
        pretty.print_token("segment");
        pretty.print_space();
    }
    if (is_func)
        pretty.print_token("function");
    else
        pretty.print_token("procedure");
    pretty.print_space();
}


scope_function_pretty::pointer
scope_function_pretty::create(const rcstring &a_name, bool is_func,
    bool isa_seg, translator_pretty *cntxt)
{
    return pointer(new scope_function_pretty(a_name, is_func, isa_seg, cntxt));
}


void
scope_function_pretty::print_name()
{
    if (print_name_needed)
    {
        print_name_needed = false;

        pretty.print_token(get_name());

        // Sadly, the parentheses may only be present if there is at
        // least one parameter.  It would be visually clearer (not to
        // mention less ambiguous) if an empty pair of parentheses
        // could be used.
    }
}


void
scope_function_pretty::print_lp()
{
    if (print_lp_needed)
    {
        print_lp_needed = false;
        pretty.print_token("(");
        print_rp_needed = true;
    }
    else
    {
        pretty.print_token(";");
        pretty.print_space();
    }
}


void
scope_function_pretty::notify_function_parameters(const name_type_list &ntl)
{
    print_name();
    if (ntl.empty())
        return;
    pretty.print_token("(");
    for (size_t j = 0; j < ntl.size(); ++j)
    {
        if (j)
        {
            pretty.print_token(";");
            pretty.print_space();
        }
        const name_type &nt = ntl[j];
        type::pointer tp = nt.get_type();
        if (type_reference::is_a(tp))
        {
            pretty.print_token("var");
            pretty.print_space();
            tp = tp->get_subtype();
        }
        for (size_t k = 0; k < nt.size(); ++k)
        {
            if (k)
            {
                pretty.print_token(",");
                pretty.print_space();
            }
            pretty.print_token(nt.get_name(k).get_name());
        }
        pretty.print_token(":");
        pretty.print_space();
        pretty.print_token(tp->get_pascal_name());
    }
    pretty.print_token(")");
}


void
scope_function_pretty::print_return_type()
{
    print_name();
    if (print_return_type_needed)
    {
        print_return_type_needed = false;
        type::pointer tp = get_return_type();
        if (!type_nothing::is_a(tp))
        {
            pretty.print_token(":");
            pretty.print_space();
            pretty.print_token(tp->get_pascal_name());
        }
        pretty.print_token(";");
        pretty.print_eoln();
    }
}


void
scope_function_pretty::begin_notify(void)
{
    print_return_type();
}


void
scope_function_pretty::print_const()
{
    if (print_const_needed)
    {
        print_const_needed = false;
        print_type_needed = true;
        print_var_needed = true;

        pretty.print_token("const");
        pretty.print_eoln();
    }
}


void
scope_function_pretty::print_type()
{
    if (print_type_needed)
    {
        print_const_needed = true;
        print_type_needed = true;
        print_var_needed = true;

        pretty.print_token("type");
        pretty.print_eoln();
    }
}


void
scope_function_pretty::print_var()
{
    if (print_var_needed)
    {
        print_const_needed = true;
        print_type_needed = true;
        print_var_needed = false;

        pretty.print_token("var");
        pretty.print_eoln();
    }
}


void
scope_function_pretty::constant_declaration_notify(const variable_name &cname,
    const expression::pointer &ep)
{
    print_const();
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
scope_function_pretty::type_declaration_notify(const variable_name &tname,
    const type::pointer &tsp)
{
    print_type();
    pretty.indent_more();
    pretty.print_token(tname.get_name());
    pretty.print_space();
    pretty.print_token("=");
    pretty.print_space();
    pretty.print_token(tsp->get_pascal_name());
    pretty.print_token(";");
    pretty.print_eoln();
    pretty.indent_less();
}


void
scope_function_pretty::variable_declaration_notify(
    const symbol_variable::pointer &svp)
{
    if (strchr(svp->get_name().c_str(), ':'))
        return;
    print_var();
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
scope_function_pretty::pre_order_traversal()
    const
{
}


void
scope_function_pretty::post_order_traversal()
    const
{
    pretty.print_token(";");
    pretty.print_eoln();
}


symbol::pointer
scope_function_pretty::constant_factory(const variable_name &vname,
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
scope_function_pretty::parameter_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    return
        symbol_variable_normal_pretty::create
        (
            vname,
            vtype,
            lex_level,
            symbol_variable::kind_parameter,
            &pretty
        );
}


symbol_variable::pointer
scope_function_pretty::variable_factory(const variable_name &vname,
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
