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

#include <lib/debug.h>
#include <lib/pcode.h>

#include <ucsdpsys_compile/expression/address/publref.h>
#include <ucsdpsys_compile/expression/ldo_publref.h>
#include <ucsdpsys_compile/expression/sro_publref.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_address_publref::~expression_address_publref()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_address_publref::expression_address_publref(
    const location &a_locn,
    const type::pointer &a_type,
    const symbol::pointer &a_sym,
    translator_compile *a_context
) :
    expression_address(a_locn, 0, a_type),
    sym(a_sym),
    context(a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(sym);
}


expression_address_publref::pointer
expression_address_publref::create(const location &a_locn,
    const type::pointer &a_type, const symbol::pointer &a_sym,
    translator_compile *a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return
        pointer
        (
            new expression_address_publref(a_locn, a_type, a_sym, a_context)
        );
}


void
expression_address_publref::traversal(int)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    context->breakpoint(get_location());
    context->emit_byte(LAO);
    context->emit_big_publref(sym->get_name());
}


expression::pointer
expression_address_publref::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_location(), get_type(), sym, context);
}


expression::pointer
expression_address_publref::clone(const location &locn)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(locn, get_type(), sym, context);
}


expression::pointer
expression_address_publref::cast(const type::pointer &tp)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_location(), tp, sym, context);
}


rcstring
expression_address_publref::lisp_representation(void)
    const
{
    return "(address-publref " + sym->get_name().quote_c() + ")";
}


expression::pointer
expression_address_publref::fast_load(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    return
        expression_ldo_publref::create
        (
            get_location(),
            get_type()->get_subtype(),
            sym,
            context
        );
}


expression::pointer
expression_address_publref::fast_store(const expression::pointer &rhs)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    return expression_sro_publref::create(get_location(), sym, rhs, context);
}


expression_address::pointer
expression_address_publref::advance_by_words(long, const type::pointer &,
    const symbol::pointer &) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    // If this were to be a WORD reference, then, yes, we could
    // increase the offset, but it isn't, it's a BIG reference.
    return expression_address::pointer();
}


bool
expression_address_publref::is_constant(void)
    const
{
    //
    // The pointer can be constant without the
    // value being pointed to being constant.
    //
    return true;
}


// vim: set ts=8 sw=4 et :
