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
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/type/anything.h>
#include <lib/type/file.h>
#include <lib/type/integer.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/load/indirect_packed.h>
#include <ucsdpsys_compile/symbol/function/blockread.h>
#include <ucsdpsys_compile/symbol/function/user_defined.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_blockread::~symbol_function_blockread()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_blockread::symbol_function_blockread(
    translator_compile *cntxt
) :
    symbol_function("blockread", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    //
    // function blockread(f: file; var a; nblocks: integer): integer;
    // function blockread(f: file; var a; nblocks: integer;
    //     relblock: integer): integer;
    //
    name_type_list args;
    {
        variable_name_list names;
        names.push_back(variable_name("f", location::builtin()));
        type::pointer tp = type_file::create(type_anything::create());
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("a", location::builtin()));
        type::pointer tp = type_reference::create(type_anything::create());
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("nblocks", location::builtin()));
        type::pointer tp = type_integer::create();
        args.push_back(name_type(names, tp));
    }

    attach_function_parameters(args);
    attach_return_type(type_integer::create());
}


symbol_function_blockread::pointer
symbol_function_blockread::create(translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_blockread(cntxt));
}


expression::pointer
symbol_function_blockread::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    switch (rhs.size())
    {
    case 5:
    case 4:
        break;

    default:
        parameter_error_wrong_number(rhs, (rhs.size() < 4 ? 3 : 4));
        DEBUG(1, "}");
        return expression_error::create(rhs.get_location());
    }

    expression_list rhs2;
    {
        expression::pointer ep =
            code.name_expression(rhs[0]->get_location(), "sys:fblockio");
        rhs2.push_back(ep);
    }

    // var f: file
    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[1]);
        DEBUG(2, "arg 1 = %s", ep->lisp_representation().c_str());
        if (!type_file::is_a(ep))
        {
            bad_file:
            pascal_lex_error
            (
                ep->get_location(),
                "function %s: parameter one: must be a file variable, not a %s",
                get_name().c_str(),
                ep->get_description().c_str()
            );
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        ep = ep->strip_indirection();
        if (!ep)
            goto bad_file;
        rhs2.push_back(ep);
    }

    // var a: packed array of char
    // or, it could be a[n]
    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[2]);
        DEBUG(2, "arg 2 = %s", rhs[2]->lisp_representation().c_str());
        ep = ep->get_byte_pointer();
        if (!ep)
        {
            pascal_lex_error
            (
                rhs[2]->get_location(),
                "function %s: parameter two: must be a packed array of char, "
                    "not a %s",
                get_name().c_str(),
                rhs[2]->get_description().c_str()
            );
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        rhs2.push_back(ep);
    }

    // nblocks: integer
    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[3]);
        DEBUG(2, "arg 3 = %s", ep->lisp_representation().c_str());
        if (!type_integer::is_a(ep))
        {
            parameter_error_wrong_type(3, ep, type_integer::create());
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        rhs2.push_back(ep);
    }

    // rblock: integer
    if (rhs.size() == 5)
    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[4]);
        DEBUG(2, "arg 4 = %s", ep->lisp_representation().c_str());
        if (!type_integer::is_a(ep))
        {
            parameter_error_wrong_type(4, ep, type_integer::create());
            return expression_error::create(rhs.get_location());
        }
        rhs2.push_back(ep);
    }
    else
    {
        // If you read the FBLOCKIO function system sources,
        // system/system.c.text, you will see that if <0 is passed, it
        // defaults rblock to the current file position.
        rhs2.push_back
        (
            expression_integer_compile::create
            (
                rhs.back()->get_location(),
                -1,
                &code
            )
        );
    }

    // doread: boolean
    rhs2.push_back
    (
        expression_boolean_compile::create
        (
            rhs.back()->get_location(),
            true,
            &code
        )
    );

    //
    // Build the function call expression node.
    //
    expression::pointer result =
        expression_function_call_user_defined::create(rhs2, &code);
    DEBUG(1, "}");
    return result;
}


expression::pointer
symbol_function_blockread::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
