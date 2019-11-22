//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#ifndef LIB_CODEFILE_FILTER_REMOVE_BY_NAME_H
#define LIB_CODEFILE_FILTER_REMOVE_BY_NAME_H

#include <lib/ac/vector>

#include <lib/codefile/filter.h>

/**
  * The codefile_filter_remove_by_name class is used to represent a
  * codefile filter to removes any segments with the given name.
  */
class codefile_filter_remove_by_name:
    public codefile_filter
{
public:
    typedef boost::shared_ptr<codefile_filter_remove_by_name> pointer;

    /**
      * The destructor.
      */
    virtual ~codefile_filter_remove_by_name();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The codefile we are filtering.
      * @param segname
      *     The name of the segment to remove.
      */
    static codefile::pointer create(const codefile::pointer &deeper,
        const rcstring &segname);

protected:
    // See base class for documentation.
    segment::pointer get_segment(unsigned nth) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param deeper
      *     The codefile we are filtering.
      * @param segname
      *     The name of the segment to remove.
      */
    codefile_filter_remove_by_name(const codefile::pointer &deeper,
        const rcstring &segname);

    typedef std::vector<segment::pointer> segs_t;

    /**
      * The segs instance variable is used to remember the filtered
      * list of available segments.  We store the list rather than
      * recalculate it repeatedly for #get_segment method calls.
      */
    segs_t segs;

    /**
      * The default constructor.  Do not use.
      */
    codefile_filter_remove_by_name();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    codefile_filter_remove_by_name(const codefile_filter_remove_by_name &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    codefile_filter_remove_by_name &operator=(
        const codefile_filter_remove_by_name &rhs);
};

#endif // LIB_CODEFILE_FILTER_REMOVE_BY_NAME_H
// vim: set ts=8 sw=4 et :
