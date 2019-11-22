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

#include <lib/type/long_integer/sized.h>


type_long_integer_sized::~type_long_integer_sized()
{
}


type_long_integer_sized::type_long_integer_sized()
{
}


type_long_integer_sized::pointer
type_long_integer_sized::create(void)
{
    return pointer(new type_long_integer_sized());
}


rcstring
type_long_integer_sized::get_name(void)
    const
{
    return rcstring::printf("integer[*]");
}


unsigned
type_long_integer_sized::get_size_in_words(void)
    const
{
    assert(!"sized long integers are only ever on the stack");
    return 1;
}


unsigned
type_long_integer_sized::get_size_in_bits(void)
    const
{
    assert(!"sized long integers are only ever on the stack");
    return 16;
}


bool
type_long_integer_sized::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_long_integer_sized::is_a(const type::pointer &arg)
{
    return !!dynamic_cast<const type_long_integer_sized *>(arg.get());
}


bool
type_long_integer_sized::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
    return is_a(rhs);
}


bool
type_long_integer_sized::is_a_variable(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
