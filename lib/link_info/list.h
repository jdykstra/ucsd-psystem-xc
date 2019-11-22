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

#ifndef LIB_LINK_INFO_LIST_H
#define LIB_LINK_INFO_LIST_H

#include <lib/ac/vector>

#include <lib/library_map_columns.h>
#include <lib/link_info.h>
#include <lib/output.h>

class codefile; // forward

/**
  * The link_info_list class is used to represent an ordered list of
  * link_info objects.
  */
class link_info_list
{
public:
    /**
      * The destructor.
      */
    virtual ~link_info_list();

    /**
      * The default constructor.
      */
    link_info_list();

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialization.
      */
    link_info_list(const link_info_list &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    link_info_list &operator=(const link_info_list &rhs);

    void push_back(const link_info::pointer &lip);

    void print_library_map(library_map_columns &columns) const;

    /**
      * The serialize_size method may be used to calculate how many
      * bytes of link into are need to represent this complete list.
      */
    unsigned serialize_size(void) const;

    /**
      * The serialize method is used to convert this list
      * into an array of bytes before writing them to the codefile.
      *
      * @param data
      *     where to write the data.
      * @param cfp
      *     The code file being written to, for the endian coding of words
      */
    void serialize(unsigned char *data, const codefile *cfp) const;

    /**
      * The empty method may be used to determine whether or not the
      * list is empty (has no entries).
      */
    bool empty(void) const { return content.empty(); }

    size_t size(void) const { return content.size(); }

    link_info::pointer get(size_t n) const { return content[n]; }

    bool ends_with_eofmark(void) const;

    /**
      * The extendable method is used to test to see if the given link
      * into can be appended to an existing record, rather than making
      * the list longer.  This is useful for the various *REF types of
      * link records.
      *
      * @param lip
      *     The link info record to test.
      */
    bool extendable(const link_info::pointer &lip);

private:
    typedef std::vector<link_info::pointer> content_t;

    content_t content;
};

#endif // LIB_LINK_INFO_LIST_H
// vim: set ts=8 sw=4 et :
