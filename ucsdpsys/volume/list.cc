//
// UCSD p-System operating system
// Copyright (C) 2006, 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
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

#include <lib/rcstring/list.h>
#include <ucsdpsys/volume/list.h>


volume_list::~volume_list()
{
    delete [] item;
    length = 0;
    maximum = 0;
    item = 0;
}


volume_list::volume_list() :
    length(0),
    maximum(0),
    item(0)
{
}


volume_list::pointer
volume_list::create(void)
{
    return pointer(new volume_list());
}


void
volume_list::push_back(const volume::pointer &vp)
{
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 16;
        volume::pointer *new_item = new volume::pointer [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_item[j] = item[j];
        delete [] item;
        item = new_item;
        maximum = new_maximum;
    }
    item[length++] = vp;
}


void
volume_list::insert_second(const volume::pointer &vp)
{
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 16;
        volume::pointer *new_item = new volume::pointer [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_item[j] = item[j];
        delete [] item;
        item = new_item;
        maximum = new_maximum;
    }
    if (length < 2)
        item[length++] = vp;
    else
    {
        for (size_t k = length; k > 1; --k)
            item[k] = item[k - 1];
        ++length;
        item[1] = vp;
    }
}


void
volume_list::slurp(void)
{
    for (size_t j = 0; j < length; ++j)
        item[j]->slurp();
}


void
volume_list::unslurp(void)
{
    for (size_t j = 0; j < length; ++j)
        item[j]->unslurp();
}


rcstring
volume_list::command_line_argument(void)
{
    rcstring_list results;
    for (size_t j = 0; j < length; ++j)
        results.push_back(item[j]->command_line_argument());
    return results.unsplit();
}


bool
volume_list::contains_system_pascal(void)
    const
{
    for (size_t j = 0; j < length; ++j)
        if (item[j]->contains_system_pascal())
            return true;
    return false;
}


bool
volume_list::contains_system_files(void)
{
    for (size_t j = 0; j < length; ++j)
        if (item[j]->contains_system_files())
            return true;
    return false;
}


// vim: set ts=8 sw=4 et :
