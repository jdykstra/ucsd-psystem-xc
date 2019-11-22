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

#include <lib/debug.h>
#include <lib/label.h>
#include <lib/pcode.h>
#include <lib/type/boolean.h>
#include <ucsdpsys_compile/expression/ldo.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ldo::~expression_ldo()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_ldo::expression_ldo(
    const location &a_locn,
    const type::pointer &a_type,
    int a_offset,
    translator_compile *cntxt
) :
    expression(a_locn),
    code(*cntxt),
    ptype(a_type),
    offset(a_offset)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(offset >= -32768);
    assert(offset < 32768);
}


expression_ldo::pointer
expression_ldo::create(const location &a_locn, const type::pointer &a_type,
    int a_offset, translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_ldo(a_locn, a_type, a_offset, cntxt));
}


void
expression_ldo::traversal(int)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    code.breakpoint(get_location());
    if (offset > 0 && offset <= 16)
    {
        assert(offset > 0);
        code.emit_byte(SLDO_1 - 1 + offset);
    }
    else
    {
        code.emit_byte(LDO);
        code.emit_big(offset);
    }
    DEBUG(1, "}");
}


expression::pointer
expression_ldo::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_location(), ptype, offset, &code);
}


expression::pointer
expression_ldo::clone(const location &locn)
    const
{
    return create(locn, ptype, offset, &code);
}


void
expression_ldo::logical_traversal(int pprec, label::pointer &true_label,
    label::pointer &false_label, bool fall_through_preference) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
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
    DEBUG(1, "}");
}


int
expression_ldo::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_ldo::get_type()
    const
{
    return ptype;
}


bool
expression_ldo::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_ldo::has_side_effect()
    const
{
    return side_effect_no;
}


rcstring
expression_ldo::lisp_representation(void)
    const
{
    return rcstring::printf("(ldo %d)", offset);
}


// vim: set ts=8 sw=4 et :
