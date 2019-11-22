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

#include <lib/cardinal.h>
#include <lib/debug.h>
#include <lib/pascal/lex.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pcode.h>
#include <lib/type/char.h>
#include <lib/type/file.h>
#include <lib/type/integer.h>
#include <lib/type/long_integer.h>
#include <lib/type/nothing.h>
#include <lib/type/real.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/comma.h>
#include <ucsdpsys_compile/symbol/function/read.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_read::~symbol_function_read()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_read::symbol_function_read(
    bool a_eoln,
    translator_compile *a_code
) :
    symbol_function((a_eoln ? "readln" : "read"), location::builtin(),
        scope::pointer(), -1),
    eoln(a_eoln),
    code(*a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_read::pointer
symbol_function_read::create(bool a_eoln, translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_read(a_eoln, a_code));
}


expression::pointer
symbol_function_read::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (!eoln && rhs.size() < 2)
    {
        parameter_error_wrong_number(rhs, 1);
        return expression_error::create(rhs.get_location());
    }

    expression::pointer file;
    size_t posn = 1;
    if (rhs.size() >= 2 && type_file::is_a(rhs[1]->get_type()))
    {
        if (!type_char::is_a(rhs[1]->get_type()->get_subtype()))
        {
            const char *suggest = "get";
            if (type_nothing::is_a(rhs[1]->get_type()->get_subtype()))
                suggest = "blockread";
            pascal_lex_error
            (
                rhs[1]->get_location(),
                "call of procedure %s: parameter one: must be a file of char, "
                    "not a %s; procedure \"%s\" is more appropriate",
                get_name().quote_c().c_str(),
                rhs[1]->get_description().c_str(),
                suggest
            );
            return expression_error::create(rhs.get_location());
        }
        file = rhs[1];
        ++posn;
    }
    else
    {
        file = code.name_expression(rhs.get_location(), "input");
    }
    assert(file);

    if (!eoln && posn >= rhs.size())
    {
        parameter_error_wrong_number(rhs, posn);
        return expression_error::create(rhs.get_location());
    }

    expression_list collection;
    for (; posn < rhs.size(); ++posn)
    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[posn]);
        if (ep->is_error())
            continue;
        if (!ep->is_lvalue())
        {
            pascal_lex_error
            (
                ep->get_location(),
                "call of procedure %s: parameter %s: must be a variable, "
                    "not a %s",
                get_name().quote_c().c_str(),
                cardinal(posn).c_str(),
                ep->get_description().c_str()
            );
            continue;
        }
        if (type_real::is_a(ep))
        {
            code.need_segment(31);

            DEBUG(2, "real");
            expression::pointer func =
                code.name_expression(ep->get_location(), "sys:freadreal");
            expression_list args;
            args.push_back(func);
            args.push_back(file);
            args.push_back(ep);
            ep = func->function_call_expression_factory(args);
        }
        else if (type_char::is_a(ep))
        {
            DEBUG(2, "char");
            expression::pointer func =
                code.name_expression(ep->get_location(), "sys:freadchar");
            expression_list args;
            args.push_back(func);
            args.push_back(file);
            args.push_back(ep);
            ep = func->function_call_expression_factory(args);
        }
        else if (type_integer::is_a(ep))
        {
            DEBUG(2, "integer");
            expression::pointer func =
                code.name_expression(ep->get_location(), "sys:freadint");
            expression_list args;
            args.push_back(func);
            args.push_back(file);
            args.push_back(ep);
            ep = func->function_call_expression_factory(args);
        }
        else if (type_long_integer::is_a(ep))
        {
            DEBUG(2, "long integer");
            expression::pointer func =
                code.name_expression(ep->get_location(), "sys:freaddec");
            expression_list args;
            args.push_back(func);
            args.push_back(file);
            args.push_back(ep);

            expression::pointer e4 =
                expression_integer_compile::create
                (
                    ep->get_location(),
                    ep->get_type()->get_size_in_words(),
                    &code
                );
            args.push_back(e4);

            ep = func->function_call_expression_factory(args);

            code.need_segment(DECOPS_SEG_NUM);
        }
        else if (type_string::is_a(ep))
        {
            DEBUG(2, "string");
            expression::pointer func =
                code.name_expression(ep->get_location(), "sys:freadstring");
            expression_list args;
            args.push_back(func);
            args.push_back(file);
            args.push_back(ep);
            type_string::pointer tsp =
                boost::dynamic_pointer_cast<type_string>(ep->get_type());
            assert(tsp);
            expression::pointer w =
                expression_integer_compile::create
                (
                    ep->get_location(),
                    tsp->get_size(),
                    &code
                );
            args.push_back(w);
            ep = func->function_call_expression_factory(args);
        }
        else
        {
            pascal_lex_error
            (
                ep->get_location(),
                "call of procedure %s: parameter %s: must be a variable of "
                    "char, integer, real or string type, not a %s",
                get_name().quote_c().c_str(),
                cardinal(posn).c_str(),
                ep->get_description().c_str()
            );
            continue;
        }
        collection.push_back(ep);
    }
    if (eoln)
    {
        DEBUG(2, "eoln");
        expression::pointer func =
        code.name_expression(rhs.back()->get_location(), "sys:freadln");
        expression_list args;
        args.push_back(func);
        args.push_back(file);
        expression::pointer ep = func->function_call_expression_factory(args);
        collection.push_back(ep);
    }

    //
    // Now build the expression node.
    //
    return expression_comma::create(collection, &code);
}


expression::pointer
symbol_function_read::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
