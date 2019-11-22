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

#include <lib/type/long_integer/unsized.h>


type_long_integer_unsized::~type_long_integer_unsized()
{
}


type_long_integer_unsized::type_long_integer_unsized(int ndig) :
    nwords((ndig + 3) >> 2)
{
    assert(ndig >= 1);
    assert(ndig <= max_digits);
}


type_long_integer_unsized::pointer
type_long_integer_unsized::create(int ndig)
{
    return pointer(new type_long_integer_unsized(ndig));
}


rcstring
type_long_integer_unsized::get_name(void)
    const
{
    return rcstring::printf("integer[%d]", nwords * 4);
}


unsigned
type_long_integer_unsized::get_size_in_words(void)
    const
{
    return (1 + nwords);
}


unsigned
type_long_integer_unsized::get_size_in_bits(void)
    const
{
    return (16 * (1 + nwords));
}


bool
type_long_integer_unsized::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_long_integer_unsized::is_a(const type::pointer &arg)
{
    return !!dynamic_cast<const type_long_integer_unsized *>(arg.get());
}


bool
type_long_integer_unsized::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
    return is_a(rhs);
}


bool
type_long_integer_unsized::is_a_variable(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
