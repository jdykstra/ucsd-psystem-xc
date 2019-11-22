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
#include <lib/label.h>
#include <lib/pcode.h>
#include <lib/symbol.h>
#include <lib/type/array.h>
#include <lib/type/boolean.h>
#include <lib/type/pointer/packed.h>
#include <lib/type/subrange.h>
#include <ucsdpsys_compile/expression/address.h>
#include <ucsdpsys_compile/expression/addition/address.h>
#include <ucsdpsys_compile/expression/byte_pointer.h>
#include <ucsdpsys_compile/expression/subtraction/integer.h>
#include <ucsdpsys_compile/expression/check_range.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/ixp.h>
#include <ucsdpsys_compile/expression/load/indirect_packed.h>
#include <ucsdpsys_compile/expression/multiplication/integer.h>
#include <ucsdpsys_compile/expression/store/indirect_packed.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_load_indirect_packed::~expression_load_indirect_packed()
{
}


expression_load_indirect_packed::expression_load_indirect_packed(
    const expression::pointer &a_operand,
    translator_compile *cntxt
) :
    expression_load(a_operand->get_location(),
            a_operand->get_type()->get_subtype()),
    code(*cntxt),
    operand(a_operand)
{
    assert(a_operand->get_type()->get_size_in_words() == 3);
    assert(type_pointer_packed::is_a(a_operand->get_type()));
    assert(a_operand->get_type()->get_subtype()->get_size_in_words() > 0);
}


expression_load_indirect_packed::pointer
expression_load_indirect_packed::create(const expression::pointer &a_operand,
    translator_compile *cntxt)
{
    return pointer(new expression_load_indirect_packed(a_operand, cntxt));
}


void
expression_load_indirect_packed::traversal(int)
    const
{
    operand->traversal(0);
    code.breakpoint(get_location());
    code.emit_byte(LDP);
}


void
expression_load_indirect_packed::logical_traversal(int pprec,
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
expression_load_indirect_packed::optimize(void)
    const
{
    expression::pointer o2 = operand->optimize();
    return expression_load_indirect_packed::create(o2, &code);
}


expression::pointer
expression_load_indirect_packed::clone(const location &locn)
    const
{
    return create(operand->clone(locn), &code);
}


expression::pointer
expression_load_indirect_packed::strip_indirection()
    const
{
    return operand;
}


expression::pointer
expression_load_indirect_packed::assignment_expression_factory(
    const expression::pointer &a_rhs) const
{
    expression::pointer rhs = code.range_check(a_rhs, get_type());
    return expression_store_indirect_packed::create(operand, rhs, &code);
}


symbol::pointer
expression_load_indirect_packed::get_symbol_hint()
    const
{
    return operand->get_symbol_hint();
}


rcstring
expression_load_indirect_packed::lisp_representation(void)
    const
{
    return
        (
            "(load-indirect-packed {"
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
expression_load_indirect_packed::get_byte_pointer(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    expression::pointer result = operand->get_byte_pointer();
    DEBUG(1, "}");
    return result;
}


// vim: set ts=8 sw=4 et :
