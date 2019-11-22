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

#include <ucsdpsys_compile/expression/mov.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_mov::~expression_mov()
{
}


expression_mov::expression_mov(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *a_code
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs),
    code(*a_code)
{
    assert(lhs->get_type()->get_subtype()->get_size_in_words() > 1);
}


expression_mov::pointer
expression_mov::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *a_code)
{
    return pointer(new expression_mov(a_lhs, a_rhs, a_code));
}


void
expression_mov::traversal(int pprec)
    const
{
    lhs->traversal(pprec);
    rhs->traversal(pprec);
    code.breakpoint(get_location());
    code.emit_byte(MOV);
    int nwords = lhs->get_type()->get_subtype()->get_size_in_words();
    assert(nwords > 0);
    assert(nwords < 32768);
    code.emit_big(nwords);
}


void
expression_mov::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"don't be silly");
}


int
expression_mov::get_precedence()
    const
{
    return prec_assign;
}


type::pointer
expression_mov::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_mov::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_mov::has_side_effect()
    const
{
    return side_effect_yes;
}


expression::pointer
expression_mov::optimize(void)
    const
{
    return create(lhs->optimize(), rhs->optimize(), &code);
}


expression::pointer
expression_mov::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn), &code);
}


rcstring
expression_mov::lisp_representation(void)
    const
{
    return
        (
            "(mov "
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
