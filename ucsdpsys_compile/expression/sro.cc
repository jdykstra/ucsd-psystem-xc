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

#include <ucsdpsys_compile/expression/sro.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_sro::~expression_sro()
{
}


expression_sro::expression_sro(const expression_sro &arg) :
    expression(arg),
    code(arg.code),
    offset(arg.offset),
    rhs(arg.rhs)
{
}


expression_sro::expression_sro(
    const location &a_locn,
    int a_offset,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression(a_locn),
    code(*cntxt),
    offset(a_offset),
    rhs(a_rhs)
{
    assert(offset > 0);
    assert(offset < 32768);
}


expression_sro::pointer
expression_sro::create(const location &a_locn, int a_offset,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_sro(a_locn, a_offset, a_rhs, cntxt));
}


void
expression_sro::traversal(int)
    const
{
    rhs->traversal(0);
    code.breakpoint(get_location());
    code.emit_byte(SRO);
    code.emit_big(offset);
}


expression::pointer
expression_sro::optimize(void)
    const
{
    return create(get_location(), offset, rhs->optimize(), &code);
}


expression::pointer
expression_sro::clone(const location &locn)
    const
{
    return create(locn, offset, rhs->clone(locn), &code);
}


void
expression_sro::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"sro is never bool");
}


int
expression_sro::get_precedence()
    const
{
    return prec_assign;
}


type::pointer
expression_sro::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_sro::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_sro::has_side_effect()
    const
{
    return side_effect_yes;
}


rcstring
expression_sro::lisp_representation(void)
    const
{
    return
        (
            rcstring::printf("(sro %d ", offset)
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
