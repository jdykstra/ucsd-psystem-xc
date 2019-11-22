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

#include <lib/pcode.h>
#include <lib/type/long_integer/sized.h>
#include <lib/type/nothing.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/function_call/str.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/lint2lint/compile.h>
#include <ucsdpsys_compile/expression/long_integer/compile.h>
#include <ucsdpsys_compile/expression/sas.h>
#include <ucsdpsys_compile/expression/string/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_str::~expression_function_call_str()
{
}


expression_function_call_str::expression_function_call_str(
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(args),
    code(*cntxt)
{
    assert(get_child_count() == 2);
    code.need_segment(DECOPS_SEG_NUM);
}


expression_function_call_str::pointer
expression_function_call_str::create(const expression_list &parameters,
    translator_compile *cntxt)
{
    return pointer(new expression_function_call_str(parameters, cntxt));
}


void
expression_function_call_str::traversal(int)
    const
{
    assert(get_child_count() == 2);

    // First parameter, long integer value.
    expression::pointer e1 = get_child(0);
    e1->traversal(0);

    // Second parameter, pointer to string.
    expression::pointer e2 = get_child(1);
    e2->traversal(0);

    // Hidden third paramater: maximum string length.
    type::pointer tp = e2->get_type()->get_subtype();
    type_string::pointer tsp = boost::dynamic_pointer_cast<type_string>(tp);
    assert(tsp);
    assert(type_string::is_a(tp));
    expression::pointer e3 =
        expression_integer_compile::create
        (
            get_child(0)->get_location(),
            tsp->get_size(),
            &code
        );
    e3->traversal(0);

    // Make the call.
    code.breakpoint(get_location());
    code.emit_ldci(DECOPS_STR);
    code.emit_byte(CXP);
    code.emit_byte(DECOPS_SEG_NUM);
    code.emit_byte(DECOPS_PROC_NUM);
}


bool
expression_function_call_str::is_constant(void)
    const
{
    return false;
}


expression::pointer
expression_function_call_str::optimize()
    const
{
    assert(get_child_count() == 2);
    expression::pointer e1 = get_child(0)->optimize();
    expression::pointer e2 = get_child(1)->optimize();

    //
    // See if we can eliminate the function call altogether.
    //
    if (code.feature_long_integer_constants())
    {
        expression_long_integer::pointer k2 =
            boost::dynamic_pointer_cast<expression_long_integer>(e2);
        if (k2)
        {
            expression::pointer a1 = get_child(0);
            expression::pointer a2 =
                expression_string_compile::create
                (
                    e2->get_location(),
                    k2->get_value().to_string(),
                    &code
                );
            return expression_sas::create(a1, a2, &code);
        }
    }

    expression_list rhs2;
    rhs2.push_back(e1);
    rhs2.push_back(e2);
    return create(rhs2, &code);
}


expression::pointer
expression_function_call_str::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


type::pointer
expression_function_call_str::get_type()
    const
{
    return type_nothing::create();
}


// vim: set ts=8 sw=4 et :
