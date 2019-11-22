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

#ifndef LIB_CODEFILE_H
#define LIB_CODEFILE_H

#include <boost/shared_ptr.hpp>

#include <lib/byte_sex.h>
#include <lib/output.h>
#include <lib/rcstring.h>
#include <lib/segment.h>

class disassembler; // forward
class discolumns; // forward
class dislabel; // forward
class code_source_map; // forward

/**
  * The codefile abstract base class is used to represent a UCSD
  * p-System codefile.  It could be an actual codefile on disk, or it
  * could be filtered in different ways.
  */
class codefile
{
public:
    typedef boost::shared_ptr<codefile> pointer;
    typedef boost::shared_ptr<disassembler> disassembler_pointer;

    enum { sizeof_block = 512 };

    /**
      * The destructor.
      */
    virtual ~codefile();

    /**
      * The print_library_map method is used to print a map of the code
      * file onto the given output.
      *
      * @param os
      *     write the map text onto this output
      */
    void print_library_map(output::pointer &os);

    /**
      * The disassemble method is used to print a disassembly of the
      * codefile onto the given output.
      *
      * @param os
      *     write the map text onto this output
      * @param flags
      *     0x01 -> no addresses, 0x02 -> opcode comments
      */
    void disassemble(output::pointer &os, int flags);

    /**
      * The copy_from method is used to copy all of another codefile's
      * segments and properties into this codefile.  This is used,
      * for example, by the librarian when manipulating codefiles.
      *
      * @param src
      *     The source codefile from which to copy.
      */
    void copy_from(const codefile::pointer &src);

    /**
      * The read_block method is used to read a block of data from the
      * file.
      *
      * @param block_num
      *     The block number to be read (zero based).
      * @param data
      *     where to put the data once read.
      * @param nblocks
      *     now many blocks to read (defaults to just 1 block)
      */
    virtual void read_block(unsigned block_num, void *data,
        unsigned nblocks = 1) const = 0;

    /**
      * The write_block method is used to write a block of data to the
      * file.
      *
      * @param block_num
      *     The block number to be read (zero based).
      * @param data
      *     where to get the data to write.
      * @param nblocks
      *     now many blocks to write (defaults to just 1 block)
      */
    virtual void write_block(unsigned block_num, const void *data,
        unsigned nblocks = 1) const = 0;

    /**
      * The get_word method is used to decode a two-byte word, taking
      * byte_sex into account.
      *
      * @param data
      *     pointer to two bytes of data
      * @returns
      *     unsigned 16-bit value
      */
    unsigned get_word(const unsigned char *data);

    /**
      * The put_word method is used to encode a two-byte word, taking
      * byte_sex into account.  It doesn't write to the codefile directly.
      *
      * @param data
      *     where to write the encoded value
      * @param value
      *     the value to be encoded
      */
    void put_word(unsigned char *data, unsigned value) const;

    /**
      * The get_byte_sex method is used to get the byte sex of the codefile.
      */
    virtual byte_sex_t get_byte_sex(void) const = 0;

    /**
      * The new_segment method is used to create a new segment in the
      * codefile.  While it has a name and a number, it does not yet
      * have a fixed location in the file, that only happens when the
      * segment is written, via the segment::write method.
      *
      * @param name
      *      The name of the new segment
      * @param syscomp
      *      If true, system segments may be used.  If false, system
      *      segments are not to be used
      */
    virtual segment::pointer new_segment(const rcstring &name, bool syscomp);

    /**
      * The new_segment method is used to create a new segment in the
      * codefile.  While it has a name and a number, it does not yet
      * have a fixed location in the file, that only happens when the
      * segment is written.
      *
      * @param name
      *      The name of the new segment
      * @param segnum
      *      The number of the new segment
      * @param segkind
      *      The kind of the new segment
      * @param mtype
      *      The mtype of the new segment.
      * @param version
      *      The segment meta-data version.
      */
    virtual segment::pointer new_segment(const rcstring &name, int segnum,
        segkind_t segkind, mtype_t mtype, segversion_t version) = 0;

    /**
      * The new_segment method is used to create a new segment in the
      * codefile.  While it has a name and a number, it does not yet
      * have a fixed location in the file, that only happens when the
      * segment is written.
      *
      * @param name
      *      The name of the new segment
      * @param segnum
      *      The number of the new segment
      * @param segkind
      *      The kind of the new segment
      */
    segment::pointer new_segment(const rcstring &name, int segnum,
        segkind_t segkind);

    /**
      * The allocate method is used to allocate more space in
      * the codefile.  It is usually used to allocate new segments.
      *
      * @param nblocks
      *     The number of blocks to allocate in the codefile.
      * @returns
      *     The block number of the first block in the allocated area.
      */
    virtual unsigned allocate(unsigned nblocks) = 0;

    /**
      * The write_segment_dictionary method is used to write out a new
      * copy of the segment dictionary (the first block of the codefile).
      */
    virtual void write_segment_dictionary(void) = 0;

    /**
      * The set_copyright_notice method is used to set the copyright
      * notice for a new codefile generated by the compiler.
      *
      * @param text
      *     The text of the copyright message,
      *     limited to 78 characters.
      */
    virtual void set_copyright_notice(const rcstring &text) = 0;

    /**
      * The get_copyright_notice method is used to obtain the text of
      * the copyright notice embedded in the codefile.
      *
      * @returns
      *     a string, empty if there is not notice in the file.
      */
    virtual rcstring get_copyright_notice(void) const = 0;

    /**
      * The get_filename method is used to obtain the name of the
      * on-disk file associated with this codefile.
      */
    virtual rcstring get_filename(void) const = 0;

    /**
      * The get_segment method is used to obtain a pointer to the
      * number'th segment.  Zero based.  No holes.
      *
      * @param number
      *     The number of the segment.  This is NOT the segment number,
      *     just an array index.  Use #get_segment_by_number if you want
      *     the segment with a particular segment number.
      * @return
      *     pointer to segment, or NULL if no more segments.
      *
      * @par
      * To iterate over all of the segments in a codefile, use
      * @code
      * for (unsigned n = 0; ; ++n)
      * {
      *     segment::pointer sp = cfp->get_segment(n);
      *     if (!sp)
      *         break;
      *     ...your loop body here...
      * }
      * @endcode
      */
    virtual segment::pointer get_segment(unsigned number) const = 0;

    /**
      * The get_segment_by_name method is used to obtain a pointer to
      * the segment of the given name.
      *
      * @param segname
      *     The name of the segment of interest.
      * @return
      *     pointer to segment, or NULL if not present
      */
    segment::pointer get_segment_by_name(const rcstring &segname) const;

    /**
      * The get_segment_by_name method is used to obtain a pointer to
      * the segment of the given name.
      *
      * @param segname
      *     The name of the segment of interest.
      * @param segkind
      *     The specific segment kind of interest.
      * @return
      *     pointer to segment, or NULL if not present
      */
    segment::pointer get_segment_by_name(const rcstring &segname,
        segkind_t segkind) const;

    /**
      * The get_segment_by_number method is used to obtain a pointer to
      * the segment of the given number.
      *
      * @param segnum
      *     The number of the segment of interest.
      * @return
      *     pointer to segment, or NULL if not present
      */
    segment::pointer get_segment_by_number(unsigned segnum) const;

    /**
      * The next_free_segment_number method is used to obtain the next
      * free (unused) segment number.
      *
      * @param syscomp
      *     If true, system segment numbers may be used.  If false,
      *     system segment numbers shall not be used.
      * @param is_a_unit
      *     True if a UNIT is being compiled
      * @returns
      *     int; the segment number (0..63), or -1 if no more room.
      */
    int next_free_segment_number(bool syscomp = false, bool is_a_unit = false);

    /**
      * The create_dataseg method is used to create the external DATASEG
      * associated with an intrinsic unit.
      *
      * @param name
      *     The name of the segment to be created.  It will be the same
      *     as the associated code segment.
      * @param segnum
      *     The number of the data segment.
      * @param size_in_words
      *     The size of the data segment to be created, in units of
      *     words (two bytes).
      */
    virtual void create_dataseg(const rcstring &name, int segnum,
        int size_in_words) = 0;

    /**
      * The find_segment_by_unit_name method is used to look for the
      * named segment (segkind == UNITSEG).
      *
      * @param name
      *     The name of the unit to search for.
      * @return
      *     a pointer to the segment, or NULL if no such unit
      *     (there could be a segment of that name, but not a UNITSEG).
      */
    segment::pointer find_segment_by_unit_name(const rcstring &name) const;

    /**
      * The maximum_number_of_segments method is used to obtain the
      * maximum number of segments capable of being used by the system,
      * or (n-1) being used as a segment number.
      * It varies according to p-machine release number, mostly.
      */
    virtual unsigned maximum_number_of_segments(void) const = 0;

    /**
      * The new_file_factory method is used to manufacture a new
      * instance of the same class, for creating a new codefile scratch,
      * with all the same properties as this codefile.
      *
      * @param filename
      *     The name of the new codefile to be created.
      */
    virtual codefile::pointer new_file_factory(const rcstring &filename)
        const = 0;

    /**
      * The native_disassembler_factory method is used to create a new
      * dynamically allocated instance of the appropriate disassembler
      * class to disassmble native procedures in this codefile.
      *
      * @param mtype
      *     The machine type of the segment to be disassembled.
      * @param where
      *     A database of addresses and corresponding label names.
      * @param report
      *     The columnar output for writing out the disassembled code.
      * @param seg_num
      *     The segment number of the segment being disassembled.
      * @param proc_num
      *     The procedure number of the procedure being disassembled.
      * @param proc_base
      *     The base address of the procedure being disassembled.
      * @param listing
      *     the distination for the listing, or NULL if no source code
      *     to be interleaved.
      */
    virtual disassembler_pointer native_disassembler_factory(mtype_t mtype,
        dislabel &where, discolumns &report, int seg_num, int proc_num,
        int proc_base, code_source_map *listing) const = 0;

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    codefile();

private:
    unsigned size_in_blocks;

    /**
      * The copy constructor.  Do not use.
      */
    codefile(const codefile &);

    /**
      * The assignment operator.  Do not use.
      */
    codefile &operator=(const codefile &);
};

#endif // LIB_CODEFILE_H
// vim: set ts=8 sw=4 et :
