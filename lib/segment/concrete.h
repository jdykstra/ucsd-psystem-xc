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

#ifndef LIB_SEGMENT_CONCRETE_H
#define LIB_SEGMENT_CONCRETE_H

#include <lib/segment.h>

class codefile; // forward

/**
  * The segment class is used to represent a code segment in a code file.
  */
class segment_concrete:
    public segment
{
public:
    typedef boost::shared_ptr<segment_concrete> pointer;

    /**
      * The destructor.
      */
    virtual ~segment_concrete();

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param parent
      *     the codefile this segment_concrete is part of.
      * @param codeleng
      *     how long the code of this segment_concrete is, in blocks.
      * @param codeaddr
      *     the block number of the start of the code of this segment_concrete,
      *     in blocks.
      * @param name
      *     the name of this segment_concrete.
      * @param segkind
      *     the kind of this segment_concrete.
      * @param textstart
      *     the block number of the INTERFACE section of this UNIT
      *     segment_concrete, as text.  Zero if not meaningful.
      * @param linkinfo_nblocks
      *     The number of blocks of link info that occur after the code
      *     blocks.
      * @param segnum
      *     The number of the segment_concrete.
      * @param mtype
      *     The mtype of the segment_concrete.
      * @param version
      *     The version of the segment_concrete.
      */
    static pointer create(codefile *parent, unsigned codeleng,
        unsigned codeaddr, const rcstring &name, segkind_t segkind,
        unsigned textstart, unsigned linkinfo_nblocks, unsigned segnum,
        mtype_t mtype, segversion_t version);

protected:
    // See base class for documentation.
    unsigned get_codeleng(void) const;

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
    unsigned get_segment_number(void) const;

    // See base class for documentation.
    unsigned get_text_address(const rcstring &text);

    // See base class for documentation.
    unsigned get_code_address(unsigned code_length);

    // See base class for documentation.
    void write_block(unsigned blknum, const void *data, unsigned nblocks) const;

    // See base class for documentation.
    void read_block(unsigned blknum, void *data, unsigned nblocks) const;

    // See base class for documentation.
    unsigned get_code_address(void) const;

    // See base class for documentation.
    unsigned get_linkinfo_nblocks(void) const;

    /**
      * The get_text_address method is used to obtain the block address of the
      * location of the segment_concrete's interface text within the codefile.
      */
    unsigned get_text_address(void) const;

    /**
      * The get_code_length method is used to obtain the number of
      * blocks in this segment_concrete.
      */
    unsigned get_code_length(void) const;

    unsigned get_text_start(void) const;

    /**
      * The get_name method is used to obtain the name of this segment_concrete.
      */
    rcstring get_name(void) const { return name; }

    /**
      * The copy_to method is used to create a new segment_concrete in
      * the destination, and copy the segments contents into the new
      * segment_concrete.
      */
    void copy_to(codefile *destination) const;

    /**
      * The get_interface_text method is used to get the text of the
      * INTERFACE section of the segment_concrete.  This is present for UNIT
      * segments in libraries.
      */
    rcstring get_interface_text(unsigned block_num, int block_len) const;

    /**
      * The get_interface_text method is used to get the text of the
      * INTERFACE section of the segment_concrete.  This is present for UNIT
      * segments in libraries.
      */
    rcstring get_interface_text(void) const;

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
      *     The data isn't sored into the segment_concrete, it is stored
      *     into the supplied buffer.
      */
    void put_word(unsigned char *data, unsigned value);

    byte_sex_t get_byte_sex(void) const;

    void attach_interface_text(const rcstring &text);

    /**
      * The segment_not_linked method is used in reponse to the EXTERNAL
      * keyword to tell a segment_concrete under construction that it is to use
      * an unlinked segkind, rather than a linked segkind.
      *
      * @param link_info_entry
      *     The accompanying link_info for use by the linker.
      *     It will either be EXTPROC or EXTFUNC.
      */
    void segment_not_linked(const link_info::pointer &link_info_entry);

    /**
      * The add_link_information method is used to add additional link
      * information to ta segment_concrete under construction.
      *
      * @param link_info_entry
      *     The link_info for use by the linker.
      */
    void add_link_information(const link_info::pointer &link_info_entry);

    // See base class fdor documentation.
    void write_link_information(int nextlc) const;

    // See base class fdor documentation.
    const link_info_list &get_link_info(void) const;

private:
    /**
      * The constructor.
      *
      * @param parent
      *     the codefile this segment_concrete is part of.
      * @param codeleng
      *     how long the code of this segment_concrete is, in blocks.
      * @param codeaddr
      *     the block number of the start of the code of this segment_concrete,
      *     in blocks.
      * @param name
      *     the name of this segment_concrete.
      * @param segkind
      *     the kind of this segment_concrete.
      * @param textstart
      *     the block number of the INTERFACE section of this UNIT
      *     segment_concrete, as text.  Zero if not meaningful.
      * @param linkinfo_nblocks
      *     The number of blocks of link info that occur after the code
      *     blocks.
      * @param segnum
      *     The number of the segment_concrete.
      * @param mtype
      *     The mtype of the segment_concrete.
      * @param version
      *     The version of the segment_concrete.
      */
    segment_concrete(codefile *parent, unsigned codeleng, unsigned codeaddr,
        const rcstring &name, segkind_t segkind, unsigned textstart,
        unsigned linkinfo_nblocks, unsigned segnum, mtype_t mtype,
        segversion_t version);

    /**
      * The parent instance variable is used to remember the code file
      * this segment_concrete is part of.
      *
      * This is used to actually read and write the file.
      *
      * It can't be a smart pointer, or we get a self-sustaining
      * reference loop, resulting in a memory leak.
      */
    codefile *parent;

    /**
      * The codeleng instance variable is used to remember how long the
      * code of this segment_concrete is, in blocks.
      */
    unsigned codeleng;

    /**
      * The codeaddr instance variable is used to remember the block
      * number of the start of the code of this segment_concrete, in blocks.
      */
    unsigned codeaddr;

    /**
      * The name instance variable is used to remember the name of this
      * segment_concrete.
      */
    rcstring name;

    /**
      * The segkind instance variable is used to remember the kind of
      * this segment_concrete.
      */
    segkind_t segkind;

    /**
      * The textstart instance variable is used to remember the block
      * number of the INTERFACE section of this UNIT segment_concrete, as text.
      * Zero if not meaningful.
      */
    unsigned textstart;

    /**
      * The linkinfo_nblocks instance variable is used to remember the
      * number of blocks of link info that follow the code blocks.
      */
    unsigned linkinfo_nblocks;

    /**
      * The segnum instance variable is used to remember the number of
      * this segment_concrete (0..63).
      */
    unsigned segnum;

    /**
      * The mtype instance variable is used to remember the machine type
      * of this segment_concrete.
      */
    mtype_t mtype;

    /**
      * The version instance variable is used to remember the system
      * version (of the system which created this codefile) of this
      * segment_concrete.
      */
    segversion_t version;

    /**
      * The link_infos instance variable is used to remember the symbol
      * information (symbol definitions and references) to be used when
      * the linker is putting it all together.
      */
    link_info_list link_infos;

    /**
      * The read_link_information method is used to read the linker
      * information, if there is any, that is need by the linker to glue
      * code together.
      */
    void read_link_information(void);

    /**
      * The read_interface_text method is used to read the interface
      * text of the segment_concrete.  This is used by the compiler so that it
      * knows the types and procedure numbers.
      */
    void read_interface_text(void);

    /**
      * The interface_text instance variable is used to remember the
      * text between the INTERFACE and IMPLEMENTATION keywords (not
      * including either keyword).
      */
    rcstring interface_text;

    /**
      * The default constructor.  Do not use.
      */
    segment_concrete();

    /**
      * The copy constructor.  Do not use.
      */
    segment_concrete(const segment_concrete &);

    /**
      * The assignment operator.  Do not use.
      */
    segment_concrete &operator=(const segment_concrete &);
};

#endif // LIB_SEGMENT_CONCRETE_H
// vim: set ts=8 sw=4 et :
