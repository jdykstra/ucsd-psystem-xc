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

#include <ucsdpsys_compile/expression/eq/multi_word.h>
#include <ucsdpsys_compile/expression/ne/multi_word.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_eq_multi_word::~expression_eq_multi_word()
{
}


expression_eq_multi_word::expression_eq_multi_word(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *a_code
) :
    expression_eq(a_lhs, a_rhs),
    code(*a_code)
{
    assert(a_lhs->strip_indirection());
    assert(a_rhs->strip_indirection());
}


expression_eq_multi_word::pointer
expression_eq_multi_word::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *a_code)
{
    return pointer(new expression_eq_multi_word(a_lhs, a_rhs, a_code));
}


void
expression_eq_multi_word::traversal(int pprec)
    const
{
    // Only the address of the parameters appear on the stack, not the values.
    // That's why we tested strip-indirection, earlier.
    get_lhs()->strip_indirection()->traversal(pprec);
    get_rhs()->strip_indirection()->traversal(pprec);

    code.breakpoint(get_location());
    code.emit_byte(EQU);
    code.emit_byte(OP_CMP_MULTI_WORD);

    int width = get_lhs()->get_type()->get_size_in_words();
    assert(width > 0);
    assert(width < 32768);
    code.emit_big(width);
}


expression::pointer
expression_eq_multi_word::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    return expression_eq_multi_word::create(e1, e2, &code);
}


expression::pointer
expression_eq_multi_word::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


void
expression_eq_multi_word::logical_traversal(int pprec,
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
            expression_ne_multi_word::create(get_lhs(), get_rhs(), &code);
        e2->traversal(pprec);
        true_branch->branch_from_here_if_false();
    }
}


// vim: set ts=8 sw=4 et :
