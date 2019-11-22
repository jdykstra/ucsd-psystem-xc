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

#include <lib/ac/string.h>

#include <lib/codefile.h>
#include <lib/link_info/list.h>


link_info_list::~link_info_list()
{
}


link_info_list::link_info_list()
{
}


link_info_list::link_info_list(const link_info_list &rhs) :
    content(rhs.content)
{
}


link_info_list &
link_info_list::operator=(const link_info_list &rhs)
{
    if (this != &rhs)
    {
        content = rhs.content;
    }
    return *this;
}


void
link_info_list::push_back(const link_info::pointer &lip)
{
    assert(lip);
    content.push_back(lip);
}


unsigned
link_info_list::serialize_size(void)
    const
{
    unsigned result = 0;
    for
    (
        content_t::const_iterator it = content.begin();
        it != content.end();
        ++it
    )
    {
        result += (*it)->serialize_size();
    }
    return result;
}


void
link_info_list::serialize(unsigned char *data, const codefile *cfp)
    const
{
    for
    (
        content_t::const_iterator it = content.begin();
        it != content.end();
        ++it
    )
    {
        size_t nbytes = (*it)->serialize(data, cfp);
        data += nbytes;
    }
}


bool
link_info_list::ends_with_eofmark(void)
    const
{
    return (!content.empty() && content.back()->is_eofmark());
}


bool
link_info_list::extendable(const link_info::pointer &lip)
{
    // This is harder to do than it appears.  You can't search from
    // the front, because that would run separate procedures together.
    // Instead, we search backwards from the end, and stop if we wee
    // SEPPROC or SEPFUNC.
    for
    (
        content_t::reverse_iterator it = content.rbegin();
        it != content.rend();
        ++it
    )
    {
        link_info::pointer lip2 = *it;
        if (lip2->same_as(*lip))
        {
            lip2->copy_references(*lip);
            return true;
        }
        switch (lip2->get_litype())
        {
        case EOFMARK:
        case UNITREF:
        case GLOBREF:
        case PUBLREF:
        case PRIVREF:
        case CONSTREF:
        case GLOBDEF:
        case PUBLDEF:
        case CONSTDEF:
        case EXTPROC:
        case EXTFUNC:
        case SEPPREF:
        case SEPFREF:
        default:
            continue;

        case SEPPROC:
        case SEPFUNC:
            break;
        }
        break;
    }
    return false;
}


// vim: set ts=8 sw=4 et :
