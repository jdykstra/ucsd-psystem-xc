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

#ifndef LIB_SEGMENT_FILTER_H
#define LIB_SEGMENT_FILTER_H

#include <lib/segment.h>

/**
  * The segment_filter class is used to represent a filter operation
  * for a segment, to change something about a segment, while still
  * appearing to be a simple segment.
  */
class segment_filter:
    public segment
{
public:
    typedef boost::shared_ptr<segment_filter> pointer;

    /**
      * The destructor.
      */
    virtual ~segment_filter();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param deeper
      *     The segment we are filtering, all operations are passed to
      *     it (eventually).
      */
    segment_filter(const segment::pointer &deeper);

    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    segkind_t get_kind(void) const;

    // See base class for documentation.
    void set_kind(segkind_t value);

    // See base class for documentation.
    mtype_t get_mtype(void) const;

    // See base class for documentation.
    void set_mtype(mtype_t value);

    // See base class for documentation.
    segversion_t get_version(void) const;

    // See base class for documentation.
    void set_version(segversion_t value);

    // See base class for documentation.
    byte_sex_t get_byte_sex(void) const;

    // See base class for documentation.
    unsigned get_segment_number(void) const;

    // See base class for documentation.
    unsigned get_text_address(void) const;

    // See base class for documentation.
    unsigned get_text_address(const rcstring &text);

    // See base class for documentation.
    rcstring get_interface_text(void) const;

    // See base class for documentation.
    rcstring get_interface_text(unsigned block_num, int block_len) const;

    // See base class for documentation.
    unsigned get_code_address(void) const;

    // See base class for documentation.
    unsigned get_code_address(unsigned code_length);

    // See base class for documentation.
    unsigned get_code_length(void) const;

    // See base class for documentation.
    void write_block(unsigned blknum, const void *data, unsigned nblocks) const;

    // See base class for documentation.
    void read_block(unsigned blknum, void *data, unsigned nblocks) const;

    // See base class for documentation.
    const link_info_list &get_link_info(void) const;

    // See base class for documentation.
    void segment_not_linked(const link_info::pointer &link_info_entry);

    // See base class for documentation.
    void add_link_information(const link_info::pointer &link_info_entry);

    // See base class for documentation.
    void write_link_information(int nextlc) const;

    // See base class for documentation.
    unsigned get_linkinfo_nblocks(void) const;

private:
    /**
      * The deeper instance variable i sused to remember the segment we
      * are filtering, all operations are passed to it (eventually).
      */
    segment::pointer deeper;

    /**
      * The default constructor.  Do not use.
      */
    segment_filter();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    segment_filter(const segment_filter &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    segment_filter &operator=(const segment_filter &rhs);
};

#endif // LIB_SEGMENT_FILTER_H
// vim: set ts=8 sw=4 et :
