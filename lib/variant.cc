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

#include <lib/debug.h>
#include <lib/variant.h>


variant::~variant()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


variant::variant(unsigned nwords) :
    size_in_words(nwords)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "nwords = %u", nwords);
}


variant::pointer
variant::create(unsigned nwords)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new variant(nwords));
}


variant::variant(const type::pointer &a_vtype) :
    size_in_words(0),
    vtype(a_vtype)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


variant::pointer
variant::create(const type::pointer &a_vtype)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new variant(a_vtype));
}


void
variant::register_child(int value, const variant::pointer &child)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "value = %d", value);
    assert(vtype);
    DEBUG(2, "vtype = %s", vtype->get_name().c_str());
    assert(child);
    children[value] = child;

    unsigned n = child->get_size_in_words();
    assert(n);
    if (size_in_words < n)
    {
        size_in_words = n;
        DEBUG(2, "size in words = %u", size_in_words);
    }
    DEBUG(1, "}");
}


void
variant::set_otherwise_size(unsigned nwords)
{
    assert(!otherwise);
    otherwise = variant::create(nwords);
}


variant::pointer
variant::get_child(int value)
    const
{
    assert(vtype);
    children_t::const_iterator it = children.find(value);
    if (it == children.end())
        return otherwise;
    return (*it).second;
}


bool
variant::terminal()
    const
{
    return children.empty();
}


void
variant::set_size(unsigned nwords)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "nwords = %u", nwords);
    assert(children.empty());
    assert(size_in_words == 0);
    assert(nwords > 0);
    size_in_words = nwords;
}


// vim: set ts=8 sw=4 et :
