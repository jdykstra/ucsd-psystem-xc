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

#include <lib/type/pointer/packed.h>
#include <lib/type/pointer/unpacked.h>
#include <ucsdpsys_compile/expression/uptr2pptr.h>


expression_uptr2pptr::~expression_uptr2pptr()
{
}


expression_uptr2pptr::expression_uptr2pptr(
    const type::pointer &a_to,
    const expression::pointer &a_from,
    const expression::pointer &a_field_width,
    const expression::pointer &a_right_bit,
    translator_compile *cntxt
) :
    expression(a_from->get_location()),
    code(*cntxt),
    to(a_to),
    from(a_from),
    field_width(a_field_width),
    right_bit(a_right_bit)
{
    assert(type_pointer_packed::is_a(to));
    assert(type_pointer_unpacked::is_a(from->get_type()));

    if (field_width->is_constant())
    {
        long n = field_width->optimize()->get_integer_value();
        assert(n > 0);
        assert(n < 16);
    }
    if (right_bit->is_constant())
    {
        long n = right_bit->optimize()->get_integer_value();
        assert(n >= 0);
        assert(n < 16);
    }
    if (field_width->is_constant() && right_bit->is_constant())
    {
        long w = field_width->optimize()->get_integer_value();
        long r = right_bit->optimize()->get_integer_value();
        assert(r + w <= 16);
    }
}


expression_uptr2pptr::pointer
expression_uptr2pptr::create(const type::pointer &a_to,
    const expression::pointer &a_from, const expression::pointer &a_field_width,
    const expression::pointer &a_right_bit, translator_compile *cntxt)
{
    return
        pointer
        (
            new expression_uptr2pptr
            (
                a_to,
                a_from,
                a_field_width,
                a_right_bit,
                cntxt
            )
        );
}


type::pointer
expression_uptr2pptr::get_type()
    const
{
    return to;
}


void
expression_uptr2pptr::traversal(int)
    const
{
    from->traversal(0);
    field_width->traversal(0);
    right_bit->traversal(0);
}


void
expression_uptr2pptr::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(!"uptr2pptr is never bool");
}


int
expression_uptr2pptr::get_precedence()
    const
{
    return from->get_precedence();
}


bool
expression_uptr2pptr::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_uptr2pptr::has_side_effect()
    const
{
    return
        (
            side_effect_no
        +
            from->has_side_effect()
        +
            field_width->has_side_effect()
        +
            right_bit->has_side_effect()
        );
}


expression::pointer
expression_uptr2pptr::optimize()
    const
{
    expression::pointer e1 = from->optimize();
    expression::pointer e2 = field_width->optimize();
    expression::pointer e3 = right_bit->optimize();
    return expression_uptr2pptr::create(to, e1, e2, e3, &code);
}


expression::pointer
expression_uptr2pptr::clone(const location &locn)
    const
{
    expression::pointer e1 = from->clone(locn);
    expression::pointer e2 = field_width->clone(locn);
    expression::pointer e3 = right_bit->clone(locn);
    return expression_uptr2pptr::create(to, e1, e2, e3, &code);
}


rcstring
expression_uptr2pptr::lisp_representation(void)
    const
{
    return
        (
            "(uptr2pptr "
        +
            from->lisp_representation()
        +
            " "
        +
            field_width->lisp_representation()
        +
            " "
        +
            right_bit->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
