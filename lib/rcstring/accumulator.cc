//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <lib/ac/string.h>
#include <lib/rcstring/accumulator.h>


rcstring_accumulator::~rcstring_accumulator()
{
    delete [] buffer;
    buffer = 0;
    length = 0;
    maximum = 0;
}


rcstring_accumulator::rcstring_accumulator() :
    length(0),
    maximum(0),
    buffer(0)
{
}


rcstring_accumulator::rcstring_accumulator(const rcstring_accumulator &arg) :
    length(0),
    maximum(0),
    buffer(0)
{
    push_back(arg);
}


rcstring_accumulator &
rcstring_accumulator::operator=(const rcstring_accumulator &arg)
{
    if (this != &arg)
    {
        clear();
        push_back(arg);
    }
    return *this;
}


rcstring
rcstring_accumulator::mkstr()
    const
{
    return rcstring(buffer, length);
}


void
rcstring_accumulator::overflow(char c)
{
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 16;
        char *new_buffer = new char [new_maximum];
        if (length)
            memcpy(new_buffer, buffer, length);
        delete [] buffer;
        buffer = new_buffer;
        maximum = new_maximum;
    }
    buffer[length++] = c;
}


void
rcstring_accumulator::push_back(const rcstring_accumulator &arg)
{
    push_back(arg.buffer, arg.length);
}


void
rcstring_accumulator::push_back(const void *data, size_t n)
{
    const char *cp = (const char *)data;
    if (!n)
        return;
    if (length + n > maximum)
    {
        size_t new_maximum = maximum;
        for (;;)
        {
            new_maximum = new_maximum * 2 + 16;
            if (length + n <= new_maximum)
                break;
        }
        char *new_buffer = new char [new_maximum];
        if (length)
            memcpy(new_buffer, buffer, length);
        delete [] buffer;
        buffer = new_buffer;
        maximum = new_maximum;
    }
    memcpy(buffer + length, cp, n);
    length += n;
}


void
rcstring_accumulator::push_back(const char *s)
{
    push_back(s, strlen(s));
}


bool
rcstring_accumulator::ends_with(const char *text)
    const
{
    size_t len = strlen(text);
    return (length >= len && 0 == memcmp(buffer + length - len, text, len));
}


// vim: set ts=8 sw=4 et :
