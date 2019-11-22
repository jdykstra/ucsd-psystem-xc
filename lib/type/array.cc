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
#include <lib/ac/string.h>

#include <lib/anonymous_name.h>
#include <lib/type/array.h>
#include <lib/type/char.h>
#include <lib/type/subrange.h>


type_array::~type_array()
{
}


type_array::type_array(
    bool a_packed,
    const type::pointer &a_index_type,
    const type::pointer &a_subtype
) :
    packed(a_packed),
    name(anonymous_name()),
    index_type(a_index_type),
    subtype(a_subtype)
{
    assert(subtype->get_size_in_bits() > 0);
    assert(!packed || subtype->get_size_in_bits() <= 8);
}


type_array::pointer
type_array::create(bool a_packed, const type::pointer &a_index_type,
    const type::pointer &a_subtype)
{
    return pointer(new type_array(a_packed, a_index_type, a_subtype));
}


rcstring
type_array::get_name()
    const
{
    if (strchr(name.c_str(), ':'))
        return get_pascal_name();
    return name;
}


rcstring
type_array::get_pascal_name()
    const
{
    return
        (
            rcstring(packed ? "packed " : "")
        +
            "array ["
        +
            index_type->get_pascal_name()
        +
            "] of "
        +
            subtype->get_pascal_name()
        );
}


rcstring
type_array::get_c_declaration(const rcstring &cname)
    const
{
    return
        (
            get_subtype()->get_c_declaration(cname)
        +
            "["
        +
            index_type->get_pascal_name()
        +
            "]"
        );
}


void
type_array::name_hint(const rcstring &suggestion)
{
    if (strchr(name.c_str(), ':'))
    {
        name =
            (
                rcstring(packed ? "packed " : "")
            +
                "array "
            +
                suggestion
            +
                rcstring::printf("(%d)", get_width())
            +
                " of "
            +
                subtype->get_name()
            );
    }
}


type::pointer
type_array::get_subtype()
    const
{
    return subtype;
}


unsigned
type_array::get_size_in_words()
    const
{
    assert(!packed || subtype->get_size_in_bits() <= 8);
    assert(index_type->may_be_used_as_array_index());
    int hi = index_type->get_maximum_value() + 1;
    int lo = index_type->get_minimum_value();
    int nelem = hi - lo;
    assert(nelem > 0);

    if (packed)
    {
        assert(subtype->get_size_in_bits() > 0);
        assert(subtype->get_size_in_bits() <= 8);
        unsigned elements_per_word = 16 / subtype->get_size_in_bits();
        assert(elements_per_word >= 1);
        return (nelem + elements_per_word - 1) / elements_per_word;
    }

    assert(subtype->get_size_in_words() > 0);
    return nelem * subtype->get_size_in_words();
}


unsigned
type_array::get_size_in_bits()
    const
{
    return (get_size_in_words() << 4);
}


bool
type_array::same_type_as(const type::pointer &rhs)
    const
{
    const type_array *tap = dynamic_cast<const type_array *>(rhs.get());
    if (!tap)
        return false;
    return
        (
            index_type == tap->get_index_type()
        &&
            packed == tap->packed
        &&
            get_width() == tap->get_width()
        &&
            subtype == tap->get_subtype()
        );
}


bool
type_array::is_a_variable()
    const
{
    return true;
}


bool
type_array::is_a(const type::pointer &arg)
{
    return (0 != dynamic_cast<const type_array *>(arg.operator->()));
}


bool
type_array::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


type::pointer
type_array::late_binding_or_null()
    const
{
    return
        type_array::create
        (
            packed,
            type::late_binding(index_type),
            type::late_binding(subtype)
        );
}


bool
type_array::need_late_binding(void)
    const
{
    return (index_type->need_late_binding() || subtype->need_late_binding());
}


bool
type_array::is_packed_array_of_char()
    const
{
    return (packed && type_char::is_a(subtype));
}


unsigned
type_array::get_width(void)
    const
{
    return
        (
            index_type->get_maximum_value()
        +
            1
        -
            index_type->get_minimum_value()
        );
}


rcstring
type_array::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :
