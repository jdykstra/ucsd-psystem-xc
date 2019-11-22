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

#include <lib/type/string.h>


type_string::~type_string()
{
}


type_string::type_string(int a_size) :
    size(a_size)
{
    assert(size >= 1);
    assert(size <= 255);
}


type_string::pointer
type_string::create(int a_size)
{
    return pointer(new type_string(a_size));
}


rcstring
type_string::get_name()
    const
{
    if (size == default_size)
        return "string";
    return rcstring::printf("string[%d]", size);
}


rcstring
type_string::get_pascal_name()
    const
{
    return get_name();
}


rcstring
type_string::get_c_declaration(const rcstring &vname)
    const
{
    // This is difficult.
#if 0
    // Do we do an exact simulation?
    return
        (
            "unsigned char "
        +
            vname
        +
            "[" + rcstring::printf("%d", 1 + size)
        +
            "]"
        );
#else
    // Or do we go with something more usable?
    return ("rcstring " + vname);
    //return ("std::string " + vname);
#endif
}


unsigned
type_string::get_size_in_bits()
    const
{
    // The +1 is for the length byte.
    return ((1 + size) * 8);
}


bool
type_string::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_string::is_a(const type::pointer &arg)
{
    return is_a(arg.get());
}


bool
type_string::is_a(const type *arg)
{
    return (0 != dynamic_cast<const type_string *>(arg));
}


bool
type_string::same_type_as(const type::pointer &rhs)
    const
{
    return is_a(rhs);
}


bool
type_string::is_a_variable()
    const
{
    return true;
}


type::pointer
type_string::get_subtype()
    const
{
    assert(0);
    return create();
}


bool
type_string::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_string::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :
