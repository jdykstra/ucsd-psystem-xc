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

#include <lib/seginfo.h>


seginfo::~seginfo()
{
}


seginfo::seginfo() :
    segnum(0),
    mtype(mtype_t(0)),
    version(segversion_t(0))
{
}


seginfo::seginfo(unsigned a_segnum, mtype_t a_mtype, segversion_t a_version) :
    segnum(a_segnum),
    mtype(a_mtype),
    version(a_version)
{
}


seginfo::seginfo(unsigned x) :
    segnum(x & 0xFF),
    mtype(mtype_t((x >> 8) & 15)),
    version(segversion_t((x >> 13) & 7))
{
}


seginfo::seginfo(const seginfo &rhs) :
    segnum(rhs.segnum),
    mtype(rhs.mtype),
    version(rhs.version)
{
}


seginfo &
seginfo::operator=(const seginfo &rhs)
{
    if (this != &rhs)
    {
        segnum = rhs.segnum;
        mtype = rhs.mtype;
        version = rhs.version;
    }
    return *this;
}


unsigned
seginfo::pack(void)
    const
{
    return (segnum & 0xFF) | ((mtype & 15) << 8) | ((version & 7) << 13);
}


// vim: set ts=8 sw=4 et :
