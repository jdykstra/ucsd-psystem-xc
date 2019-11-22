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
#include <lib/pcode.h>
#include <lib/type/array.h>
#include <lib/type/integer.h>
#include <lib/type/pointer/packed.h>
#include <lib/type/pointer/unpacked.h>
#include <ucsdpsys_compile/expression/addition/address.h>
#include <ucsdpsys_compile/expression/byte_pointer.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/ixp.h>
#include <ucsdpsys_compile/expression/uptr2pptr.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ixp::~expression_ixp()
{
}


expression_ixp::expression_ixp(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    code(*cntxt),
    lhs(a_lhs),
    rhs(a_rhs),
    tp
    (
        type_pointer_packed::create
        (
            a_lhs->get_type()->get_subtype()->get_subtype()
        )
    )
{
    assert(type_pointer_unpacked::is_a(lhs->get_type()));
    assert(type_array::is_a(lhs->get_type()->get_subtype()));
    assert(lhs->get_type()->get_subtype()->get_subtype()->get_size_in_bits()
            < 16);
}


expression_ixp::pointer
expression_ixp::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_ixp(a_lhs, a_rhs, cntxt));
}


type::pointer
expression_ixp::get_type()
    const
{
    return tp;
}


void
expression_ixp::traversal(int)
    const
{
    unsigned field_width = tp->get_subtype()->get_size_in_bits();
    assert(field_width < 16);
    assert(field_width > 0);
    unsigned elements_per_word = 16 / field_width;
    assert(elements_per_word > 0);

    lhs->traversal(0);
    rhs->traversal(0);
    code.breakpoint(get_location());
    code.emit_byte(IXP);
    code.emit_byte(elements_per_word);
    code.emit_byte(field_width);
}


expression::pointer
expression_ixp::optimize(void)
    const
{
    expression::pointer e2 = rhs->optimize();
    if (e2->is_constant())
    {
        unsigned field_width = tp->get_subtype()->get_size_in_bits();
        assert(field_width < 16);
        assert(field_width > 0);
        unsigned elements_per_word = 16 / field_width;
        assert(elements_per_word > 0);

        int idx = e2->get_integer_value();
        // it has been range checked
        assert(idx >= 0);

        //
        // word_offset: how many words to advance the base pointer.
        //     It is a constant, so it should be possible to improve the
        //     code further with another optimize pass.
        //
        unsigned word_offset = idx / elements_per_word;

        //
        // right_bit_number: the position of the bit field within the
        //     word pointed to
        //
        unsigned sub_idx = idx % elements_per_word;
        unsigned right_bit_number = sub_idx * field_width;

        //
        // Build the offset as an expression.  We add this to the
        // pointer.  Then we optimize again, because a single opcode can
        // probably be constructed (this is why we didn't optimize lhs
        // before this).
        //
        expression::pointer offset =
            expression_integer_compile::create
            (
                e2->get_location(),
                word_offset,
                &code
            );
        expression::pointer ap1 =
            expression_addition_address::create
            (
                type_pointer_unpacked::create(type_integer::create()),
                lhs,
                offset,
                get_symbol_hint(),
                &code
            );
        expression::pointer ap2 = ap1->optimize();

        return
            expression_uptr2pptr::create
            (
                tp,
                ap2,
                expression_integer_compile::create
                (
                    e2->get_location(),
                    field_width,
                    &code
                ),
                expression_integer_compile::create
                (
                    e2->get_location(),
                    right_bit_number,
                    &code
                ),
                &code
            );
    }
    expression::pointer e1 = lhs->optimize();
    return expression_ixp::create(e1, e2, &code);
}


expression::pointer
expression_ixp::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn), &code);
}


void
expression_ixp::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"ixp is not bool");
}


int
expression_ixp::get_precedence()
    const
{
    return prec_paren;
}


bool
expression_ixp::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_ixp::has_side_effect()
    const
{
    return (side_effect_no + lhs->has_side_effect() + rhs->has_side_effect());
}


rcstring
expression_ixp::lisp_representation(void)
    const
{
    return
        (
            "(ixp {"
        +
            get_type()->get_pascal_name()
        +
            "} "
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


expression::pointer
expression_ixp::get_byte_pointer(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", lisp_representation().c_str());
    type_pointer_packed::pointer tppp =
        boost::dynamic_pointer_cast<type_pointer_packed>(get_type());
    if (!tppp)
    {
        DEBUG(1, "}");
        return expression::pointer();
    }
    if (tppp->get_subtype()->get_size_in_bits() != 8)
    {
        DEBUG(1, "}");
        return expression::pointer();
    }
    expression::pointer result = expression_byte_pointer::create(lhs, rhs);
    DEBUG(1, "}");
    return result;
}


// vim: set ts=8 sw=4 et :
