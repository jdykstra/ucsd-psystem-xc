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
#include <lib/type/pointer/packed.h>
#include <ucsdpsys_compile/expression/store/indirect_packed.h>
#include <ucsdpsys_compile/translator/compile.h>



expression_store_indirect_packed::~expression_store_indirect_packed()
{
}


expression_store_indirect_packed::expression_store_indirect_packed(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_store(a_rhs),
    code(*cntxt),
    lhs(a_lhs)
{
    assert(type_pointer_packed::is_a(lhs->get_type()));
}


expression_store_indirect_packed::pointer
expression_store_indirect_packed::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_store_indirect_packed(a_lhs, a_rhs, cntxt));
}


void
expression_store_indirect_packed::pre_order_traversal(void)
    const
{
    lhs->traversal(0);
}


void
expression_store_indirect_packed::post_order_traversal(void)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(STP);
}


expression::pointer
expression_store_indirect_packed::optimize(void)
    const
{
    expression::pointer e1 = lhs->optimize();
    expression::pointer e2 = get_from()->optimize();
    return create(e1, e2, &code);
}


expression::pointer
expression_store_indirect_packed::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), get_from()->clone(locn), &code);
}


rcstring
expression_store_indirect_packed::lisp_representation(void)
    const
{
    return
        (
            "(store-indirect-packed "
        +
            lhs->lisp_representation()
        +
            " "
        +
            get_from()->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
