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

#include <ucsdpsys_compile/expression/stl.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_stl::~expression_stl()
{
}


expression_stl::expression_stl(const expression_stl &arg) :
    expression(arg),
    code(arg.code),
    offset(arg.offset),
    rhs(arg.rhs)
{
}


expression_stl::expression_stl(
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
    assert(offset >= -32768);
    assert(offset < 32768);
}


expression_stl::pointer
expression_stl::create(const location &a_locn, int a_offset,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_stl(a_locn, a_offset, a_rhs, cntxt));
}


void
expression_stl::traversal(int)
    const
{
    rhs->traversal(0);
    code.breakpoint(get_location());
    code.emit_byte(STL);
    code.emit_big(offset);
}


expression::pointer
expression_stl::optimize(void)
    const
{
    return create(get_location(), offset, rhs, &code);
}


expression::pointer
expression_stl::clone(const location &locn)
    const
{
    return create(locn, offset, rhs->clone(locn), &code);
}


void
expression_stl::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"stl is void");
}


int
expression_stl::get_precedence()
    const
{
    return prec_assign;
}


type::pointer
expression_stl::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_stl::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_stl::has_side_effect()
    const
{
    return side_effect_yes;
}


rcstring
expression_stl::lisp_representation(void)
    const
{
    return
        (
            "(stl "
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
