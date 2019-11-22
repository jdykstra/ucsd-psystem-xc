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

#include <lib/pcode.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/pointer.h>

#include <ucsdpsys_compile/expression/address.h>
#include <ucsdpsys_compile/expression/addition/address.h>
#include <ucsdpsys_compile/expression/addition/integer.h>
#include <ucsdpsys_compile/expression/inc.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_addition_address::~expression_addition_address()
{
}


expression_addition_address::expression_addition_address(
    const type::pointer &a_tp,
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    const symbol::pointer &a_hint,
    translator_compile *cntxt
) :
    expression_addition(a_lhs, a_rhs),
    code(*cntxt),
    tp(a_tp),
    hint(a_hint)
{
    assert(type_pointer::is_a(a_lhs->get_type()));
    assert(type_pointer::is_a(tp));
    // The two subtypes are not necessarily the same.
}


expression_addition_address::pointer
expression_addition_address::create(const type::pointer &a_tp,
    const expression::pointer &a_lhs, const expression::pointer &a_rhs,
    const symbol::pointer &a_hint, translator_compile *cntxt)
{
    return
        pointer
        (
            new expression_addition_address(a_tp, a_lhs, a_rhs, a_hint, cntxt)
        );
}


type::pointer
expression_addition_address::get_type()
    const
{
    return tp;
}


void
expression_addition_address::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


void
expression_addition_address::post_order_traversal(int)
    const
{
    //
    // There is no simple add-to-pointer opcode, but there is a
    // "multiply by a constant then add to pointer" opcode.  We use a
    // constant one (1) multiplier.
    //
    // The reason we don't simply use the ADI opcode is because these
    // pointers could be word pointers or they could by byte pointers.
    //
    // The reason we don't simply use the INC opcode is because the
    // number of words to advance is not a constant (in which case,
    // optimize() would have done it already).
    //
    code.breakpoint(get_location());
    code.emit_byte(IXA);
    code.emit_big(1);
}


expression::pointer
expression_addition_address::clone(const location &locn)
    const
{
    return
        create
        (
            tp,
            get_lhs()->clone(locn),
            get_rhs()->clone(locn),
            get_symbol_hint(),
            &code
        );
}


expression::pointer
expression_addition_address::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();

    //
    // Look for expressions of the form
    //     ((ptr + k1) + k2)
    // and turn them into expressions of the form
    //     (ptr + (k1 + k2))
    // and then optimize again.
    //
    expression_addition_address::pointer e1x =
        boost::dynamic_pointer_cast<expression_addition_address>(e1);
    if (e1x)
    {
        expression::pointer ep =
            expression_addition_address::create
            (
                tp,
                e1x->get_lhs(),
                expression_addition_integer::create(e1x->get_rhs(), e2, &code),
                get_symbol_hint(),
                &code
            );
        return ep->optimize();
    }

    //
    // Look for constants on the right hand side.
    //
    assert(type_integer::is_a(e2->get_type())
        || e2->get_type()->may_be_used_as_array_index());
    if (e2->is_constant())
    {
        //
        // Each of the various load address expressions (opcodes) can
        // have their offset advanced...
        //
        expression_address::pointer eap =
            boost::dynamic_pointer_cast<expression_address>(e1);
        if (eap)
        {
            expression::pointer ep =
                eap->advance_by_words
                (
                    e2->get_integer_value(),
                    tp,
                    get_symbol_hint()
                );
            if (ep)
                return ep;
        }

        //
        // The INC opcode can be used to index the word pointer by a
        // constant number of words.  It has a limited range.
        //
        long big = e2->optimize()->get_integer_value();
        if (big >= 0 && big < 32768)
            return expression_inc::create(tp, e1, big, hint, &code);
    }

    // Future optimization...
    //
    // IXA arg:big
    //    Index array. tos is an integer index, tos-1 is the array base
    //    word pointer, and (big) is the size (in words) of an array
    //    element.  Compute a word pointer (tos-1) + (arg * tos) to the
    //    indexed element and push the pointer.

    // Run out of tricks.
    return
        expression_addition_address::create
        (
            tp,
            e1,
            e2,
            get_symbol_hint(),
            &code
        );
}


symbol::pointer
expression_addition_address::get_symbol_hint()
    const
{
    return (hint ? hint : get_lhs()->get_symbol_hint());
}


// vim: set ts=8 sw=4 et :
