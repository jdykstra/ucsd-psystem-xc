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

#include <lib/label.h>
#include <lib/pcode.h>
#include <lib/type/boolean.h>
#include <ucsdpsys_compile/expression/lod.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_lod::~expression_lod()
{
}


expression_lod::expression_lod(
    const location &a_locn,
    const type::pointer &a_type,
    unsigned a_down,
    int a_offset,
    translator_compile *cntxt
) :
    expression(a_locn),
    code(*cntxt),
    ptype(a_type),
    down(a_down),
    offset(a_offset)
{
    assert(down > 0);
    assert(down < 256);
    assert(offset >= -32768);
    assert(offset < 32768);
}


expression_lod::pointer
expression_lod::create(const location &a_locn, const type::pointer &a_type,
    unsigned a_down, int a_offset, translator_compile *cntxt)
{
    return pointer(new expression_lod(a_locn, a_type, a_down, a_offset, cntxt));
}


void
expression_lod::traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(LOD);
    code.emit_byte(down);
    code.emit_big(offset);
}


expression::pointer
expression_lod::optimize(void)
    const
{
    return create(get_location(), ptype, down, offset, &code);
}


expression::pointer
expression_lod::clone(const location &locn)
    const
{
    return create(locn, ptype, down, offset, &code);
}


void
expression_lod::logical_traversal(int pprec, label::pointer &true_label,
    label::pointer &false_label, bool fall_through_preference) const
{
    assert(type_boolean::is_a(get_type()));
    traversal(pprec);
    if (fall_through_preference)
    {
        false_label->branch_from_here_if_false();
    }
    else
    {
        true_label->branch_from_here_if_true();
    }
}


int
expression_lod::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_lod::get_type()
    const
{
    return ptype;
}


bool
expression_lod::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_lod::has_side_effect()
    const
{
    return side_effect_no;
}


rcstring
expression_lod::lisp_representation(void)
    const
{
    return rcstring::printf("(lod %d %d)", down, offset);
}


// vim: set ts=8 sw=4 et :
