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
#include <lib/type.h>


type::~type()
{
}


type::type()
{
}


type::type(const type &)
{
}


unsigned
type::get_size_in_bytes()
    const
{
    return (2 * get_size_in_words());
}


unsigned
type::get_size_in_words()
    const
{
    return ((get_size_in_bits() + 15) >> 4);
}


int
type::get_finit_recwords()
    const
{
    // See system/system.b.text in the II.0
    // source for the FINIT implementation.
    return get_size_in_words();
}


bool
type::is_func()
    const
{
    return false;
}


bool
type::is_an_implicit_reference()
    const
{
    return false;
}


void
type::name_hint(const rcstring &)
{
    // Do nothing.
}


int
type::get_minimum_value()
    const
{
    DEBUG(1, "type is %s", typeid(*this).name());
    assert(!"must be overridden by derived class");
    return 0;
}


int
type::get_maximum_value()
    const
{
    DEBUG(1, "type is %s", typeid(*this).name());
    assert(!"must be overridden by derived class");
    return 0;
}


bool
type::may_be_used_as_array_index()
    const
{
    return false;
}


type::pointer
type::late_binding(const type::pointer &tp)
{
    type::pointer result = tp->late_binding_or_null();
    return (result ? result : tp);
}


type::pointer
type::late_binding_or_null()
    const
{
    return pointer();
}


// vim: set ts=8 sw=4 et :
