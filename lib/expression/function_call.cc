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
#include <lib/ac/typeinfo>

#include <lib/expression/function_call.h>
#include <lib/expression/list.h>
#include <lib/type.h>


expression_function_call::~expression_function_call()
{
}


expression_function_call::expression_function_call(
    const expression_list &args
) :
    expression(args.get_location()),
    children(args)
{
    assert(!get_location().empty());
}


expression_function_call::expression_function_call(
    const location &locn,
    const expression_list &args
) :
    expression(locn),
    children(args)
{
    assert(!get_location().empty());
}


expression_function_call::expression_function_call(
    const location &locn
) :
    expression(locn),
    children()
{
    assert(!get_location().empty());
}


expression_function_call::expression_function_call(
    const expression_function_call &arg
) :
    expression(arg),
    children(arg.children)
{
    assert(!get_location().empty());
}


void
expression_function_call::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    for (size_t j = 0; j < children.size(); ++j)
    {
        children.get(j)->traversal(j ? 0 : get_precedence());
        if (j + 1 < children.size())
            comma_order_traversal(pprec, j);
    }
    post_order_traversal(pprec, children.empty() ? 0 : (children.size() - 1));
}


void
expression_function_call::logical_traversal(int pprec,
    label::pointer &true_branch, label::pointer &false_branch,
    bool fall_through_preference) const
{
    traversal(pprec);

    // This can happen when a user-defined function returns a boolean value.
    if (fall_through_preference)
        false_branch->branch_from_here_if_false();
    else
        true_branch->branch_from_here_if_true();
}


int
expression_function_call::get_precedence()
    const
{
    return prec_paren;
}


type::pointer
expression_function_call::get_type()
    const
{
    return children.get(0)->get_type()->get_subtype();
}


bool
expression_function_call::is_lvalue()
    const
{
    // No function can return a reference, and that would be the only
    // case where an L-value could be returned.
    return false;
}


expression::side_effect_t
expression_function_call::has_side_effect()
    const
{
    // Actually, this depends on the code which defines it having side
    // effects, but for now, just assume it does.  (And we may not
    // yet have its code to analize for side effects if it's forward
    // declared.)
    return side_effect_yes;
}


void
expression_function_call::pre_order_traversal(int)
    const
{
    // Do nothing.
}



void
expression_function_call::comma_order_traversal(int, int)
    const
{
    // Do nothing.
}


void
expression_function_call::post_order_traversal(int, int)
    const
{
    // Do nothing.
}


rcstring
expression_function_call::lisp_representation(void)
    const
{
    rcstring result("(function-call=");
    result += typeid(*this).name();
    for (size_t j = 0; j < children.size(); ++j)
        result += " " + children.get(j)->lisp_representation();
    result += ")";
    return result;
}


// vim: set ts=8 sw=4 et :
