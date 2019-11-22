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

#ifndef LIB_CODEFILE_FILTER_INSERT_H
#define LIB_CODEFILE_FILTER_INSERT_H

#include <lib/ac/vector>

#include <lib/codefile/filter.h>

/**
  * The codefile_filter_insert class is used to represent a filter that
  * inserts a segment from another file.
  */
class codefile_filter_insert:
    public codefile_filter
{
public:
    typedef boost::shared_ptr<codefile_filter_insert> pointer;

    /**
      * The destructor.
      */
    virtual ~codefile_filter_insert();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The codefile to be filtered.
      * @param deeper2
      *     The codefile That is the source of the segment to be inserted.
      * @param sp2
      *     The segment (within deeper2) to be inserted.
      */
    static codefile::pointer create(const codefile::pointer &deeper,
        const codefile::pointer &deeper2, const segment::pointer &sp2);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The codefile to be filtered.
      * @param deeper2
      *     The codefile That is the source of the segment to be inserted.
      * @param segname2
      *     The name of the segment (within deeper2) to be inserted.
      */
    static codefile::pointer create(const codefile::pointer &deeper,
        const codefile::pointer &deeper2, const rcstring &segname2);

protected:
    // See base file for documentation.
    segment::pointer get_segment(unsigned number) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param deeper
      *     The codefile to be filtered.
      * @param deeper2
      *     The codefile That is the source of the segment to be inserted.
      * @param sp2
      *     The segment (within deeper2) to be inserted.
      */
    codefile_filter_insert(const codefile::pointer &deeper,
        const codefile::pointer &deeper2, const segment::pointer &sp2);

    typedef std::vector<segment::pointer> segs_t;

    /**
      * The segs instance variable is used to remember the filtered
      * list of available segments.  We store the list rather than
      * recalculate it repeatedly for #get_segment method calls.
      */
    segs_t segs;

    /**
      * The deeper2 instance variable is used to rememeber the source of
      * the other segment.  (Segments only have weak parent pointers.)
      */
    codefile::pointer deeper2;

    /**
      * The default constructor.  Do not use.
      */
    codefile_filter_insert();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    codefile_filter_insert(const codefile_filter_insert &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    codefile_filter_insert &operator=(const codefile_filter_insert &rhs);
};

#endif // LIB_CODEFILE_FILTER_INSERT_H
// vim: set ts=8 sw=4 et :
