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

#include <lib/debug.h>
#include <lib/label.h>
#include <lib/pcode.h>
#include <lib/type/boolean.h>
#include <ucsdpsys_compile/expression/ldo_publref.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ldo_publref::~expression_ldo_publref()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_ldo_publref::expression_ldo_publref(
    const location &a_locn,
    const type::pointer &a_type,
    const symbol::pointer &a_sym,
    translator_compile *a_context
) :
    expression(a_locn),
    ptype(a_type),
    sym(a_sym),
    context(a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(sym);
}


expression_ldo_publref::pointer
expression_ldo_publref::create(const location &a_locn,
    const type::pointer &a_type, const symbol::pointer &a_sym,
    translator_compile *a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return
        pointer(new expression_ldo_publref(a_locn, a_type, a_sym, a_context));
}


void
expression_ldo_publref::traversal(int)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    context->breakpoint(get_location());
    context->emit_byte(LDO);
    context->emit_big_publref(sym->get_name());
    DEBUG(1, "}");
}


expression::pointer
expression_ldo_publref::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_location(), ptype, sym, context);
}


expression::pointer
expression_ldo_publref::clone(const location &locn)
    const
{
    return create(locn, ptype, sym, context);
}


void
expression_ldo_publref::logical_traversal(int pprec, label::pointer &true_label,
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
expression_ldo_publref::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_ldo_publref::get_type()
    const
{
    return ptype;
}


bool
expression_ldo_publref::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_ldo_publref::has_side_effect()
    const
{
    return side_effect_no;
}


rcstring
expression_ldo_publref::lisp_representation(void)
    const
{
    return "(ldo-publref " + sym->get_name().quote_c() + ")";
}


// vim: set ts=8 sw=4 et :
