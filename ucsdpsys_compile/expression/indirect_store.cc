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
#include <lib/type/boolean.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/nothing.h>
#include <lib/type/real.h>
#include <lib/type/string.h>
#include <ucsdpsys_compile/expression/indirect_store.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_indirect_store::~expression_indirect_store()
{
}


expression_indirect_store::expression_indirect_store(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    const type::pointer &a_type,
    translator_compile *cntxt
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    code(*cntxt),
    lhs(a_lhs),
    rhs(a_rhs),
    storing_type(a_type)
{
}


expression_indirect_store::pointer
expression_indirect_store::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, const type::pointer &a_type,
    translator_compile *cntxt)
{
    return pointer(new expression_indirect_store(a_lhs, a_rhs, a_type, cntxt));
}


void
expression_indirect_store::traversal(int)
    const
{
    code.breakpoint(get_location());
    lhs->traversal(0);
    rhs->traversal(0);
    if
    (
        type_boolean::is_a(storing_type)
    ||
        type_enumerated::is_a(storing_type)
    ||
        type_integer::is_a(storing_type)
    )
    {
        code.emit_byte(STO);
    }
    else if (type_real::is_a(storing_type))
    {
        code.emit_byte(STM);
        code.emit_byte(2);
    }
    else if (type_string::is_a(storing_type))
    {
        code.emit_byte(SAS);
        unsigned len = storing_type->get_size_in_bits();
        assert(len >= 16);
        len = (len >> 3) - 1;
        assert(len < 256);
        code.emit_byte(len);
    }
    else
    {
        assert(0);
        code.error
        (
            get_location(),
            "unable to generate code for indirect store of %s type",
            storing_type->get_name().c_str()
        );
    }
}


int
expression_indirect_store::get_precedence()
    const
{
    return 0;
}


type::pointer
expression_indirect_store::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_indirect_store::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_indirect_store::has_side_effect()
    const
{
    return side_effect_yes;
}


void
expression_indirect_store::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"stores are void, not bool");
}


expression::pointer
expression_indirect_store::optimize(void)
    const
{
    expression::pointer e1 = lhs->optimize();
    expression::pointer e2 = rhs->optimize();
    return expression_indirect_store::create(e1, e2, storing_type, &code);
}


expression::pointer
expression_indirect_store::clone(const location &locn)
    const
{
    expression::pointer e1 = lhs->clone(locn);
    expression::pointer e2 = rhs->clone(locn);
    return create(e1, e2, storing_type, &code);
}


rcstring
expression_indirect_store::lisp_representation(void)
    const
{
    return
        (
            "(indirect-store "
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
