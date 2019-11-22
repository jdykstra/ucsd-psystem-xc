//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#ifndef LIB_SEGMENT_H
#define LIB_SEGMENT_H

#include <boost/shared_ptr.hpp>

#include <lib/byte_sex.h>
#include <lib/link_info/list.h>
#include <lib/library_map_columns.h>
#include <lib/mtype.h>
#include <lib/segkind.h>
#include <lib/segversion.h>

class codefile; // forward

/**
  * The segment class is used to represent a segment in a code file.
  * Usually, this is a code segment, but not always.
  */
class segment
{
public:
    typedef boost::shared_ptr<segment> pointer;

    /**
      * The destructor.
      */
    virtual ~segment();

protected:
    /**
      * The default constructror.
      * For use by derived classes only.
      */
    segment();

public:
    /**
      * The print_library_map method is used to print a map of this code
      * segment onto the standard output.
      *
      * @param columns
      *     The automatic columns formatting to use
      */
    void print_library_map(library_map_columns &columns) const;

    /**
      * The disassemble method is used to print a map of this code
      * segment onto the standard output.
      *
      * @param os
      *     write the text map to this output
      * @param flags
      *   0x01
      *     True if there should be no address column in the output.
      *     The is promarily for testing, so that single add or removed
      *     opcodes show in the diff listing, rather than having the
      *     whole output being different because all the lines now have
      *     a different address.
      *   0x02
      *     add a comment describing each opcode
      * @param cfp
      *     Pointer to containing codefile.
      */
    void disassemble(output::pointer &os, int flags, codefile *cfp) const;

    /**
      * The get_kind method is used to obtain the kind of segment this
      * is.
      */
    virtual segkind_t get_kind(void) const = 0;

    virtual void set_kind(segkind_t value) = 0;

    virtual mtype_t get_mtype(void) const = 0;

    virtual void set_mtype(mtype_t value) = 0;

    virtual segversion_t get_version(void) const = 0;

    virtual void set_version(segversion_t value) = 0;

    virtual const link_info_list &get_link_info(void) const = 0;

    /**
      * The get_segment_number method is used to obtain the number of
      * this segment.
      */
    virtual unsigned get_segment_number(void) const = 0;

    /**
      * Some segments also have link information following their code.
      * The get_linkinfo_nblocks method may be used to obtain the number
      * of blocks of link information associated with this segment.
      */
    virtual unsigned get_linkinfo_nblocks(void) const = 0;

    /**
      * The get_text_address method is used to write the UNIT interface text
      * to the codefile, and track the block address of the interface text,
      * for later use when writing the segment dictionary of the codefile.
      *
      * @param text
      *     The text falling between the INTERFACE and IMPLEMENTATION
      *     ketwords (excluding both keywords).  Case is preserved, as
      *     are comments.
      */
    virtual unsigned get_text_address(const rcstring &text) = 0;

    /**
      * The get_code_address is used when constructing a new code
      * segment, to allocate space for the imminent block_write calls.
      *
      * @param code_length
      *     The size of the segment in bytes, for insertion into the
      *     segment dictionary.
      * @returns
      *     The block address of the code, if present.
      *     If there is no code, it returns zero.
      */
    virtual unsigned get_code_address(unsigned code_length) = 0;

    /**
      * The write_block method is used to write data into the code segment.
      *
      * @param blknum
      *     The block number to start wring at.  The codeaddr will be
      *     added before it is written into the codefile.
      * @param data
      *     The data to be written
      * @param nblocks
      *     The number of blocks to write
      */
    virtual void write_block(unsigned blknum, const void *data,
        unsigned nblocks) const = 0;

    /**
      * The read_block method is used to read data from the code segment.
      *
      * @param blknum
      *     The block number to start wring at.  The codeaddr will be
      *     added before it is written into the codefile.
      * @param data
      *     where the data is to be read into
      * @param nblocks
      *     The number of blocks to read
      */
    virtual void read_block(unsigned blknum, void *data, unsigned nblocks)
        const = 0;

    /**
      * The get_code_address method is used to obtain the block number
      * (address) of the location of the segment within the codefile.
      */
    virtual unsigned get_code_address(void) const = 0;

    /**
      * The get_text_address method is used to obtain the block address of the
      * location of the segment's interface text within the codefile.
      */
    virtual unsigned get_text_address(void) const = 0;

    /**
      * The get_code_length method is used to obtain the number of
      * blocks in this segment.
      */
    virtual unsigned get_code_length(void) const = 0;

    /**
      * The get_name method is used to obtain the name of this segment.
      */
    virtual rcstring get_name(void) const = 0;

    /**
      * The copy_to method is used to create a new segment in the
      * destination, and copy the segments contents into the new segment.
      */
    void copy_to(codefile *destination) const;

    /**
      * The get_interface_text method is used to get the text of the
      * INTERFACE section of the segment.  This is present for UNIT
      * segments in libraries.
      */
    virtual rcstring get_interface_text(unsigned block_num, int block_len)
        const = 0;

    /**
      * The get_interface_text method is used to get the text of the
      * INTERFACE section of the segment.  This is present for UNIT
      * segments in libraries.
      */
    virtual rcstring get_interface_text(void) const = 0;

    /**
      * The put_word method is used to encode two bytes of data from a
      * 16-bit word value, taking the parent codefile's byte sex into
      * account.
      *
      * @param data
      *     Pointer to the two bytes to be decoded.
      * @param value
      *     the 16-bit value to deconstruct.
      *
      * @note
      *     The data isn't stored into the segment, it is stored into the
      *     supplied buffer.
      */
    void put_word(unsigned char *data, unsigned value) const;

    unsigned get_word(const unsigned char *data) const;

    virtual byte_sex_t get_byte_sex(void) const = 0;

    /**
      * The segment_not_linked method is used in reponse to the EXTERNAL
      * keyword to tell a segment under construction that it is to use
      * an unlinked segkind, rather than a linked segkind.
      *
      * @param link_info_entry
      *     The accompanying link_info for use by the linker.
      *     It will either be EXTPROC or EXTFUNC.
      */
    virtual void segment_not_linked(const link_info::pointer &link_info_entry)
        = 0;

    /**
      * The add_link_information method is used to add additional link
      * information to ta segment under construction.
      *
      * @param link_info_entry
      *     The link_info for use by the linker.
      */
    virtual void add_link_information(const link_info::pointer &link_info_entry)
        = 0;

    /**
      * The write_link_information method is used to write the linker
      * information to the codefile.  It is essential that this is done
      * after the codeaddr and codeleng are set.
      *
      * @param nextlc
      *     where to allocate GLOBDEFs.
      */
    virtual void write_link_information(int nextlc) const = 0;

private:
    /**
      * The copy constructor.  Do not use.
      */
    segment(const segment &);

    /**
      * The assignment operator.  Do not use.
      */
    segment &operator=(const segment &);
};

#endif // LIB_SEGMENT_H
// vim: set ts=8 sw=4 et :
