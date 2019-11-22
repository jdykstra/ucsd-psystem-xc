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

#include <lib/debug.h>
#include <lib/pascal/lex.h>
#include <lib/type/char.h>
#include <lib/type/integer.h>

#include <ucsdpsys_compile/expression/char/compile.h>
#include <ucsdpsys_compile/expression/function_call/chr.h>


expression_function_call_chr::~expression_function_call_chr()
{
}


expression_function_call_chr::expression_function_call_chr(
    const location &locn,
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(locn, args),
    code(*cntxt)
{
    assert(args.size() == 1);
    assert(args[0]);
}


expression_function_call_chr::pointer
expression_function_call_chr::create(const location &locn,
    const expression_list &args, translator_compile *cntxt)
{
    assert(args.size() == 1);
    assert(args[0]);
    return pointer(new expression_function_call_chr(locn, args, cntxt));
}


bool
expression_function_call_chr::is_constant(void)
    const
{
    assert(get_child_count() == 1);
    return get_child(0)->is_constant();
}


expression::pointer
expression_function_call_chr::clone(const location &locn)
    const
{
    expression_list args;
    for (size_t j = 0; j < get_child_count(); ++j)
        args.push_back(get_child(j)->clone(locn));
    return create(locn, args, &code);
}


expression::pointer
expression_function_call_chr::optimize()
    const
{
    assert(get_child_count() == 1);
    expression::pointer ep1 = get_child(0);
    assert(ep1);
    assert(type_integer::is_a(ep1));

    expression::pointer ep2 = ep1->optimize();
    assert(ep2);
    if (ep2->is_constant())
    {
        int c = ep2->get_integer_value();
        type::pointer tp = type_char::create();
        int lo = tp->get_minimum_value();
        int hi = tp->get_maximum_value();
        if (c < lo || c > hi)
        {
            pascal_lex_error
            (
                get_location(),
                "character constant value %d is not in the range %d..%d",
                c,
                lo,
                hi
            );
            c = '?';
        }
        return expression_char_compile::create(get_location(), c, &code);
    }
    expression_list args;
    args.push_back(ep2);
    return expression_function_call_chr::create(get_location(), args, &code);
}


type::pointer
expression_function_call_chr::get_type()
    const
{
    return type_char::create();
}


void
expression_function_call_chr::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"chr is not bool");
}


// vim: set ts=8 sw=4 et :
