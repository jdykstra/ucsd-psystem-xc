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

#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/pointer.h>

#include <ucsdpsys_compile/expression/address/global.h>
#include <ucsdpsys_compile/expression/ldo.h>
#include <ucsdpsys_compile/expression/sro.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_address_global::~expression_address_global()
{
}


expression_address_global::expression_address_global(
    const expression_address_global &arg
) :
    expression_address(arg),
    code(arg.code),
    hint(arg.hint)
{
}


static int
limit(int value, int lo, int hi)
{
    if (value < lo)
        return lo;
    if (value > hi)
        return hi;
    return value;
}


expression_address_global::expression_address_global(
    const location &a_locn,
    int a_offset,
    const type::pointer &a_type,
    const symbol::pointer &a_hint,
    translator_compile *cntxt
) :
    expression_address(a_locn, limit(a_offset, 0, 32767), a_type),
    code(*cntxt),
    hint(a_hint)
{
    if (a_offset < 0 || a_offset >= 32768)
    {
        pascal_lex_error
        (
            a_locn,
            "global address offset %d out of range (0..32767)",
            a_offset
        );
    }
}


expression_address_global::pointer
expression_address_global::create(const location &a_locn, int a_offset,
    const type::pointer &a_type, const symbol::pointer &a_hint,
    translator_compile *cntxt)
{
    return
        pointer
        (
            new expression_address_global
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
expression_address_global::cast(const type::pointer &a_type)
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
expression_address_global::traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(LAO);
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    code.emit_big(get_offset());
}


expression::pointer
expression_address_global::fast_load(void)
    const
{
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    assert(get_offset() > 0);
    assert(get_offset() < 32768);
    return
        expression_ldo::create
        (
            get_location(),
            get_type()->get_subtype(),
            get_offset(),
            &code
        );
}


expression::pointer
expression_address_global::fast_store(const expression::pointer &rhs)
    const
{
    if (get_type()->get_subtype()->get_size_in_words() != 1)
        return expression::pointer();
    return expression_sro::create(get_location(), get_offset(), rhs, &code);
}


bool
expression_address_global::is_constant(void)
    const
{
    //
    // The pointer can be constant without the
    // value being pointed to being constant.
    //
    return true;
}


expression_address::pointer
expression_address_global::advance_by_words(long nwords,
    const type::pointer &new_type, const symbol::pointer &new_hint) const
{
    long new_offset = get_offset() + nwords;
    if (new_offset <= 0)
        return expression_address::pointer();
    if (new_offset >= 32768)
        return expression_address::pointer();
    return
        expression_address_global::create
        (
            get_location(),
            new_offset,
            new_type,
            new_hint,
            &code
        );
}


symbol::pointer
expression_address_global::get_symbol_hint(void)
    const
{
    return hint;
}


expression::pointer
expression_address_global::clone(const location &locn)
    const
{
    return create(locn, get_offset(), get_type(), hint, &code);
}


expression::pointer
expression_address_global::optimize(void)
    const
{
    return pointer(new expression_address_global(*this));
}


rcstring
expression_address_global::lisp_representation(void)
    const
{
    return
        rcstring::printf
        (
            "(address-global {%s} %u)",
            get_type()->get_pascal_name().c_str(),
            get_offset()
        );
}


// vim: set ts=8 sw=4 et :
