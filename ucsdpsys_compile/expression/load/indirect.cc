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
#include <lib/expression/string.h>
#include <lib/label.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/symbol.h>
#include <lib/type/array.h>
#include <lib/type/boolean.h>
#include <lib/type/pointer/packed.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/string.h>
#include <lib/type/subrange.h>

#include <ucsdpsys_compile/expression/address/global.h>
#include <ucsdpsys_compile/expression/address/intermediate.h>
#include <ucsdpsys_compile/expression/address/local.h>
#include <ucsdpsys_compile/expression/addition/address.h>
#include <ucsdpsys_compile/expression/byte_pointer.h>
#include <ucsdpsys_compile/expression/subtraction/integer.h>
#include <ucsdpsys_compile/expression/check_range.h>
#include <ucsdpsys_compile/expression/inc.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/ixp.h>
#include <ucsdpsys_compile/expression/ixs.h>
#include <ucsdpsys_compile/expression/ldb.h>
#include <ucsdpsys_compile/expression/load/indirect.h>
#include <ucsdpsys_compile/expression/load/indirect_packed.h>
#include <ucsdpsys_compile/expression/load/string.h>
#include <ucsdpsys_compile/expression/lpa.h>
#include <ucsdpsys_compile/expression/mov.h>
#include <ucsdpsys_compile/expression/multiplication/integer.h>
#include <ucsdpsys_compile/expression/store/indirect.h>
#include <ucsdpsys_compile/expression/uptr2pptr.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_load_indirect::~expression_load_indirect()
{
}


expression_load_indirect::expression_load_indirect(
    const expression::pointer &a_operand,
    translator_compile *cntxt
) :
    expression_load(a_operand->get_location(),
        a_operand->get_type()->get_subtype()),
    code(*cntxt),
    operand(a_operand)
{
    assert(a_operand->get_type()->get_size_in_words() == 1);
    assert(type_pointer_unpacked::is_a(a_operand->get_type()));
    assert(a_operand->get_type()->get_subtype()->get_size_in_words() > 0);
}


expression_load_indirect::pointer
expression_load_indirect::create(const expression::pointer &a_operand,
    translator_compile *cntxt)
{
    return pointer(new expression_load_indirect(a_operand, cntxt));
}


void
expression_load_indirect::traversal(int)
    const
{
    // Load the address onto the stack.
    operand->traversal(0);

    code.breakpoint(get_location());
    unsigned size = get_type()->get_size_in_words();
    assert(size > 0);
    if (size == 1)
    {
        // Short INDirect load (ea+0)
        //
        // FIXME: this is an optimization, the optimize method should
        //        allocate a new expression_sind instance.
        code.emit_byte(SIND_0);
    }
    else
    {
        // LoaD Multiple
        assert(size < 256);
        code.emit_byte(LDM);
        code.emit_byte(size);
    }
}


void
expression_load_indirect::logical_traversal(int pprec,
    label::pointer &true_label, label::pointer &false_label,
    bool fall_through_preference) const
{
    assert(type_boolean::is_a(get_type()));
    traversal(pprec);
    if (fall_through_preference)
    {
        false_label->branch_from_here_if_false();
    }
    else
    {
        true_label->branch_from_here_if_true();
    }
}


expression::pointer
expression_load_indirect::optimize(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    expression::pointer o2 = operand->optimize();
    DEBUG(2, "o2 = %s", o2->lisp_representation().c_str());

    if (get_type()->get_size_in_words() == 1)
    {
        //
        // Rather than grope the operand, we let each class of operand take
        // care of it themselves.  This covers LDL (local), LDO (global) and
        // LOD (intermediate) opcodes.
        //
        expression_address::pointer eap =
            boost::dynamic_pointer_cast<expression_address>(o2);
        if (eap)
        {
            DEBUG(2, "fast load?");
            expression::pointer ep = eap->fast_load();
            if (ep)
            {
                DEBUG(2, "ep = %s", ep->lisp_representation().c_str());
                DEBUG(1, "yes }");
                return ep;
            }
            DEBUG(2, "no");
        }

        //
        // See if we can use IND (INDirect load).
        //
        expression_inc::pointer eip =
            boost::dynamic_pointer_cast<expression_inc>(o2);
        if (eip)
        {
            DEBUG(2, "INDirect load?");
            expression::pointer ep = eip->indirect_load(get_type(), 0);
            if (ep)
            {
                DEBUG(2, "ep = %s", ep->lisp_representation().c_str());
                DEBUG(1, "yes }");
                return ep;
            }
            DEBUG(2, "no");
        }
    }

    // If all else fails...
    expression::pointer result = create(o2, &code);
    DEBUG(1, "}");
    return result;
}


expression::pointer
expression_load_indirect::clone(const location &locn)
    const
{
    return create(operand->clone(locn), &code);
}


expression::pointer
expression_load_indirect::strip_indirection()
    const
{
    return operand;
}


expression::pointer
expression_load_indirect::assignment_expression_factory(
    const expression::pointer &a_rhs) const
{
    expression::pointer rhs(a_rhs);

    rhs = code.range_check(rhs, get_type());
    return expression_store_indirect::create(operand, rhs, &code);
}


expression::pointer
expression_load_indirect::dot_expression_factory(const location &locn,
    const symbol::pointer &sp) const
{
    assert(!operand->get_location().empty());

    //
    // The translator base class already checked, and this is a valid
    // thing to do at this point.  Add the offset of the record field
    // to the address of the record, and build a new indirect load from
    // that.  Leave it to the optimize() pass to figure out the cleverest
    // way to generate the code.
    //
    bit_address addr = sp->get_bit_offset();
    bool packed = sp->is_packed();
    assert(packed || addr.on_word_boundary());
    if (packed)
    {
        expression::pointer a1 =
            expression_addition_address::create
            (
                type_pointer_unpacked::create(sp->get_type()),
                operand,
                expression_integer_compile::create
                (
                    locn,
                    addr.get_word_addr(),
                    &code
                ),
                sp,
                &code
            );
        expression::pointer a2 =
            expression_uptr2pptr::create
            (
                type_pointer_packed::create(sp->get_type()),
                a1,
                expression_integer_compile::create
                (
                    locn,
                    sp->get_type()->get_size_in_bits(),
                    &code
                ),
                expression_integer_compile::create
                (
                    locn,
                    addr.get_bit_number(),
                    &code
                ),
                &code
            );
        return expression_load_indirect_packed::create(a2, &code);
    }

    expression::pointer a2 =
        expression_addition_address::create
        (
            type_pointer_unpacked::create(sp->get_type()),
            operand,
            expression_integer_compile::create
            (
                locn,
                addr.get_word_addr(),
                &code
            ),
            sp,
            &code
        );
    // Strings are different...
    if (type_string::is_a(sp->get_type()))
        return expression_load_string::create(a2, &code);
    return expression_load_indirect::create(a2, &code);
}


symbol::pointer
expression_load_indirect::get_symbol_hint()
    const
{
    return operand->get_symbol_hint();
}


expression::pointer
expression_load_indirect::array_index_expression_factory(
    const expression::pointer &a_rhs) const
{
    expression::pointer rhs = a_rhs;
    assert(type_pointer::is_a(operand->get_type()));
    type::pointer left_type = operand->get_type()->get_subtype();
    DEBUG(1, "left_type = %s", left_type->get_pascal_name().c_str());

    //
    // Strings are a special case of byte arrays.
    //
    {
        type_string::pointer tsp =
            boost::dynamic_pointer_cast<type_string>(left_type);
        if (tsp)
        {
            expression::pointer ep =
                expression_byte_pointer::create(operand, rhs);
            if (code.issue_range_check())
                ep = expression_ixs::create(ep, &code);
            return expression_ldb::create(ep, &code);
        }
    }

    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(left_type);
    assert(tap);
    type::pointer index_type = tap->get_index_type();
    DEBUG(1, "index_type = %s", index_type->get_pascal_name().c_str());

    //
    // range check the array index if necessary
    //
    // Don't need to range check boolean if it was packed.
    //     OTOH, if it wasn't packed, just rhs&1
    // Don't need to range check char if it was packed.
    //     OTOH, if it wasn't packed, just rhs&255 ?!?
    // Don't need to range check enum if it was packed
    //     AND has 2^n values.
    // Don't need to range check integer subrange if it was packed
    //     AND has 2^n values.
    {
        expression::pointer lo =
            expression_integer_compile::create
            (
                rhs->get_location(),
                index_type->get_minimum_value(),
                &code
            );
        if (code.issue_range_check())
        {
            expression::pointer hi =
                expression_integer_compile::create
                (
                    rhs->get_location(),
                    index_type->get_maximum_value(),
                    &code
                );
            rhs = expression_check_range::create(rhs, lo, hi, &code);
        }
        else if (rhs->is_constant())
        {
            long n = rhs->optimize()->get_integer_value();
            long min = index_type->get_minimum_value();
            long max = index_type->get_maximum_value();
            if (n < min || n > max)
            {
                type::pointer tp = rhs->get_type();
                pascal_lex_warning
                (
                    rhs->get_location(),
                    "expression value %s is not in the range %s..%s",
                    tp->get_human_readable_representation_of_value(n).c_str(),
                    tp->get_human_readable_representation_of_value(min).c_str(),
                    tp->get_human_readable_representation_of_value(max).c_str()
                );
            }
        }

        //
        // Subtract the array index lower subrange.
        //
        if (index_type->get_minimum_value() != 0)
        {
            rhs = expression_subtraction_integer::create(rhs, lo, &code);
        }
    }

    //
    // Packed arrays have their own opcodes.
    //
    if (tap->is_packed())
    {
        DEBUG(2, "subtype = %s", tap->get_subtype()->get_pascal_name().c_str());
        int max = tap->get_subtype()->get_maximum_value();
        if (max >= 128 && max < 256)
        {
            // Note: the UCSD native compiler uses packed arrays (as opposed to
            // byte arrays) for quantities of 6 and 7 bits, even though only 2
            // will fit into a 16-bit word.  Byte addressing would have been
            // faster, but they didn't, so we can't either.
            expression::pointer addr =
                expression_byte_pointer::create(operand, rhs);
            return expression_ldb::create(addr, &code);
        }
        expression::pointer addr = expression_ixp::create(operand, rhs, &code);
        return expression_load_indirect_packed::create(addr, &code);
    }

    //
    // Build a new variable address node, by adding the appropriate
    // offset to the array base pointer.
    //
    // FIXME: use IXP if possible.
    //
    expression::pointer sizeof_element =
        expression_integer_compile::create
        (
            operand->get_location(),
            left_type->get_subtype()->get_size_in_words(),
            &code
        );
    expression::pointer offset =
        expression_multiplication_integer::create(rhs, sizeof_element, &code);
    expression::pointer addr =
        expression_addition_address::create
        (
            type_pointer_unpacked::create(left_type->get_subtype()),
            operand,
            offset,
            get_symbol_hint(),
            &code
        );

    // Strings are different...
    assert(type_pointer::is_a(addr->get_type()));
    if (type_string::is_a(addr->get_type()->get_subtype()))
        return expression_load_string::create(addr, &code);

    //
    // Build a new indirect load to fetch the array element, after all
    // the set-up to calculate its address.
    //
    return expression_load_indirect::create(addr, &code);
}


bool
expression_load_indirect::is_simple_with_stmt()
    const
{
    // (@operand)^
    // (@operand)^^
    expression::pointer arg = operand;

    {
        expression_load_indirect::pointer p2 =
            boost::dynamic_pointer_cast<expression_load_indirect>(arg);
        if (p2)
            arg = p2->operand;
    }

    // (@arg)^
    {
        expression_address_local::pointer p2 =
            boost::dynamic_pointer_cast<expression_address_local>(arg);
        if (p2)
            return true;
    }

    {
        expression_address_global::pointer p2 =
            boost::dynamic_pointer_cast<expression_address_global>(arg);
        if (p2)
            return true;
    }

    {
        expression_address_intermediate::pointer p2 =
            boost::dynamic_pointer_cast<expression_address_intermediate>(arg);
        if (p2)
            return true;
    }

    return false;
}


expression::pointer
expression_load_indirect::mov_factory(const expression::pointer &lhs)
    const
{
    return expression_mov::create(lhs, operand, &code);
}


rcstring
expression_load_indirect::lisp_representation(void)
    const
{
    return
        (
            "(load-indirect {"
        +
            get_type()->get_pascal_name()
        +
            "} ["
        +
            get_location().representation()
        +
            "] "
        +
            operand->lisp_representation()
        +
            ")"
        );
}


expression::pointer
expression_load_indirect::get_byte_pointer(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", lisp_representation().c_str());
    expression::pointer idx =
        expression_integer_compile::create(operand->get_location(), 0, &code);
    expression::pointer result = expression_byte_pointer::create(operand, idx);
    DEBUG(1, "}");
    return result;
}


// vim: set ts=8 sw=4 et :
