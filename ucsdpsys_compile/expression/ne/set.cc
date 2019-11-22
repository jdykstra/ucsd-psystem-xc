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

#include <lib/ac/assert.h>

#include <lib/debug.h>
#include <lib/label.h>
#include <lib/pcode.h>
#include <lib/type/anything.h>
#include <lib/type/integer.h>
#include <lib/type/set/sized.h>
#include <lib/type/set/unsized.h>
#include <lib/type/subrange.h>

#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/eq/set.h>
#include <ucsdpsys_compile/expression/ne/set.h>
#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/expression/set2set/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ne_set::~expression_ne_set()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_ne_set::expression_ne_set(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_ne(a_lhs, a_rhs),
    code(*cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "lhs %s", a_lhs->get_type()->get_name().c_str());
    DEBUG(1, "rhs %s", a_rhs->get_type()->get_name().c_str());
}


expression_ne_set::pointer
expression_ne_set::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression::pointer lhs2 = a_lhs;
    expression::pointer rhs2 = a_rhs;
    cntxt->fix_binary_set_operands(lhs2, rhs2);

    pointer result(new expression_ne_set(lhs2, rhs2, cntxt));
    DEBUG(1, "}");
    return result;
}


void
expression_ne_set::post_order_traversal(int)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    code.breakpoint(get_location());
    code.emit_byte(NEQ);
    code.emit_byte(OP_CMP_SET);
}


expression::pointer
expression_ne_set::optimize(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();

    assert(e1->get_type() == e2->get_type());
    if (e1->is_constant() && e2->is_constant())
    {
        expression_set_compile::pointer lesp =
            boost::dynamic_pointer_cast<expression_set_compile>(e1);
        assert(lesp);
        expression_set_compile::pointer resp =
            boost::dynamic_pointer_cast<expression_set_compile>(e2);
        assert(resp);
        bool result = (lesp->get_value() != resp->get_value());
        expression::pointer ep =
            expression_boolean_compile::create(get_location(), result, &code);
        DEBUG(1, "}");
        return ep;
    }

    expression::pointer result = expression_ne_set::create(e1, e2, &code);
    DEBUG(1, "}");
    return result;
}


expression::pointer
expression_ne_set::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


void
expression_ne_set::logical_traversal(int pprec,
    label::pointer &true_branch, label::pointer &false_branch,
    bool fall_through_preference) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (fall_through_preference)
    {
        // code gen positive, FJP to false_branch
        traversal(pprec);
        false_branch->branch_from_here_if_false();
    }
    else
    {
        // code gen negative, FJP to true_branch
        expression::pointer nep =
            expression_eq_set::create(get_lhs(), get_rhs(), &code);
        nep->traversal(pprec);
        true_branch->branch_from_here_if_false();
    }
    DEBUG(1, "}");
}


// vim: set ts=8 sw=4 et :
