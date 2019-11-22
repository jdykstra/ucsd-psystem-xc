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

#include <lib/type.h>
#include <ucsdpsys_littoral/scope/program/littoral.h>
#include <ucsdpsys_littoral/symbol/expression/constant/littoral.h>
#include <ucsdpsys_littoral/symbol/variable/normal/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


scope_program_littoral::~scope_program_littoral()
{
}


scope_program_littoral::scope_program_littoral(
    const rcstring &a_name,
    translator_littoral *cntxt
) :
    scope_program(a_name),
    littoral(*cntxt),
    have_printed_const_keyword(false),
    have_printed_type_keyword(false),
    have_printed_var_keyword(false)
{
    littoral.print_token("//");
    littoral.print_space();
    littoral.print_token("program");
    littoral.print_space();
    littoral.print_token(get_name());
    littoral.print_eoln();
}


scope_program_littoral::pointer
scope_program_littoral::create(const rcstring &a_name,
    translator_littoral *cntxt)
{
    return pointer(new scope_program_littoral(a_name, cntxt));
}


void
scope_program_littoral::begin_notify()
{
}


void
scope_program_littoral::constant_declaration_notify(const variable_name &cname,
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
scope_program_littoral::type_declaration_notify(const variable_name &tname,
    const type::pointer &tp)
{
    littoral.print_token("typedef");
    littoral.print_space();
    rcstring n = tname.get_name().downcase();
    littoral.print_token(tp->get_c_declaration(n));
    littoral.print_token(";");
    littoral.print_eoln();
}


void
scope_program_littoral::variable_declaration_notify(
    const symbol_variable::pointer &svp)
{
    if (strchr(svp->get_name().c_str(), ':'))
        return;

    rcstring n = svp->get_name().downcase();
    littoral.print_token(svp->get_type()->get_c_declaration(n));
    littoral.print_token(";");
    littoral.print_eoln();
}


void
scope_program_littoral::pre_order_traversal()
    const
{
    littoral.print_eoln();
    littoral.print_eoln();
    littoral.print_token("int");
    littoral.print_eoln();
    littoral.print_token("main(int argc, char **argv)");
    littoral.print_eoln();
}


void
scope_program_littoral::post_order_traversal()
    const
{
}


void
scope_program_littoral::label_declaration_notify(const variable_name_list &)
{
}


symbol::pointer
scope_program_littoral::constant_factory(const variable_name &vname,
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
scope_program_littoral::parameter_factory(const variable_name &vname,
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
scope_program_littoral::variable_factory(const variable_name &vname,
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
