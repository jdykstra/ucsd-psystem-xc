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

#include <lib/ac/float.h>
#include <lib/ac/math.h>

#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/real.h>

#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/expression/function_call/pwroften.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_pwroften::~expression_function_call_pwroften()
{
}


expression_function_call_pwroften::expression_function_call_pwroften(
    const expression_function_call_pwroften &arg
) :
    expression_function_call(arg),
    code(arg.code)
{
}


expression_function_call_pwroften::expression_function_call_pwroften(
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(args),
    code(*cntxt)
{
}


expression_function_call_pwroften::pointer
expression_function_call_pwroften::create(const expression_list &args,
    translator_compile *cntxt)
{
    return pointer(new expression_function_call_pwroften(args, cntxt));
}


void
expression_function_call_pwroften::post_order_traversal(int, int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(CSP);
    code.emit_byte(CSP_PWROFTEN);
}


bool
expression_function_call_pwroften::is_constant(void)
    const
{
    assert(get_child_count() == 1);
    return get_child(0)->is_constant();
}


expression::pointer
expression_function_call_pwroften::optimize()
    const
{
    assert(get_child_count() == 1);
    expression::pointer ep = get_child(0)->optimize();
    if (ep->is_constant())
    {
        long i = ep->get_integer_value();
        if (i < FLT_MIN_10_EXP || i > FLT_MAX_10_EXP)
        {
            //
            // The Klebsch p-machine implementation uses the native
            // C 32-bit representation, i.e. "float".  So we use the
            // information provided by C to find the limits.
            //
            // Note that UCSD Pascal said 0..39
            //
            pascal_lex_error
            (
                ep->get_location(),
                "the power of ten requested (%ld) is out of range (%d..%d)",
                i,
                FLT_MIN_10_EXP,
                FLT_MAX_10_EXP
            );
            i = 0;
        }
        double d = pow(10.0, i);
        return expression_real_compile::create(ep->get_location(), d, &code);
    }

    expression_list rhs2;
    rhs2.push_back(ep);
    return pointer(new expression_function_call_pwroften(rhs2, &code));
}


expression::pointer
expression_function_call_pwroften::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


type::pointer
expression_function_call_pwroften::get_type()
    const
{
    return type_real::create();
}


void
expression_function_call_pwroften::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"pwroften it isn't boolean");
}


// vim: set ts=8 sw=4 et :
