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

#include <ucsdpsys_compile/expression/address/external.h>
#include <ucsdpsys_compile/expression/lde.h>
#include <ucsdpsys_compile/expression/ste.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_address_external::~expression_address_external()
{
}


expression_address_external::expression_address_external(
    const location &a_locn,
    const type::pointer &a_ptype,
    int a_segnum,
    int a_offset,
    translator_compile *a_context
) :
    expression_address(a_locn, a_offset, a_ptype),
    segnum(a_segnum),
    context(a_context)
{
    assert(segnum >= 0);
    assert(segnum < 256);
}


expression_address_external::pointer
expression_address_external::create(const location &a_locn,
    const type::pointer &a_ptype, int a_segnum, int a_offset,
    translator_compile *a_context)
{
    return
        pointer
        (
            new expression_address_external
            (
                a_locn,
                a_ptype,
                a_segnum,
                a_offset,
                a_context
            )
        );
}


void
expression_address_external::traversal(int)
    const
{
    context->emit_byte(LAE);
    context->emit_byte(segnum);
    context->emit_big(get_offset());
}


expression::pointer
expression_address_external::optimize(void)
    const
{
    return create(get_location(), get_type(), segnum, get_offset(), context);
}


expression::pointer
expression_address_external::clone(const location &where)
    const
{
    return create(where, get_type(), segnum, get_offset(), context);
}


rcstring
expression_address_external::lisp_representation(void)
    const
{
    return rcstring::printf("(address-external %d %d)", segnum, get_offset());
}


expression::pointer
expression_address_external::fast_load(void)
    const
{
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    return
        expression_lde::create
        (
            get_location(),
            get_type()->get_subtype(),
            segnum,
            get_offset(),
            context
        );
}


expression::pointer
expression_address_external::fast_store(const expression::pointer &rhs)
    const
{
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    return
        expression_ste::create
        (
            get_location(),
            segnum,
            get_offset(),
            rhs,
            context
        );
}


expression_address::pointer
expression_address_external::advance_by_words(long nwords,
    const type::pointer &tp, const symbol::pointer &) const
{
    return create(get_location(), tp, segnum, get_offset() + nwords, context);
}


expression::pointer
expression_address_external::cast(const type::pointer &tp)
    const
{
    return create(get_location(), tp, segnum, get_offset(), context);
}


// vim: set ts=8 sw=4 et :
