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
#include <lib/ac/math.h>
#include <lib/ac/time.h>
#include <libexplain/gettimeofday.h>

#include <lib/time_value.h>


time_value::~time_value()
{
}


time_value::time_value()
{
    tv_sec = 0;
    tv_usec = 0;
    assert(valid());
}


time_value::time_value(int seconds)
{
    tv_sec = seconds;
    tv_usec = 0;
    assert(valid());
}


time_value::time_value(double seconds)
{
    long whole_seconds = long(floor(seconds));
    tv_sec = whole_seconds;
    tv_usec = long(floor((seconds - whole_seconds) * 1e6));
    assert(valid());
}


bool
time_value::valid()
    const
{
    return (tv_usec >= 0 && tv_usec < 1000000);
}


time_value::time_value(const time_value &rhs)
{
    tv_sec = rhs.tv_sec;
    tv_usec = rhs.tv_usec;
    assert(valid());
}


time_value &
time_value::operator=(const time_value &rhs)
{
    assert(rhs.valid());
    if (this != &rhs)
    {
        tv_sec = rhs.tv_sec;
        tv_usec = rhs.tv_usec;
    }
    assert(valid());
    return *this;
}


time_value &
time_value::operator+=(const time_value &rhs)
{
    assert(valid());
    assert(rhs.valid());
    tv_sec += rhs.tv_sec;
    tv_usec += rhs.tv_usec;
    if (tv_usec >= 1000000)
    {
        ++tv_sec;
        tv_usec -= 1000000;
    }
    assert(valid());
    return *this;
}


time_value
time_value::operator+(const time_value &rhs)
    const
{
    time_value result(*this);
    result += rhs;
    return result;
}


time_value &
time_value::operator-=(const time_value &rhs)
{
    assert(valid());
    assert(rhs.valid());
    tv_sec -= rhs.tv_sec;
    tv_usec -= rhs.tv_usec;
    if (tv_usec < 0)
    {
        --tv_sec;
        tv_usec += 1000000;
    }
    assert(valid());
    return *this;
}


time_value
time_value::operator-(const time_value &rhs)
    const
{
    time_value result(*this);
    result -= rhs;
    return result;
}


time_value::operator double()
    const
{
    return (tv_sec + tv_usec * 1e-6);
}


void
time_value::get_time_of_day()
{
    explain_gettimeofday_or_die(this, 0);
    assert(valid());
}


bool
time_value::operator==(const time_value &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    return (tv_sec == rhs.tv_sec && tv_usec == rhs.tv_usec);
}


bool
time_value::operator!=(const time_value &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    return (tv_sec != rhs.tv_sec || tv_usec != rhs.tv_usec);
}


bool
time_value::operator<(const time_value &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    if (tv_sec != rhs.tv_sec)
        return (tv_sec < rhs.tv_sec);
    return (tv_usec < rhs.tv_usec);
}


bool
time_value::operator<=(const time_value &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    if (tv_sec < rhs.tv_sec)
        return true;
    if (tv_sec > rhs.tv_sec)
        return false;
    return (tv_usec <= rhs.tv_usec);
}


bool
time_value::operator>(const time_value &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    if (tv_sec != rhs.tv_sec)
        return (tv_sec > rhs.tv_sec);
    return (tv_usec > rhs.tv_usec);
}


bool
time_value::operator>=(const time_value &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    if (tv_sec < rhs.tv_sec)
        return false;
    if (tv_sec > rhs.tv_sec)
        return true;
    return (tv_usec >= rhs.tv_usec);
}


// vim: set ts=8 sw=4 et :
