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
#include <lib/symbol/program.h>
#include <lib/type/function.h>

#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/exit.h>
#include <ucsdpsys_compile/symbol/function/exit.h>
#include <ucsdpsys_compile/symbol/function/user_defined.h>


symbol_function_exit::~symbol_function_exit()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_exit::symbol_function_exit(translator_compile *a_code) :
    symbol_function("exit", location::builtin(), scope::pointer(), -1),
    code(*a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    //
    // procedure exit(f: function);
    //
    variable_name_list names;
    names.push_back(variable_name("f", location::builtin()));
    type::pointer tp = type_function::create();
    name_type_list args;
    args.push_back(name_type(names, tp));
    attach_function_parameters(args);
}


symbol_function_exit::pointer
symbol_function_exit::create(translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_exit(a_code));
}


expression::pointer
symbol_function_exit::function_call_expression_factory(
    const expression_list &args) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (args.size() != 2)
    {
        parameter_error_wrong_number(args, 1);
        DEBUG(1, "}");
        return expression_error::create(args.get_location());
    }
    if (!type_function::is_a(args[1]->get_type()))
    {
        yuck:
        pascal_lex_error
        (
            args.get_location(),
            "exit function: parameter one: should be a function name, not a %s",
            args[1]->get_description().c_str()
        );
        DEBUG(1, "}");
        return expression_error::create(args.get_location());
    }

    DEBUG(2, "args[1] = %s", args[1]->lisp_representation().c_str());

    //
    // See if it is the name of a function or procedure.
    //
    {
        expression_function::pointer efp =
            boost::dynamic_pointer_cast<expression_function>(args[1]);
        if (efp)
        {
            symbol_function::pointer sfp = efp->get_sfp();
            DEBUG(2, "sfp = %p", sfp.get());
            assert(sfp);
            symbol_function_user_defined::pointer sfudp =
                boost::dynamic_pointer_cast<symbol_function_user_defined>(sfp);
            if (sfudp)
            {
                DEBUG(2, "sfudp = %p", sfudp.get());
                int seg_num = sfudp->get_segment_number();
                DEBUG(2, "seg_num = %d", seg_num);
                assert(seg_num >= 0);
                assert(seg_num < 64);
                int proc_num = sfudp->get_procedure_number();
                DEBUG(2, "proc_num = %d", proc_num);
                assert(proc_num > 0);
                assert(proc_num < 256);

                //
                // Build the function call.
                //
                expression_list rhs;
                expression::pointer e1 =
                    expression_integer_compile::create
                    (
                        efp->get_location(),
                        seg_num,
                        &code
                    );
                rhs.push_back(e1);
                expression::pointer e2 =
                    expression_integer_compile::create
                    (
                        efp->get_location(),
                        proc_num,
                        &code
                    );
                rhs.push_back(e2);
                DEBUG(1, "}");
                return expression_function_call_exit::create(rhs, &code);
            }
        }
    }

    //
    // Nope, can't figure it out.
    //
    DEBUG(1, "oops");
    goto yuck;
}


expression::pointer
symbol_function_exit::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
