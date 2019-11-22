//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/symbol/function.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/nothing.h>

#include <ucsdpsys_compile/expression/address/local.h>
#include <ucsdpsys_compile/expression/address/intermediate.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/load/indirect.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function::~expression_function()
{
}


expression_function::expression_function(
    const expression_function &arg
) :
    expression(arg),
    code(arg.code),
    sfp(arg.sfp)
{
}


expression_function::expression_function(
    const location &a_locn,
    const symbol_function::pointer &a_sfp,
    translator_compile *cntxt
) :
    expression(a_locn),
    code(*cntxt),
    sfp(a_sfp)
{
}


expression_function::pointer
expression_function::create(const location &a_locn,
    const symbol_function::pointer &a_sfp, translator_compile *cntxt)
{
    return pointer(new expression_function(a_locn, a_sfp, cntxt));
}


expression::pointer
expression_function::clone(const location &locn)
    const
{
    return create(locn, sfp, &code);
}


void
expression_function::traversal(int)
    const
{
    // The only way to get here is if there is a bug.  The error should
    // already have been reported to the user and the compile aborted.
    assert(0);
}


void
expression_function::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    // The only way to get here is if there is a bug.  The error should
    // already have been reported to the user and the compile aborted.
    assert(!"functions aren't boolean values");
}


expression::pointer
expression_function::assignment_expression_factory(
    const expression::pointer &rhs) const
{
    type::pointer ret = sfp->get_type()->get_subtype();
    if (type_nothing::is_a(ret))
    {
        pascal_lex_error
        (
            get_location(),
            "cannot assign return value of a procedure"
        );
        return expression_error::create(get_location());
    }

    //
    // This is one of the things I intensely dislike about Pascal: if
    // the current function's name appears on the left hand side of an
    // assignment, then it refers to an anonymous variable reserved
    // to hold the function return value.  There is no explicit RETURN
    // keyword.  Sigh.
    //
    // It gets worse: it doesn't even have to be the local procedure
    // or function, but any procedure or function in the scope stack.
    // Combine non-local return assignment with the non-local return
    // feature of EXIT(name) for added loathsome.
    //
    int down = 0;
    if (!code.is_current_function(sfp))
    {
        //
        // make sure it's a direct ancestor
        //
        if (!code.function_is_on_scope_stack(sfp))
        {
            pascal_lex_error
            (
                get_location(),
                "cannot assign return value because not a direct ancestor"
            );
            return expression_error::create(get_location());
        }

        //
        // This will need an intermediate variable address, rather than
        // a local address.
        //
        assert(code.get_lex_level() > sfp->get_lex_level());
        down = code.get_lex_level() - sfp->get_lex_level();
    }
    else
    {
        assert(code.get_lex_level() == sfp->get_lex_level());
    }

    //
    // Build a local variable right-hand-side, and then use the usual
    // assignment expression factory.
    //
    int offset = 1;
    type::pointer preta = type_pointer_unpacked::create(ret);
    expression::pointer addr =
        (
            down > 0
        ?
            (expression::pointer)
            expression_address_intermediate::create
            (
                get_location(),
                offset,
                preta,
                down,
                sfp,
                &code
            )
        :
            (expression::pointer)
            expression_address_local::create
            (
                get_location(),
                offset,
                preta,
                sfp,
                &code
            )
        );
    expression::pointer lhs = expression_load_indirect::create(addr, &code);

    //
    // Assignment type mismatches will be reported by this call:
    //
    return lhs->assignment_expression_factory(rhs);
}


int
expression_function::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_function::get_type()
    const
{
    return sfp->get_type();
}


bool
expression_function::is_lvalue()
    const
{
    //
    // This is tricky: we can't actually tell whether this is yay or nay
    // until it actually appears on the left hand side of an assignment.
    // So we pretend.
    //
    return code.is_current_function(sfp);
}


expression::side_effect_t
expression_function::has_side_effect()
    const
{
    return side_effect_no;
}


expression::pointer
expression_function::function_call_expression_factory(
    const expression_list &args) const
{
    return sfp->function_call_expression_factory(args);
}


expression::pointer
expression_function::optimize()
    const
{
    return pointer(new expression_function(*this));
}


rcstring
expression_function::lisp_representation(void)
    const
{
    return ("(function " + sfp->get_name() + ")");
}


// vim: set ts=8 sw=4 et :
