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

#include <ucsdpsys_compile/expression/address/intermediate.h>
#include <ucsdpsys_compile/expression/lod.h>
#include <ucsdpsys_compile/expression/str.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_address_intermediate::~expression_address_intermediate()
{
}


expression_address_intermediate::expression_address_intermediate(
    const expression_address_intermediate &arg
) :
    expression_address(arg),
    code(arg.code),
    down(arg.down),
    hint(arg.hint)
{
}


expression_address_intermediate::expression_address_intermediate(
    const location &a_locn,
    int a_offset,
    const type::pointer &a_type,
    unsigned a_down,
    const symbol::pointer &a_hint,
    translator_compile *cntxt
) :
    expression_address(a_locn, a_offset, a_type),
    code(*cntxt),
    down(a_down),
    hint(a_hint)
{
    assert(down > 0);
    assert(down < 256);
    assert(a_offset >= -32768);
    assert(a_offset < 32768);
}


expression_address_intermediate::pointer
expression_address_intermediate::create(const location &a_locn,
    int a_offset, const type::pointer &a_type, unsigned a_down,
    const symbol::pointer &a_hint, translator_compile *cntxt)
{
    return
        pointer
        (
            new expression_address_intermediate
            (
                a_locn,
                a_offset,
                a_type,
                a_down,
                a_hint,
                cntxt
            )
        );
}


expression::pointer
expression_address_intermediate::cast(const type::pointer &a_type)
    const
{
    assert(type_pointer::is_a(a_type));
    return
        create
        (
            get_location(),
            get_offset(),
            a_type,
            down,
            hint,
            &code
        );
}


void
expression_address_intermediate::traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(LDA);
    assert(down > 0);
    assert(down < 256);
    code.emit_byte(down);
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    code.emit_big(get_offset());
}


expression::pointer
expression_address_intermediate::fast_load()
    const
{
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    assert(down > 0);
    assert(down < 256);
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    return
        expression_lod::create
        (
            get_location(),
            get_type()->get_subtype(),
            down,
            get_offset(),
            &code
        );
}


expression::pointer
expression_address_intermediate::fast_store(const expression::pointer &rhs)
    const
{
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    assert(down > 0);
    assert(down < 256);
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    return
        expression_str::create
        (
            get_location(),
            down,
            get_offset(),
            rhs,
            &code
        );
}


bool
expression_address_intermediate::is_constant()
    const
{
    //
    // The pointer can be constant without the
    // value being pointed to being constant.
    //
    return true;
}


expression_address::pointer
expression_address_intermediate::advance_by_words(long nwords,
        const type::pointer &new_type, const symbol::pointer &new_hint)
    const
{
    long new_offset = get_offset() + nwords;
    if (new_offset <= 0)
        return expression_address::pointer();
    if (new_offset >= 32768)
        return expression_address::pointer();
    return
        expression_address_intermediate::create
        (
            get_location(),
            new_offset,
            new_type,
            down,
            (new_hint ? new_hint : hint),
            &code
        );
}


symbol::pointer
expression_address_intermediate::get_symbol_hint()
    const
{
    return hint;
}


expression::pointer
expression_address_intermediate::clone(const location &locn)
    const
{
    return create(locn, get_offset(), get_type(), down, hint, &code);
}


expression::pointer
expression_address_intermediate::optimize()
    const
{
    return pointer(new expression_address_intermediate(*this));
}


rcstring
expression_address_intermediate::lisp_representation(void)
    const
{
    return rcstring::printf("(address-intermediate %d %d)", down, get_offset());
}


// vim: set ts=8 sw=4 et :
