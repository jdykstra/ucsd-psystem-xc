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

#include <lib/debug.h>
#include <lib/pcode.h>
#include <lib/type/set/unsized.h>

#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/in/compile.h>
#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_in_compile::~expression_in_compile()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in_compile::expression_in_compile(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *a_code
) :
    expression_in(a_lhs, a_rhs),
    code(*a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in_compile::pointer
expression_in_compile::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_in_compile(a_lhs, a_rhs, a_code));
}


void
expression_in_compile::post_order_traversal(int)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    code.breakpoint(get_location());
    // must put size of set (in words) on the stack if it isn't already
    type::pointer tp = get_rhs()->get_type();
    assert(type_set::is_a(tp));
    if (type_set_unsized::is_a(tp))
        code.emit_ldci(tp->get_size_in_words());
    code.emit_byte(INN);
}


void
expression_in_compile::logical_traversal(int pprec, label::pointer &true_branch,
    label::pointer &false_branch, bool fall_through_preference) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    traversal(pprec);
    if (fall_through_preference)
    {
        // code gen positive, FJP to false_branch
        false_branch->branch_from_here_if_false();
    }
    else
    {
        // code gen negative, FJP to true_branch
        true_branch->branch_from_here_if_true();
        // FIXME: if rhs is a constant, invert the constant?
    }
    DEBUG(1, "}");
}


expression::pointer
expression_in_compile::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();

    //
    // If both are constant, we can ask the set if the bit is present.
    //
    if (e1->is_constant() && e2->is_constant())
    {
        expression_set_compile::pointer esp =
            boost::dynamic_pointer_cast<expression_set_compile>(e2);
        assert(esp);
        return
            expression_boolean_compile::create
            (
                get_location(),
                esp->get_bit(e1->get_integer_value()),
                &code
            );
    }

    //
    // If the right hand side is the empty set, the result is always false.
    //
    if (e2->is_constant())
    {
        expression_set_compile::pointer esp =
            boost::dynamic_pointer_cast<expression_set_compile>(e2);
        assert(esp);
        if (esp->get_value().empty())
        {
            const location &locn = get_location();
            return expression_boolean_compile::create(locn, false, &code);
        }
    }

    //
    // If the left hand side is constant, and OUTSIDE the range of the
    // set type, the result is always false.
    //
    if (e1->is_constant())
    {
        signed short e1val = e1->get_integer_value();
        type::pointer tp2 = e2->get_type();
        assert(type_set::is_a(tp2));
        type::pointer tp2s = tp2->get_subtype();
        if
        (
            e1val < tp2s->get_minimum_value()
        ||
            e1val > tp2s->get_maximum_value()
        )
        {
            const location &locn = get_location();
            return expression_boolean_compile::create(locn, false, &code);
        }
    }

    //
    // Reassemble.
    //
    return expression_in_compile::create(e1, e2, &code);
}


expression::pointer
expression_in_compile::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
