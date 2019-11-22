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

#ifndef LIB_SEGINFO_H
#define LIB_SEGINFO_H

#include <lib/mtype.h>
#include <lib/segment.h>
#include <lib/segversion.h>

/**
  * The seginfo class is used to represent converting from and from the
  * seginfo field in a codefile's segment dictionary.
  */
class seginfo
{
public:
    /**
      * The destructor.
      */
    virtual ~seginfo();

    /**
      * The default constructor.  Do not use.
      */
    seginfo();

    /**
      * The constructor.
      *
      * @param segnum
      *     The segment number, 0..63
      * @param mtype
      *     The machine type of the segment
      * @param version
      *     The format version of the segment.
      */
    seginfo(unsigned segnum, mtype_t mtype, segversion_t version);

    /**
      * The constructor.
      *
      * @param encoded
      *     The packed for of a seginfo value.
      */
    seginfo(unsigned encoded);

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    seginfo(const seginfo &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    seginfo &operator=(const seginfo &rhs);

    /**
      * The packed method may be used to build the packed version of
      * a seginfo value.  This is the inverse of the constructor that
      * takes a single unsigned parameter.
      */
    unsigned pack(void) const;

    unsigned get_segnum(void) const { return segnum; }
    mtype_t get_mtype(void) const { return mtype; }
    segversion_t get_version(void) const { return version; }

private:
    unsigned segnum;
    mtype_t mtype;
    segversion_t version;
};

#endif // LIB_SEGINFO_H
// vim: set ts=8 sw=4 et :
