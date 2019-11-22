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
#include <lib/statement/compound.h>
#include <lib/symbol.h>
#include <lib/type/nothing.h>
#include <lib/type/reference.h>
#include <lib/variable/name/list.h>

#include <ucsdpsys_littoral/scope/function/littoral.h>
#include <ucsdpsys_littoral/symbol/expression/constant/littoral.h>
#include <ucsdpsys_littoral/symbol/variable/normal/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


scope_function_littoral::~scope_function_littoral()
{
    littoral.indent_less();
}


scope_function_littoral::scope_function_littoral(
    const rcstring &a_name,
    translator_littoral *cntxt
) :
    scope_function(a_name),
    littoral(*cntxt)
{
}


scope_function_littoral::pointer
scope_function_littoral::create(const rcstring &a_name,
    translator_littoral *cntxt)
{
    return pointer(new scope_function_littoral(a_name, cntxt));
}


void
scope_function_littoral::notify_function_parameters(const name_type_list &ntl)
{
    ntl.flatten_into(parameters);
}


void
scope_function_littoral::begin_notify(void)
{
}


void
scope_function_littoral::constant_declaration_notify(const variable_name &cname,
    const expression::pointer &ep)
{
    littoral.print_token("const");
    littoral.print_space();
    rcstring n = cname.get_name().downcase();
    littoral.print_token(ep->get_type()->get_c_declaration(n));
    littoral.print_space();
    littoral.print_token("=");
    littoral.print_space();
    ep->traversal(0);
    littoral.print_token(";");
    littoral.print_eoln();
}


void
scope_function_littoral::type_declaration_notify(const variable_name &tname,
    const type::pointer &tsp)
{
    littoral.print_token("typedef");
    littoral.print_space();
    rcstring n = tname.get_name().downcase();
    littoral.print_token(tsp->get_c_declaration(n));
    littoral.print_token(";");
    littoral.print_eoln();
}


void
scope_function_littoral::variable_declaration_notify(
    const symbol_variable::pointer &svp)
{
    if (strchr(svp->get_name().c_str(), ':'))
        return;
    variable_name_list names;
    names.push_back(svp->get_name());
    name_type nt(names, svp->get_type());
    name_type_list ntl;
    ntl.push_back(nt);
    ntl.flatten_into(variables);
}


void
scope_function_littoral::traversal()
    const
{
    littoral.print_eoln();
    littoral.print_eoln();
    littoral.print_token(
        get_sp()->get_type()->get_subtype()->get_c_declaration("").trim());
    littoral.print_eoln();
    littoral.print_token(get_name().downcase());
    littoral.print_token("(");

    if (parameters.size() == 0)
        littoral.print_token("void");
    bool first = true;
    for (size_t j = 0; j < parameters.size(); ++j)
    {
        const name_type &nt = parameters[j];
        for (size_t k = 0; k < nt.size(); ++k)
        {
            if (first)
                first = false;
            else
            {
                littoral.print_token(",");
                littoral.print_space();
            }
            const variable_name &vn = nt.get_name(k);
            rcstring n = vn.get_name().downcase();
            rcstring t = nt.get_type()->get_c_declaration(n);
            littoral.print_token(t);
        }
    }

    littoral.print_token(")");
    littoral.print_eoln();
    littoral.print_token("{");
    littoral.print_eoln();
    littoral.indent_more();

    for (size_t j = 0; j < variables.size(); ++j)
    {
        const name_type &nt = variables[j];
        for (size_t k = 0; k < nt.size(); ++k)
        {
            const variable_name &vn = nt.get_name(k);
            rcstring n = vn.get_name().downcase();
            rcstring t = nt.get_type()->get_c_declaration(n);
            littoral.print_token(t);
            littoral.print_token(";");
            littoral.print_eoln();
        }
    }
    if (!variables.empty())
        littoral.print_eoln();

    statement_compound::pointer scp =
        boost::dynamic_pointer_cast<statement_compound>(get_body());
    if (scp)
    {
        const statement_list sl = scp->get_content();
        for (size_t j = 0; j < sl.size(); ++j)
            sl[j]->traversal();
    }

    littoral.indent_less();
    littoral.print_token("}");
    littoral.print_eoln();
}


symbol::pointer
scope_function_littoral::constant_factory(const variable_name &vname,
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
scope_function_littoral::parameter_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    return
        symbol_variable_normal_littoral::create
        (
            vname,
            vtype,
            lex_level,
            symbol_variable::kind_parameter,
            &littoral
        );
}


symbol_variable::pointer
scope_function_littoral::variable_factory(const variable_name &vname,
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
