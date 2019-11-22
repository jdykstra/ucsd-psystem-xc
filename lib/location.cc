//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//


#include <lib/location.h>


location::~location()
{
}


location::location() :
    line_number(1)
{
}


location::location(const rcstring &arg1, long arg2) :
    file_name(arg1),
    line_number(arg2 < 0 ? 0 : arg2)
{
}


location::location(const location &arg) :
    file_name(arg.file_name),
    line_number(arg.line_number)
{
}


location &
location::operator=(const location &arg)
{
    if (this != &arg)
    {
        file_name = arg.file_name;
        line_number = arg.line_number;
    }
    return *this;
}


rcstring
location::representation(void)
    const
{
    if (line_number == 0)
        return file_name;
    return rcstring::printf("%s: %ld", file_name.c_str(), line_number);
}


rcstring
location::get_debughook(int arg)
    const
{
    return rcstring::printf("debughook(%ld,%d);\n", line_number, arg);
}


rcstring
location::get_hash_line(void)
    const
{
    return
        rcstring::printf
        (
            "#line %ld %s\n",
            line_number,
            file_name.quote_c().c_str()
        );
}


void
location::operator+=(const location &rhs)
{
    if (rhs.empty())
        return;
    if (empty())
    {
        file_name = rhs.file_name;
        line_number = rhs.line_number;
        return;
    }

    if
    (
        file_name == rhs.file_name
    &&
        rhs.line_number > 0
    &&
        rhs.line_number < line_number
    )
        line_number = rhs.line_number;
}


location
location::operator+(const location &rhs)
    const
{
    location result = *this;
    result += rhs;
    return result;
}


bool
location::empty(void)
    const
{
    return file_name.empty();
}


bool
location::operator==(const location &rhs)
    const
{
    return
        (
            file_name == rhs.get_file_name()
        &&
            line_number == rhs.get_line_number()
        );
}


location
location::builtin(void)
{
    return location("builtin", 0);
}


void
location::clear(void)
{
    file_name.clear();
    line_number = 0;
}


// vim: set ts=8 sw=4 et :
