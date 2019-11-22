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

#include <lib/debug.h>

#include <lib/expression/dereference.h>
#include <lib/type/reference.h>


expression_dereference::~expression_dereference()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_dereference::expression_dereference(
    const expression::pointer &a_operand
) :
    expression(a_operand->get_location()),
    operand(a_operand)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
expression_dereference::traversal(int pprec)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    pre_order_traversal(pprec);
    operand->traversal(get_precedence());
    post_order_traversal(pprec);
    DEBUG(1, "}");
}


void
expression_dereference::pre_order_traversal(int)
    const
{
    // Do nothing.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
expression_dereference::post_order_traversal(int)
    const
{
    // Do nothing.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


int
expression_dereference::get_precedence()
    const
{
    //
    // We don't need the carat for reference (implict pointers), we only
    // need them for explicit pointers.
    //
    if (is_explicit())
        return prec_unary;
    return operand->get_precedence();
}


type::pointer
expression_dereference::get_type()
    const
{
    return operand->get_type()->get_subtype();
}


bool
expression_dereference::is_lvalue()
    const
{
    return true;
}


expression::side_effect_t
expression_dereference::has_side_effect()
    const
{
    return operand->has_side_effect();
}


bool
expression_dereference::is_explicit()
    const
{
    DEBUG(1, "this = %s", lisp_representation().c_str());
    type::pointer tsp = operand->get_type();
    bool result = !type_reference::is_a(tsp);
    DEBUG(1, "is_explicit => %s", (result ? "true" : "false"));
    return result;
}


void
expression_dereference::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


rcstring
expression_dereference::lisp_representation(void)
    const
{
    return
        (
            "(dereference {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            operand->lisp_representation()
        +
            ")"
        );
}


expression::pointer
expression_dereference::strip_indirection(void)
    const
{
    return operand;
}


// vim: set ts=8 sw=4 et :
