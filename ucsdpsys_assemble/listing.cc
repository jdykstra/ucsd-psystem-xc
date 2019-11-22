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

#include <ucsdpsys_assemble/listing.h>


listing::~listing()
{
}


listing::listing(const output::pointer &arg) :
    orig(arg),
    col(arg),
    addr_empty_flag(true)
{
    address_column = col.create_new_column(0, 5);
    data_column = col.create_new_column(6, 6 + 14);
    source_column = col.create_new_column(22, 0);
}


bool
listing::addr_empty(void)
    const
{
    return addr_empty_flag;
}


void
listing::addr_printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    address_column->vfprintf(fmt, ap);
    va_end(ap);
    addr_empty_flag = false;
}


void
listing::data_printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    data_column->vfprintf(fmt, ap);
    va_end(ap);
}


void
listing::eoln(void)
{
    col.eoln();
    addr_empty_flag = true;
}


output::pointer
listing::get_source_column(void)
    const
{
    assert(source_column);
    return source_column;
}


void
listing::set_heading1(const rcstring &text)
{
    orig->set_heading1(text);
}


void
listing::set_heading2(const rcstring &text)
{
    orig->set_heading2(text);
}


void
listing::flush(void)
{
    col.flush();
}


// vim: set ts=8 sw=4 et :
