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

#include <lib/pcode.h>
#include <lib/type/array.h>

#include <ucsdpsys_compile/expression/gt/byte_array.h>
#include <ucsdpsys_compile/expression/le/byte_array.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_le_byte_array::~expression_le_byte_array()
{
}


expression_le_byte_array::expression_le_byte_array(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *a_code
) :
    expression_le(a_lhs, a_rhs),
    code(*a_code)
{
    assert(a_lhs->strip_indirection());
    assert(a_rhs->strip_indirection());
}


expression_le_byte_array::pointer
expression_le_byte_array::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *a_code)
{
    return pointer(new expression_le_byte_array(a_lhs, a_rhs, a_code));
}


void
expression_le_byte_array::traversal(int pprec)
    const
{
    // Only the address of the parameters appear on the stack, not the values.
    // That's why we tested strip-indirection, earlier.
    get_lhs()->strip_indirection()->traversal(pprec);
    get_rhs()->strip_indirection()->traversal(pprec);

    code.breakpoint(get_location());
    code.emit_byte(LEQ);
    code.emit_byte(OP_CMP_BYTE_ARRAY);

    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(get_lhs()->get_type());
    assert(tap);
    int width = tap->get_width();
    assert(width > 0);
    code.emit_big(width);
}


expression::pointer
expression_le_byte_array::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    return create(e1, e2, &code);
}


expression::pointer
expression_le_byte_array::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


void
expression_le_byte_array::logical_traversal(int pprec,
    label::pointer &true_branch, label::pointer &false_branch,
    bool fall_through_preference) const
{
    if (fall_through_preference)
    {
        traversal(pprec);
        false_branch->branch_from_here_if_false();
    }
    else
    {
        expression::pointer e2 =
            expression_gt_byte_array::create(get_lhs(), get_rhs(), &code);
        e2->traversal(pprec);
        true_branch->branch_from_here_if_false();
    }
}


// vim: set ts=8 sw=4 et :
