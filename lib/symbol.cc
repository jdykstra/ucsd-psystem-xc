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
#include <lib/pascal/lex.h>
#include <lib/symbol.h>
#include <lib/type/integer.h>
#include <lib/variable/name.h>


symbol::~symbol()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol::symbol(
    const variable_name &a_name,
    const type::pointer &a_stype,
    int a_lex_level
) :
    name(a_name.get_name()),
    locn(a_name.get_location()),
    stype(a_stype),
    packed(false),
    lex_level(a_lex_level)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol::symbol(
    const rcstring &a_name,
    const type::pointer &a_type,
    int a_lex_level
) :
    name(a_name),
    locn(pascal_lex_location()),
    stype(a_type),
    packed(false),
    lex_level(a_lex_level)
{
}


symbol::symbol(
    const rcstring &a_name,
    const location &dec_loc,
    const type::pointer &a_type,
    int a_lex_level
) :
    name(a_name),
    locn(dec_loc),
    stype(a_type),
    packed(false),
    lex_level(a_lex_level)
{
}


void
symbol::set_bit_offset(const bit_address &new_bit_offset, bool new_packed)
{
    bit_offset = new_bit_offset;
    packed = new_packed && (get_type()->get_size_in_bits() < 16);
    assert(packed || bit_offset.on_word_boundary());
}


void
symbol::add_reference(const location &loc, const rcstring &arg)
{
    add_reference(variable_name(arg, loc));
}


void
symbol::add_reference(const variable_name &)
{
    // Do nothing by default.
}


unsigned
symbol::get_size_in_bytes()
    const
{
    return stype->get_size_in_bytes();
}


unsigned
symbol::get_size_in_bits()
    const
{
    return stype->get_size_in_bits();
}


void
symbol::change_type(const type::pointer &tp)
{
    stype = tp;
}


bool
symbol::is_packed()
    const
{
    assert(packed || bit_offset.on_word_boundary());
    return packed;
}


void
symbol::type_late_binding()
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    stype = type::late_binding(stype);
    DEBUG(1, "}");
}


void
symbol::check_defined(void)
{
    if (!need_late_binding())
        return;

    pascal_lex_error
    (
        get_declaration_location(),
        "%s %s not completely defined",
        get_kind(),
        name.quote_c().c_str()
    );
    stype = type_integer::create();
}


bool
symbol::need_late_binding(void)
    const
{
    return stype->need_late_binding();
}


#include <lib/expression/error.h>


expression::pointer
symbol::name_expression_factory(const symbol::pointer &sp,
    const location &where, int)
{
    assert(this == sp.get());
    pascal_lex_error
    (
        where,
        "unable to reference symbol %s which is of %s type",
        get_name().quote_c().c_str(),
        get_type()->get_name().quote_c().c_str()
    );
    return expression_error::create(where);
}


// vim: set ts=8 sw=4 et :
