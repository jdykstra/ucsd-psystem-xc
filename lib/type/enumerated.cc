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
#include <lib/type/enumerated.h>
#include <lib/type/subrange.h>


type_enumerated::~type_enumerated()
{
}


type_enumerated::type_enumerated() :
    name(anonymous_name())
{
}


type_enumerated::pointer
type_enumerated::create()
{
    return pointer(new type_enumerated());
}


rcstring
type_enumerated::get_name()
    const
{
    return "enumerated " + name;
}


rcstring
type_enumerated::get_pascal_name()
    const
{
    return name;
}


rcstring
type_enumerated::get_c_declaration(const rcstring &vname)
    const
{
    return (name.downcase() + " " + vname);
}


void
type_enumerated::name_hint(const rcstring &suggestion)
{
    //
    // If you change the invent_name function,
    // you have to change this test as well.
    //
    if (strchr(name.c_str(), ':'))
        name = suggestion;
}


unsigned
type_enumerated::get_size_in_bits()
    const
{
    unsigned nbits = 0;
    while ((size_t(1) << nbits) < size())
        ++nbits;
    return nbits;
}


bool
type_enumerated::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
    type::pointer r1 = rhs;
    for (;;)
    {
        type_subrange::pointer rr =
            boost::dynamic_pointer_cast<type_subrange>(r1);
        if (!rr)
            break;
        r1 = rr->get_subtype();
    }

    type_enumerated::pointer r2 =
        boost::dynamic_pointer_cast<type_enumerated>(r1);
    if (!r2)
        return false;
    return (get_name() == r2->get_name());
}


bool
type_enumerated::is_a(const expression::pointer &ep)
{
    return is_a(ep->get_type());
}


bool
type_enumerated::is_a(const type::pointer &tp)
{
    type::pointer r1 = tp;
    for (;;)
    {
        type_subrange::pointer rr =
            boost::dynamic_pointer_cast<type_subrange>(r1);
        if (!rr)
            break;
        r1 = rr->get_subtype();
    }

    return boost::dynamic_pointer_cast<type_enumerated>(r1);
}


bool
type_enumerated::is_a_variable()
    const
{
    return true;
}


type::pointer
type_enumerated::get_subtype()
    const
{
    assert(0);
    return create();
}


int
type_enumerated::get_minimum_value()
    const
{
    return 0;
}


int
type_enumerated::get_maximum_value()
    const
{
    assert(size() >= 1);
    return (size() - 1);
}


bool
type_enumerated::may_be_used_as_array_index()
    const
{
    return true;
}


bool
type_enumerated::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_enumerated::get_human_readable_representation_of_value(int n)
    const
{
    if (n < 0 || (size_t)n >= members.size())
        return rcstring::printf("%d", n);
    return members.get(n)->get_name();
}


// vim: set ts=8 sw=4 et :
