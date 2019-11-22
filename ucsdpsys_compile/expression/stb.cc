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
#include <lib/type/nothing.h>

#include <ucsdpsys_compile/expression/stb.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_stb::~expression_stb()
{
}


expression_stb::expression_stb(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *a_code
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs),
    code(*a_code)
{
}


expression_stb::pointer
expression_stb::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *a_code)
{
    return pointer(new expression_stb(a_lhs, a_rhs, a_code));
}


void
expression_stb::traversal(int)
    const
{
    lhs->traversal(0);
    rhs->traversal(0);

    //
    // "Store byte.  Index byte pointer tos-2 by the integer index
    // tos-1, and push the byte tos into the location pointered to by
    // the resulting byte pointer."
    //
    code.breakpoint(get_location());
    code.emit_byte(STB);
}


void
expression_stb::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"can't ever be boolean");
}


int
expression_stb::get_precedence(void)
    const
{
    return prec_assign;
}


type::pointer
expression_stb::get_type(void)
    const
{
    return type_nothing::create();
}


bool
expression_stb::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_stb::has_side_effect(void)
    const
{
    return side_effect_yes;
}


expression::pointer
expression_stb::optimize(void)
    const
{
    return create(lhs->optimize(), rhs->optimize(), &code);
}


expression::pointer
expression_stb::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn), &code);
}


rcstring
expression_stb::lisp_representation(void)
    const
{
    return
        (
            "(stb "
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
