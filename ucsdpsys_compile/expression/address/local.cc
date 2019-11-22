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
#include <lib/type/pointer.h>

#include <ucsdpsys_compile/expression/address/local.h>
#include <ucsdpsys_compile/expression/ldl.h>
#include <ucsdpsys_compile/expression/stl.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_address_local::~expression_address_local()
{
}


expression_address_local::expression_address_local(
    const location &a_locn,
    int a_offset,
    const type::pointer &a_type,
    const symbol::pointer &a_hint,
    translator_compile *cntxt
) :
    expression_address(a_locn, a_offset, a_type),
    code(*cntxt),
    hint(a_hint)
{
    assert(a_offset >= -32768);
    assert(a_offset < 32768);
}


expression_address_local::pointer
expression_address_local::create(const location &a_locn, int a_offset,
    const type::pointer &a_type, const symbol::pointer &a_hint,
    translator_compile *cntxt)
{
    return
        pointer
        (
            new expression_address_local
            (
                a_locn,
                a_offset,
                a_type,
                a_hint,
                cntxt
            )
        );
}


expression::pointer
expression_address_local::cast(const type::pointer &a_type)
    const
{
    assert(type_pointer::is_a(a_type));
    return
        create
        (
            get_location(),
            get_offset(),
            a_type,
            hint,
            &code
        );
}


void
expression_address_local::traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(LLA);
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    code.emit_big(get_offset());
}


expression::pointer
expression_address_local::fast_load()
    const
{
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    return
        expression_ldl::create
        (
            get_location(),
            get_type()->get_subtype(),
            get_offset(),
            &code
        );
}


expression::pointer
expression_address_local::fast_store(const expression::pointer &rhs)
    const
{
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    return expression_stl::create(get_location(), get_offset(), rhs, &code);
}


bool
expression_address_local::is_constant()
    const
{
    //
    // The pointer can be constant without the
    // value being pointed to being constant.
    //
    return true;
}


expression_address::pointer
expression_address_local::advance_by_words(long nwords,
    const type::pointer &new_type, const symbol::pointer &new_hint) const
{
    long new_offset = get_offset() + nwords;
    if (new_offset <= 0)
        return expression_address::pointer();
    if (new_offset >= 32768)
        return expression_address::pointer();
    return
        expression_address_local::create
        (
            get_location(),
            new_offset,
            new_type,
            (new_hint ? new_hint : hint),
            &code
        );
}


symbol::pointer
expression_address_local::get_symbol_hint()
    const
{
    return hint;
}


expression::pointer
expression_address_local::optimize(void)
    const
{
    return create(get_location(), get_offset(), get_type(), hint, &code);
}


expression::pointer
expression_address_local::clone(const location &locn)
    const
{
    return create(locn, get_offset(), get_type(), hint, &code);
}


rcstring
expression_address_local::lisp_representation(void)
    const
{
    return
        rcstring::printf
        (
            "(address-local {%s} %d)",
            get_type()->get_pascal_name().c_str(),
            get_offset()
        );
}


// vim: set ts=8 sw=4 et :
