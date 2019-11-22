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
#include <lib/rcstring/accumulator.h>

#include <ucsdpsys_link/link_info_entry.h>
#include <ucsdpsys_link/segrec.h>



link_info_entry::~link_info_entry()
{
}


link_info_entry::link_info_entry(
    const link_info::pointer &a_lip,
    segrec *a_origin
) :
    lip(a_lip),
    origin(a_origin)
{
}


link_info_entry::pointer
link_info_entry::create(const link_info::pointer &a_lip, segrec *a_origin)
{
    return pointer(new link_info_entry(a_lip, a_origin));
}


link_info_entry &
link_info_entry::operator=(const link_info_entry &rhs)
{
    if (&rhs != this)
    {
        lip = rhs.lip;
        origin = rhs.origin;
        place = rhs.place;
    }
    return *this;
}


rcstring
link_info_entry::get_symbol_table_key(void)
    const
{
    return get_symbol_table_key(lip->get_name(), lip->get_litype());
}


rcstring
link_info_entry::get_symbol_table_key(const rcstring &name,
    link_info_type_t litype)
{
    return name + ":" + link_info_type_name(litype);
}


#if 0
bool
link_info_entry::operator==(const link_info_entry &rhs)
    const
{
    return (name == rhs.name && litype == rhs.litype);
}


bool
link_info_entry::operator!=(const link_info_entry &rhs)
    const
{
    return !operator==(rhs);
}


bool
link_info_entry::operator<(const link_info_entry &rhs)
    const
{
    return (name != rhs.name ? name < rhs.name : litype < rhs.litype);
}


bool
link_info_entry::operator>=(const link_info_entry &rhs)
    const
{
    return !operator<(rhs);
}


bool
link_info_entry::operator<=(const link_info_entry &rhs)
    const
{
    return (name != rhs.name ? name <= rhs.name : litype <= rhs.litype);
}


bool
link_info_entry::operator>(const link_info_entry &rhs)
    const
{
    return !operator<=(rhs);
}
#endif


rcstring
link_info_entry::representation(void)
    const
{
    rcstring_accumulator buf;
    buf.push_back("{ lip = ");
    buf.push_back(lip->representation());

    switch (lip->get_litype())
    {
    case CONSTDEF:
    case CONSTREF:
    case EOFMARK:
    case GLOBDEF:
    case GLOBREF:
    case PRIVREF:
    case PUBLDEF:
    case PUBLREF:
    case SEPFREF:
    case SEPPREF:
    case UNITREF:
        break;

    case EXTPROC:
    case EXTFUNC:
        break;

    case SEPPROC:
    case SEPFUNC:
        if (place.length)
        {
            buf.push_back(", place = ");
            buf.push_back(place.representation());
        }
        break;
    }
    buf.push_back(" }");
    return buf.mkstr();
}


link_info_entry::place_t::place_t() :
    destbase(0),
    srcbase(0),
    length(0)
{
}


rcstring
link_info_entry::place_t::representation(void)
    const
{
    return
        rcstring::printf
        (
            "{ destbase = %d, srcbase = %d, length = %d }",
            destbase,
            srcbase,
            length
        );
}


void
link_info_entry::sanity_check(const rcstring &caption)
    const
{
    lip->sanity_check(caption);
}


// vim: set ts=8 sw=4 et :
