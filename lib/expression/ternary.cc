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

#include <lib/expression/ternary.h>
#include <lib/translator.h>


expression_ternary::~expression_ternary()
{
}


static location
lsum3(const expression::pointer &a, const expression::pointer &b,
    const expression::pointer &c)
{
    return (a->get_location() + b->get_location() + c->get_location());
}


expression_ternary::expression_ternary(
    const expression::pointer &a_cond,
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator *a_xlat
) :
    expression(lsum3(a_cond, a_lhs, a_rhs)),
    cond(a_cond),
    lhs(a_lhs),
    rhs(a_rhs),
    xlat(*a_xlat)
{
}


void
expression_ternary::logical_traversal(int pprec, label::pointer &true_br,
    label::pointer &false_br, bool fall_thru) const
{
    pre_order_traversal(pprec);

    label::pointer lhs_br = xlat.label_factory();
    label::pointer rhs_br = xlat.label_factory();
    label::pointer end_br = xlat.label_factory();
    cond->logical_traversal(get_precedence(), lhs_br, rhs_br, true);

    quest_order_traversal(pprec);

    lhs_br->define_here();
    lhs->logical_traversal(get_precedence(), true_br, false_br, fall_thru);
    end_br->goto_from_here();

    colon_order_traversal(pprec);

    rhs_br->define_here();
    rhs->logical_traversal(get_precedence(), true_br, false_br, fall_thru);
    end_br->define_here();

    post_order_traversal(pprec);
}


void
expression_ternary::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);

    label::pointer lhs_br = xlat.label_factory();
    label::pointer rhs_br = xlat.label_factory();
    label::pointer end_br = xlat.label_factory();
    cond->logical_traversal(get_precedence(), lhs_br, rhs_br, true);

    quest_order_traversal(pprec);

    lhs_br->define_here();
    lhs->traversal(get_precedence());
    end_br->goto_from_here();

    colon_order_traversal(pprec);

    rhs_br->define_here();
    rhs->traversal(get_precedence());
    end_br->define_here();

    post_order_traversal(pprec);
}


int
expression_ternary::get_precedence(void)
    const
{
    return prec_ternary;
}


type::pointer
expression_ternary::get_type(void)
    const
{
    return lhs->get_type();
}


bool
expression_ternary::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_ternary::has_side_effect(void)
    const
{
    return
        (
            cond->has_side_effect()
        +
            lhs->has_side_effect()
        +
            rhs->has_side_effect()
        );
}


rcstring
expression_ternary::lisp_representation(void)
    const
{
    return
        (
            "(ternary {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            cond->lisp_representation()
        +
            " "
        +
            lhs->lisp_representation()
        +
            " "
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


void
expression_ternary::pre_order_traversal(int) const
{
    // Do nothing.
}


void
expression_ternary::quest_order_traversal(int) const
{
    // Do nothing.
}


void
expression_ternary::colon_order_traversal(int) const
{
    // Do nothing.
}


void
expression_ternary::post_order_traversal(int)
    const
{
    // Do nothing.
}


// vim: set ts=8 sw=4 et :
