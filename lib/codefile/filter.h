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

#ifndef LIB_CODEFILE_FILTER_H
#define LIB_CODEFILE_FILTER_H

#include <lib/codefile.h>

/**
  * The codefile_filter abstract class is used to represent a codefile
  * filter of some sort.  It passes all operations to the deeper
  * codefile,derived classes will override the necessary methods to
  * impose their filtering.
  */
class codefile_filter:
    public codefile
{
public:
    typedef boost::shared_ptr<codefile_filter> pointer;

    /**
      * The destructor.
      */
    virtual ~codefile_filter();

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
      *
      * @param deeper
      *     The codefile to be filtered.
      */
    codefile_filter(const codefile::pointer &deeper);

    // See base class for documentation.
    void read_block(unsigned block_num, void *data, unsigned nblocks) const;

    // See base class for documentation.
    void write_block(unsigned block_num, const void *data, unsigned nblocks)
        const;

    // See base class for documentation.
    byte_sex_t get_byte_sex(void) const;

    // See base class for documentation.
    segment::pointer new_segment(const rcstring &name, int segnum,
        segkind_t segkind, mtype_t mtype, segversion_t version);

    // See base class for documentation.
    unsigned allocate(unsigned nblocks);

    // See base class for documentation.
    void write_segment_dictionary(void);

    // See base class for documentation.
    void set_copyright_notice(const rcstring &text);

    // See base class for documentation.
    rcstring get_copyright_notice(void) const;

    // See base class for documentation.
    rcstring get_filename(void) const;

    // See base class for documentation.
    segment::pointer get_segment(unsigned number) const;

    // See base class for documentation.
    void create_dataseg(const rcstring &name, int segnum, int size_in_words);

    // See base class for documentation.
    unsigned maximum_number_of_segments(void) const;

    // See base class for documentation.
    codefile::pointer new_file_factory(const rcstring &filename) const;

    // See base class for documentation.
    disassembler_pointer native_disassembler_factory(mtype_t mtype,
        dislabel &where, discolumns &report, int seg_num, int proc_num,
        int proc_base, code_source_map *listing) const;

private:
    /**
      * The deeper instance variable is used to remember the codefile to
      * be filtered.
      */
    codefile::pointer deeper;

    /**
      * The default constructor.  Do not use.
      */
    codefile_filter();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    codefile_filter(const codefile_filter &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    codefile_filter &operator=(const codefile_filter &rhs);
};

#endif // LIB_CODEFILE_FILTER_H
// vim: set ts=8 sw=4 et :
