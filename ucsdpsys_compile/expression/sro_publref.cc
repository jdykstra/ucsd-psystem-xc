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

#include <ucsdpsys_compile/expression/sro_publref.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_sro_publref::~expression_sro_publref()
{
}


expression_sro_publref::expression_sro_publref(
    const location &a_locn,
    const symbol::pointer &a_sym,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression(a_locn),
    code(*cntxt),
    sym(a_sym),
    rhs(a_rhs)
{
}


expression_sro_publref::pointer
expression_sro_publref::create(const location &a_locn,
    const symbol::pointer &a_sym, const expression::pointer &a_rhs,
    translator_compile *cntxt)
{
    return pointer(new expression_sro_publref(a_locn, a_sym, a_rhs, cntxt));
}


void
expression_sro_publref::traversal(int)
    const
{
    rhs->traversal(0);
    code.breakpoint(get_location());
    code.emit_byte(SRO);
    code.emit_big_publref(sym->get_name());
}


expression::pointer
expression_sro_publref::optimize(void)
    const
{
    return create(get_location(), sym, rhs->optimize(), &code);
}


expression::pointer
expression_sro_publref::clone(const location &locn)
    const
{
    return create(locn, sym, rhs->clone(locn), &code);
}


void
expression_sro_publref::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"sro_publref is never bool");
}


int
expression_sro_publref::get_precedence()
    const
{
    return prec_assign;
}


type::pointer
expression_sro_publref::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_sro_publref::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_sro_publref::has_side_effect()
    const
{
    return side_effect_yes;
}


rcstring
expression_sro_publref::lisp_representation(void)
    const
{
    return "(sro-publref " + sym->get_name().quote_c() + ")";
}


// vim: set ts=8 sw=4 et :
