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
#include <lib/type/nothing.h>
#include <lib/type/pointer.h>
#include <lib/type/string.h>
#include <ucsdpsys_compile/expression/sas.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_sas::~expression_sas()
{
}


expression_sas::expression_sas(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    code(*cntxt),
    lhs(a_lhs),
    rhs(a_rhs)
{
}


expression_sas::pointer
expression_sas::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_sas(a_lhs, a_rhs, cntxt));
}


void
expression_sas::traversal(int)
    const
{
    lhs->traversal(0);
    rhs->traversal(0);

    code.breakpoint(get_location());
    code.emit_byte(SAS);
    assert(type_pointer::is_a(lhs->get_type()));
    assert(type_string::is_a(lhs->get_type()->get_subtype()));
    unsigned len = lhs->get_type()->get_subtype()->get_size_in_bits() >> 3;
    assert(len >= 2);
    --len;
    assert(len < 256);
    code.emit_byte(len);
}


void
expression_sas::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"sas is not bool");
}


int
expression_sas::get_precedence()
    const
{
    return prec_assign;
}


type::pointer
expression_sas::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_sas::is_lvalue()
    const
{
    return false;
}


expression::pointer
expression_sas::optimize(void)
    const
{
    expression::pointer e1 = lhs->optimize();
    expression::pointer e2 = rhs->optimize();
    return expression_sas::create(e1, e2, &code);
}


expression::pointer
expression_sas::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn), &code);
}


expression::side_effect_t
expression_sas::has_side_effect()
    const
{
    return side_effect_yes;
}


rcstring
expression_sas::lisp_representation(void)
    const
{
    return
        (
            "(sas "
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
