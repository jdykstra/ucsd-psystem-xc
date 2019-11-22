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

#include <lib/label.h>
#include <lib/pcode.h>
#include <lib/type/boolean.h>
#include <lib/type/integer.h>
#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/function_call/odd.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_odd::~expression_function_call_odd()
{
}


expression_function_call_odd::expression_function_call_odd(
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(args),
    code(*cntxt)
{
}


expression_function_call_odd::pointer
expression_function_call_odd::create(const expression_list &args,
    translator_compile *cntxt)
{
    return pointer(new expression_function_call_odd(args, cntxt));
}


bool
expression_function_call_odd::is_constant(void)
    const
{
    assert(get_child_count() == 1);
    return get_child(0)->is_constant();
}


expression::pointer
expression_function_call_odd::optimize(void)
    const
{
    assert(get_child_count() == 1);
    expression::pointer ep1 = get_child(0);
    assert(type_integer::is_a(ep1));

    expression::pointer ep2 = ep1->optimize();
    if (ep2->is_constant())
    {
        bool b = 1 & ep2->get_integer_value();
        return expression_boolean_compile::create(get_location(), b, &code);
    }
    expression_list args;
    args.push_back(ep2);
    return create(args, &code);
}


expression::pointer
expression_function_call_odd::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


type::pointer
expression_function_call_odd::get_type(void)
    const
{
    return type_boolean::create();
}


void
expression_function_call_odd::logical_traversal(int pprec,
    label::pointer &true_branch, label::pointer &false_branch,
    bool fall_through_preference) const
{
    traversal(pprec);

    //
    // We do not actually do anything.  The boolean opcodes (including
    // the FJP opcode) simply look at the least significant bit, which
    // (surprize!) indicates whether or not the integer value is odd.
    //
    if (fall_through_preference)
        false_branch->branch_from_here_if_false();
    else
        true_branch->branch_from_here_if_true();
}


expression::pointer
expression_function_call_odd::ord_odd_hack(void)
    const
{
    assert(get_child_count() == 1);
    if (!type_integer::is_a(get_child(0)))
        return expression::pointer();
    return get_child(0);
}


// vim: set ts=8 sw=4 et :
