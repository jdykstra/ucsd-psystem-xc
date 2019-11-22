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
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/anything.h>
#include <lib/type/file.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/symbol/function/close.h>
#include <ucsdpsys_compile/symbol/function/user_defined.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_close::~symbol_function_close()
{
}


symbol_function_close::symbol_function_close(
    translator_compile *cntxt
) :
    symbol_function("close", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // procedure close(f: file);
    // procedure close(f: file, ftype: sys_closetype);
    //
    variable_name_list names;
    names.push_back(variable_name("f", location::builtin()));
    name_type_list args;
    type::pointer tp = type_file::create(type_anything::create());
    args.push_back(name_type(names, tp));
    attach_function_parameters(args);
}


symbol_function_close::pointer
symbol_function_close::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_close(cntxt));
}


expression::pointer
symbol_function_close::function_call_expression_factory(
    const expression_list &rhs) const
{
    expression::pointer arg2 =
        expression_integer_compile::create
        (
            rhs.back()->get_location(),
            CNORMAL,
            &code
        );
    switch (rhs.size())
    {
    case 2:
        break;

    case 3:
        {
            //
            // The UCSD compiler has a disgusting hack for the CLOSE
            // function.  If there is a second parameter it looks for four
            // specific identifiers.  That is, it fakes an enumerated type.
            // See compiler/bodypart.b.text around line 290.
            //
            // Rather than clutter the grammar further (and all the fall-out
            // that would entail) we don't fake it, we create a real
            // enumerated type.
            //
            // Yes, it is possible for users to re-define any of these four
            // symbols, potentially rendering a "valid" program invalid, but
            // if that happens
            //   1. a shadow warning will be emitted
            //   2. close will emit an error message
            // Which is to say, it will not be silently broken, and a good
            // indication of how to fix the problem is produced.
            //
            // See lib/translator/builtin.cc for where this enumerated
            // type is defined.
            //
            type::pointer tp = code.get_type_by_name("sys:closetype");
            assert(tp);
            expression::pointer ep =
                code.call_noparam_func_if_necessary(rhs[2]);
            if (ep->get_type() != tp)
            {
                pascal_lex_error
                (
                    ep->get_location(),
                    "%s: parameters two: must be one of normal, lock, "
                        "purge or crunch; not a %s",
                    get_name().c_str(),
                    ep->get_description().c_str()
                );
                return expression_error::create(ep->get_location());
            }
            arg2 = ep;
        }
        break;

    default:
        parameter_error_wrong_number(rhs, (rhs.size() < 2 ? 1 : 2));
        return expression_error::create(rhs.get_location());
    }

    //
    // The IOCHECK opcode is added by
    // expression_function_call_user_defined::post_order_traversal
    // ucsdpsys_compile/expression/function_call/user_defined.cc
    //
    expression_list rhs2;
    {
        expression::pointer ep =
            code.name_expression(rhs[0]->get_location(), "sys:fclose");
        rhs2.push_back(ep);
    }

    // var f: file
    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[1]);
        if (!type_file::is_a(ep))
        {
            pascal_lex_error
            (
                ep->get_location(),
                "function %s: parameter one: must be a file, not a %s",
                get_name().c_str(),
                ep->get_description().c_str()
            );
            return expression_error::create(rhs.get_location());
        }
        rhs2.push_back(ep->strip_indirection());
    }

    // ftype: CLOSETYPE = (CNORMAL,CLOCK,CPURGE,CCRUNCH);
    rhs2.push_back(arg2);

    // build the expression node.
    return expression_function_call_user_defined::create(rhs2, &code);
}


expression::pointer
symbol_function_close::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
