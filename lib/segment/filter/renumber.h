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

#ifndef LIB_SEGMENT_FILTER_RENUMBER_H
#define LIB_SEGMENT_FILTER_RENUMBER_H

#include <lib/segment/filter.h>

/**
  * The segment_filter_renumber class is used to represent the
  * processing required to transparently re-number a segment.
  */
class segment_filter_renumber:
    public segment_filter
{
public:
    typedef boost::shared_ptr<segment_filter_renumber> pointer;

    /**
      * The destructor.
      */
    virtual ~segment_filter_renumber();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The segment being filtered
      * @param segnum
      *     The new number for the segment.
      */
    static pointer create(const segment::pointer &deeper, int segnum);

protected:
    // See base class for documentation.
    unsigned get_segment_number(void) const;

    // See base class for documentation.
    void read_block(unsigned blknum, void *data, unsigned nblocks) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param deeper
      *     The segment being filtered
      * @param segnum
      *     The new number for the segment.
      */
    segment_filter_renumber(const segment::pointer &deeper, int segnum);

    /**
      * The segnum instance variable is used to remember the new number
      * for the segment.
      */
    int segnum;

    /**
      * The default constructor.  Do not use.
      */
    segment_filter_renumber();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    segment_filter_renumber(const segment_filter_renumber &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    segment_filter_renumber &operator=(const segment_filter_renumber &rhs);
};

#endif // LIB_SEGMENT_FILTER_RENUMBER_H
// vim: set ts=8 sw=4 et :
