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

#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/set/sized.h>
#include <lib/type/subrange.h>
#include <ucsdpsys_compile/expression/set_intersection.h>
#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_set_intersection::~expression_set_intersection()
{
}


expression_set_intersection::expression_set_intersection(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    code(*cntxt),
    lhs(a_lhs),
    rhs(a_rhs)
{
    assert(type_set_sized::is_a(lhs->get_type()));
    type::pointer t1 = lhs->get_type()->get_subtype();
    if (type_subrange::is_a(t1))
        t1 = t1->get_subtype();

    assert(type_set_sized::is_a(rhs->get_type()));
    type::pointer t2 = rhs->get_type()->get_subtype();
    if (type_subrange::is_a(t2))
        t2 = t2->get_subtype();

    if (t1 != t2)
    {
        pascal_lex_error
        (
            get_location(),
            "invalid set intersection, given a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
    }
}


expression_set_intersection::pointer
expression_set_intersection::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    expression::pointer lhs2 = a_lhs;
    expression::pointer rhs2 = a_rhs;
    cntxt->fix_binary_set_operands(lhs2, rhs2);
    return pointer(new expression_set_intersection(lhs2, rhs2, cntxt));
}


void
expression_set_intersection::traversal(int)
    const
{
    lhs->traversal(0);
    rhs->traversal(0);
    code.breakpoint(get_location());
    code.emit_byte(INT);
}


expression::pointer
expression_set_intersection::optimize(void)
    const
{
    expression::pointer lop = lhs->optimize();
    assert(type_set_sized::is_a(lop->get_type()));
    expression::pointer rop = rhs->optimize();
    assert(type_set_sized::is_a(rop->get_type()));
    if (lop->is_constant() && rop->is_constant())
    {
        expression_set_compile *lesp =
            dynamic_cast<expression_set_compile *>(lop.operator->());
        assert(lesp);
        expression_set_compile *resp =
            dynamic_cast<expression_set_compile *>(rop.operator->());
        assert(resp);
        set s(lesp->get_value() * resp->get_value());
        location locn(lhs->get_location() + rhs->get_location());
        type::pointer nt =
            expression_set_compile::minimum_subrange(lhs->get_type(), s);
        return expression_set_compile::create(locn, nt, s, &code);
    }
    if (lop->is_empty_set())
        return lop;
    if (rop->is_empty_set())
        return rop;
    return expression_set_intersection::create(lop, rop, &code);
}


expression::pointer
expression_set_intersection::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn), &code);
}


type::pointer
expression_set_intersection::get_type()
    const
{
    return lhs->get_type();
}


void
expression_set_intersection::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"set intersection is never bool");
}


int
expression_set_intersection::get_precedence()
    const
{
    return prec_add;
}


bool
expression_set_intersection::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_set_intersection::has_side_effect()
    const
{
    return (side_effect_no + lhs->has_side_effect() + rhs->has_side_effect());
}


rcstring
expression_set_intersection::lisp_representation(void)
    const
{
    return
        (
            "(set-intersection "
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


// vim: set ts=8 sw=4 et :
