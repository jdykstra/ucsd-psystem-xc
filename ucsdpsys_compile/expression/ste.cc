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

#include <lib/pcode.h>
#include <lib/type/nothing.h>

#include <ucsdpsys_compile/expression/ste.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ste::~expression_ste()
{
}


expression_ste::expression_ste(
    const location &a_locn,
    int a_segnum,
    int a_offset,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression(a_locn),
    code(*cntxt),
    segnum(a_segnum),
    offset(a_offset),
    rhs(a_rhs)
{
    assert(offset > 0);
    assert(offset < 32768);
}


expression_ste::pointer
expression_ste::create(const location &a_locn, int a_segnum, int a_offset,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return
        pointer(new expression_ste(a_locn, a_segnum, a_offset, a_rhs, cntxt));
}


void
expression_ste::traversal(int)
    const
{
    rhs->traversal(0);
    code.breakpoint(get_location());
    code.emit_byte(STE);
    code.emit_byte(segnum);
    code.emit_big(offset);
}


expression::pointer
expression_ste::optimize(void)
    const
{
    return create(get_location(), segnum, offset, rhs->optimize(), &code);
}


expression::pointer
expression_ste::clone(const location &locn)
    const
{
    return create(locn, segnum, offset, rhs->clone(locn), &code);
}


int
expression_ste::get_precedence(void)
    const
{
    return prec_assign;
}


type::pointer
expression_ste::get_type(void)
    const
{
    return type_nothing::create();
}


bool
expression_ste::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_ste::has_side_effect(void)
    const
{
    return side_effect_yes;
}


rcstring
expression_ste::lisp_representation(void)
    const
{
    return
        (
            rcstring::printf("(ste %d %d ", segnum, offset)
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
