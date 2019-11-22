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

#include <lib/set.h>


set::~set()
{
    delete [] value;
    length = 0;
    maximum = 0;
    value = 0;
}


set::set() :
    length(0),
    maximum(0),
    value(0)
{
}


set::set(unsigned bitnum) :
    length(0),
    maximum(0),
    value(0)
{
    set_bit(bitnum);
}


set::set(unsigned bitnum_lo, unsigned bitnum_hi) :
    length(0),
    maximum(0),
    value(0)
{
    assert(bitnum_lo <= bitnum_hi);
    for (;;)
    {
        set_bit(bitnum_lo);
        if (bitnum_lo >= bitnum_hi)
            break;
        ++bitnum_lo;
    }
}


set::set(const set &arg) :
    length(0),
    maximum(0),
    value(0)
{
    size_t new_maximum = 0;
    while (new_maximum < arg.length)
        new_maximum = new_maximum * 2 + 8;
    if (new_maximum)
    {
        value = new value_t [new_maximum];
        maximum = new_maximum;
        size_t j = 0;
        for (; j < arg.length; ++j)
            value[j] = arg.value[j];
        for (; j < maximum; ++j)
            value[j] = 0;
        length = arg.length;
    }
}


set &
set::operator=(const set &arg)
{
    if (this != &arg)
    {
        if (maximum < arg.length)
        {
            delete [] value;
            for (;;)
            {
                maximum = maximum * 2 + 8;
                if (maximum >= arg.length)
                    break;
            }
            value = new value_t [maximum];
            size_t j = 0;
            for (; j < arg.length; ++j)
                value[j] = arg.value[j];
            length = arg.length;
            for (; j < maximum; ++j)
                value[j] = 0;
        }
        else
        {
            size_t j = 0;
            for (; j < arg.length; ++j)
                value[j] = arg.value[j];
            // We only need to zero out the portion between arg's length
            // and this's length, because from length..maximum is
            // already zero.
            for (; j < length; ++j)
                value[j] = 0;
            length = arg.length;
        }
    }
    return *this;
}


set &
set::operator+=(const set &arg)
{
    if (arg.length > maximum)
    {
        assert(length <= maximum);
        assert(arg.length > length);
        for (;;)
        {
            maximum = maximum * 2 + 8;
            if (maximum >= arg.length)
                break;
        }
        value_t *new_value = new value_t [maximum];
        size_t j = 0;
        for (; j < length; ++j)
            new_value[j] = value[j] | arg.value[j];
        for (; j < arg.length; ++j)
            new_value[j] = arg.value[j];
        for (; j < maximum; ++j)
            new_value[j] = 0;
        delete [] value;
        value = new_value;
        length = arg.length;
    }
    else
    {
        for (size_t j = 0; j < arg.length; ++j)
            value[j] |= arg.value[j];
        if (length < arg.length)
            length = arg.length;
    }
    return *this;
}


set
set::operator+(const set &arg)
    const
{
    set result(*this);
    result += arg;
    return result;
}


set &
set::operator-=(const set &arg)
{
    for (size_t j = 0; j < length && j < arg.length; ++j)
        value[j] &= ~arg.value[j];
    // normalize
    while (length > 0 && value[length - 1] == 0)
        --length;
    return *this;
}


set
set::operator-(const set &arg)
    const
{
    set result(*this);
    result -= arg;
    return result;
}


set &
set::operator*=(const set &arg)
{
    while (length > arg.length)
        value[--length] = 0;
    for (size_t j = 0; j < length; ++j)
        value[j] &= arg.value[j];
    // normalize
    while (length > 0 && value[length - 1] == 0)
        --length;
    return *this;
}


set
set::operator*(const set &arg)
    const
{
    set result(*this);
    result *= arg;
    return result;
}


size_t
set::size()
    const
{
    assert(length == 0 || (value && value[length - 1] != 0));
    return length;
}


unsigned
set::nth(size_t n)
    const
{
    assert(n < length);
    return value[n];
}


void
set::clear()
{
    while (length > 0)
        value[--length] = 0;
}


bool
set::get_bit(size_t n)
    const
{
    size_t word = n >> 4;
    if (word >= length)
        return false;
    value_t bit = ((value_t)1 << (n & 15));
    return (value[word] & bit);
}


void
set::set_bit(size_t n, bool val)
{
    size_t word = n >> 4;
    value_t bit = ((value_t)1 << (n & 15));
    if (val)
    {
        //
        // Set the bit...
        //
        if (word >= maximum)
        {
            for (;;)
            {
                maximum = maximum * 2 + 8;
                if (word < maximum)
                    break;
            }
            value_t *new_value = new value_t [maximum];
            size_t j = 0;
            while (j < length)
            {
                new_value[j] = value[j];
                ++j;
            }
            while (j < maximum)
                new_value[j++] = 0;
            delete [] value;
            value = new_value;
        }
        assert(word < maximum);
        if (word >= length)
            length = word + 1;
        value[word] |= bit;
    }
    else
    {
        //
        // Clear the bit...
        //
        if (word < length)
        {
            value[word] &= ~bit;
            // normalize
            while (length > 0 && value[length - 1] == 0)
                --length;
        }
    }
}


bool
set::operator==(const set &rhs)
    const
{
    // both sides should be in normalized form...
    assert(length == 0 || value[length - 1]);
    assert(rhs.length == 0 || rhs.value[rhs.length - 1]);

    // ...thus if they are not the same length, they cannot be equal
    if (length != rhs.length)
        return false;

    for (size_t j = 0; j < length; ++j)
    {
        if (value[j] != rhs.value[j])
            return false;
    }
    return true;
}


bool
set::operator!=(const set &rhs)
    const
{
    return !operator==(rhs);
}


bool
set::operator<=(const set &rhs)
    const
{
    set temp(*this);
    temp -= rhs;
    return temp.empty();
}


bool
set::operator<(const set &rhs)
    const
{
    return (operator<=(rhs) && operator!=(rhs));
}


bool
set::operator>=(const set &rhs)
    const
{
    set temp(rhs);
    temp -= *this;
    return temp.empty();
}


bool
set::operator>(const set &rhs)
    const
{
    return (operator>=(rhs) && operator!=(rhs));
}


#include <lib/rcstring/accumulator.h>


rcstring
set::representation()
    const
{
    rcstring_accumulator ac;
    ac.push_back('[');
    bool first = true;
    for (size_t j = 0; j < length * 16; ++j)
    {
        if (get_bit(j))
        {
            if (!first)
                ac.push_back(", ");
            first = false;
            ac.printf("%d", int(j));
            size_t k = j;
            while (get_bit(k + 1))
                ++k;
            if (k != j)
            {
                ac.printf("..%d", int(k));
                j = k;
            }
        }
    }
    ac.push_back(']');
    return ac.mkstr();
}


// vim: set ts=8 sw=4 et :
