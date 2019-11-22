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
#include <lib/type/pointer.h>

#include <ucsdpsys_compile/expression/address.h>
#include <ucsdpsys_compile/expression/load/indirect.h>
#include <ucsdpsys_compile/expression/store/indirect.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_store_indirect::~expression_store_indirect()
{
}


expression_store_indirect::expression_store_indirect(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_store(a_rhs),
    code(*cntxt),
    lhs(a_lhs)
{
    DEBUG(2, "lhs = %s", a_lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", a_rhs->lisp_representation().c_str());
    assert(lhs->get_type()->get_subtype() == get_from()->get_type());
}


expression_store_indirect::pointer
expression_store_indirect::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_store_indirect(a_lhs, a_rhs, cntxt));
}


void
expression_store_indirect::pre_order_traversal()
    const
{
    lhs->traversal(0);
}


void
expression_store_indirect::post_order_traversal()
    const
{
    code.breakpoint(get_location());
    assert(type_pointer::is_a(lhs->get_type()));
    assert(lhs->get_type()->get_subtype() == get_from()->get_type());
    unsigned size = lhs->get_type()->get_subtype()->get_size_in_words();
    assert(size >= 1);
    if (size == 1)
    {
        code.emit_byte(STO);
    }
    else
    {
        assert(size < 256);
        code.emit_byte(STM);
        code.emit_big(size);
    }
}


expression::pointer
expression_store_indirect::optimize(void)
    const
{
    expression::pointer o_lhs = lhs->optimize();
    expression::pointer rhs = get_from();
    expression::pointer o_rhs = rhs->optimize();

    //
    // If we have
    //    (store_indirect lhs-addr (load_indirect rhs-addr))
    // turn it into
    //    (mov lhs-addr rhs-addr)
    //
    // Somewhat arbitrarily, we limit this optimization to things larger
    // than a real.
    //
    if (o_lhs->get_type()->get_subtype()->get_size_in_words() >= 2)
    {
        expression_load_indirect::pointer rhs2 =
            boost::dynamic_pointer_cast<expression_load_indirect>(o_rhs);
        if (rhs2)
        {
            expression::pointer ep = rhs2->mov_factory(o_lhs);
            return ep->optimize();
        }
    }

    //
    // Rather than grope the left hand side, we let each class of left
    // hand side take care of it themselves.
    //
    {
        expression_address::pointer eap =
            boost::dynamic_pointer_cast<expression_address>(o_lhs);
        if (eap)
        {
            expression::pointer ep = eap->fast_store(o_rhs);
            if (ep)
            {
                return ep;
            }
        }
    }

    return create(o_lhs, o_rhs, &code);
}


expression::pointer
expression_store_indirect::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), get_from()->clone(locn), &code);
}


rcstring
expression_store_indirect::lisp_representation(void)
    const
{
    return
        (
            "(store-indirect "
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
