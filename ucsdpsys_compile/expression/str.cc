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

#include <ucsdpsys_compile/expression/str.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_str::~expression_str()
{
}


expression_str::expression_str(
    const location &a_locn,
    unsigned a_down,
    int a_offset,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression(a_locn),
    code(*cntxt),
    down(a_down),
    offset(a_offset),
    rhs(a_rhs)
{
    assert(down > 0);
    assert(down < 256);
    assert(offset >= -32768);
    assert(offset < 32768);
}


expression_str::pointer
expression_str::create(const location &a_locn, unsigned a_down, int a_offset,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_str(a_locn, a_down, a_offset, a_rhs, cntxt));
}


void
expression_str::traversal(int)
    const
{
    rhs->traversal(0);
    code.breakpoint(get_location());
    code.emit_byte(STR);
    code.emit_byte(down);
    code.emit_big(offset);
}


expression::pointer
expression_str::optimize(void)
    const
{
    return create(get_location(), down, offset, rhs->optimize(), &code);
}


expression::pointer
expression_str::clone(const location &locn)
    const
{
    return create(locn, down, offset, rhs->clone(locn), &code);
}


void
expression_str::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"str is never bool");
}


int
expression_str::get_precedence()
    const
{
    return prec_assign;
}


type::pointer
expression_str::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_str::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_str::has_side_effect()
    const
{
    return side_effect_yes;
}


rcstring
expression_str::lisp_representation(void)
    const
{
    return
        (
            "(str "
        +
            rcstring::printf("%d", down)
        +
            " "
        +
            rcstring::printf("%d", offset)
        +
            " "
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
