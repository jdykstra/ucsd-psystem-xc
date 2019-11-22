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

#include <lib/ac/string.h>

#include <lib/anonymous_name.h>
#include <lib/type/subrange.h>


type_subrange::~type_subrange()
{
}


type_subrange::type_subrange(int a_minimum, int a_maximum,
        const type::pointer &a_subtype) :
    minimum(a_minimum),
    maximum(a_maximum),
    subtype(a_subtype),
    name(anonymous_name())
{
    assert(a_minimum <= a_maximum);
}


type_subrange::pointer
type_subrange::create(int a_minimum, int a_maximum,
    const type::pointer &a_subtype)
{
    return pointer(new type_subrange(a_minimum, a_maximum, a_subtype));
}


rcstring
type_subrange::get_name()
    const
{
    return get_pascal_name();
}


rcstring
type_subrange::get_pascal_name()
    const
{
    return
        (
            subtype->get_human_readable_representation_of_value(minimum)
        +
            ".."
        +
            subtype->get_human_readable_representation_of_value(maximum)
        );
}


rcstring
type_subrange::get_c_declaration(const rcstring &vname)
    const
{
    return get_subtype()->get_c_declaration(vname);
}


void
type_subrange::name_hint(const rcstring &suggestion)
{
    if (strchr(name.c_str(), ':'))
        name = subtype->get_name() + " subrange " + suggestion;
}


unsigned
type_subrange::get_size_in_bits()
    const
{
    if (minimum < 0)
        return 16;
    unsigned nbits = 0;
    while ((1 << nbits) <= maximum)
        ++nbits;
    return nbits;
}


type::pointer
type_subrange::get_subtype()
    const
{
    return subtype;
}


bool
type_subrange::is_a_variable()
    const
{
    return true;
}


bool
type_subrange::is_a(const type::pointer &arg)
{
    return is_a(arg.get());
}


bool
type_subrange::is_a(const type *arg)
{
    return (0 != dynamic_cast<const type_subrange *>(arg));
}


bool
type_subrange::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
# if 0
    // FIXME: we can't cope with this much strictness
    // at the moment.
    return
        (
            is_a(rhs)
        &&
            get_minimum_value() == rhs->get_minimum_value()
        &&
            get_maximum_value() == rhs->get_maximum_value()
        );
#else
    return subtype->same_type_as(rhs);
#endif
}


int
type_subrange::get_minimum_value()
    const
{
    return minimum;
}


int
type_subrange::get_maximum_value()
    const
{
    return maximum;
}


bool
type_subrange::may_be_used_as_array_index()
    const
{
    return true;
}


bool
type_subrange::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_subrange::get_human_readable_representation_of_value(int n)
    const
{
    return subtype->get_human_readable_representation_of_value(n);
}


// vim: set ts=8 sw=4 et :
