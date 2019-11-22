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
#include <lib/type/array.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/pointer/byte.h>
#include <lib/type/reference.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/byte_pointer.h>


expression_byte_pointer::~expression_byte_pointer()
{
}


expression_byte_pointer::expression_byte_pointer(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs)
{
    DEBUG(2, "lhs is %s", lhs->get_description().c_str());
    DEBUG(2, "rhs is %s", rhs->get_description().c_str());
    assert(type_pointer::is_a(lhs->get_type()) ||
        type_reference::is_a(lhs->get_type()));
    assert
    (
        type_integer::is_a(rhs->get_type())
    ||
        type_enumerated::is_a(rhs->get_type())
    ||
        type_boolean::is_a(rhs->get_type())
    );
}


expression_byte_pointer::pointer
expression_byte_pointer::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs)
{
    return pointer(new expression_byte_pointer(a_lhs, a_rhs));
}


void
expression_byte_pointer::traversal(int)
    const
{
    lhs->traversal(0);
    rhs->traversal(0);
    // We don't actually emit any code of our own,
    // we just built the two word value on the stack.
}


void
expression_byte_pointer::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"can never be boolean");
}


int
expression_byte_pointer::get_precedence(void)
    const
{
    return prec_paren;
}


type::pointer
expression_byte_pointer::get_type(void)
    const
{
    type::pointer tp = lhs->get_type()->get_subtype();
    if (type_array::is_a(tp))
        tp = tp->get_subtype();
    if (type_string::is_a(tp))
        tp = type_char::create();
    return type_pointer_byte::create(tp);
}


bool
expression_byte_pointer::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_byte_pointer::has_side_effect(void)
    const
{
    return lhs->has_side_effect() + rhs->has_side_effect();
}


expression::pointer
expression_byte_pointer::optimize(void)
    const
{
    return create(lhs->optimize(), rhs->optimize());
}


expression::pointer
expression_byte_pointer::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn));
}


rcstring
expression_byte_pointer::lisp_representation(void)
    const
{
    return
        (
            "(byte-pointer "
        +
            lhs->lisp_representation()
        +
            " "
        +
            lhs->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
