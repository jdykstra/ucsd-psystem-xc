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

#include <lib/type/boolean.h>
#include <lib/type/pointer.h>
#include <lib/pcode.h>

#include <ucsdpsys_compile/expression/inc.h>
#include <ucsdpsys_compile/expression/ind.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ind::~expression_ind()
{
}


expression_ind::expression_ind(
    const expression::pointer &a_lhs,
    int a_rhs,
    const type::pointer &a_tp,
    translator_compile &a_code
) :
    expression(a_lhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs),
    tp(a_tp),
    code(a_code)
{
    assert(rhs >= 0);
    assert(rhs <= 32767);
}


expression_ind::pointer
expression_ind::create(const expression::pointer &a_lhs, int a_rhs,
    const type::pointer &a_tp, translator_compile &a_code)
{
    return pointer(new expression_ind(a_lhs, a_rhs, a_tp, a_code));
}


void
expression_ind::traversal(int pprec)
    const
{
    assert(rhs >= 0);
    assert(rhs <= 32767);
    lhs->traversal(pprec);
    code.breakpoint(get_location());
    if (rhs < 8)
    {
        code.emit_byte(SIND_0 + rhs);
    }
    else
    {
        code.emit_byte(IND);
        code.emit_big(rhs);
    }
}


void
expression_ind::logical_traversal(int pprec, label::pointer &true_branch,
    label::pointer &false_branch, bool fall_through_preference) const
{
    assert(type_boolean::is_a(get_type()));
    traversal(pprec);
    if (fall_through_preference)
    {
        false_branch->branch_from_here_if_false();
    }
    else
    {
        true_branch->branch_from_here_if_true();
    }
}


int
expression_ind::get_precedence()
    const
{
    return 0;
}


type::pointer
expression_ind::get_type()
    const
{
    return tp;
}


bool
expression_ind::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_ind::has_side_effect()
    const
{
    return side_effect_no;
}


expression::pointer
expression_ind::optimize(void)
    const
{
    expression_inc::pointer eip =
        boost::dynamic_pointer_cast<expression_inc>(lhs);
    if (eip)
    {
        expression::pointer ep = eip->indirect_load(tp, rhs);
        if (ep)
            return ep->optimize();
    }

    return expression_ind::create(lhs, rhs, tp, code);
}


expression::pointer
expression_ind::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs, tp, code);
}


rcstring
expression_ind::lisp_representation(void)
    const
{
    return
        (
            "(INDirect-load {"
        +
            get_type()->get_pascal_name()
        +
            "} ["
        +
            get_location().representation()
        +
            "] "
        +
            lhs->lisp_representation()
        +
            " "
        +
            rcstring::printf("%d", rhs)
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
