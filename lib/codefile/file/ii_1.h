//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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

#ifndef LIB_CODEFILE_FILE_II_1_H
#define LIB_CODEFILE_FILE_II_1_H

#include <lib/codefile/file.h>

/**
  * The codefile_file_ii_1 class is used to represent an actual codefile on
  * disk, using the segment dictionary layout used by the II.1 p-machine.  It
  * can be read and written.
  */
class codefile_file_ii_1:
    public codefile_file
{
public:
    typedef boost::shared_ptr<codefile_file> pointer;

    /**
      * The destructor.
      */
    virtual ~codefile_file_ii_1();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class, for codefiles that already exist.
      *
      * @param filename
      *     The path of the file to be read.
      */
    static pointer create(const rcstring &filename);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class, for codefiles that are to be
      * created from scratch.
      *
      * @param filename
      *     The path of the file to be read.
      * @param byte_sex
      *     The byte sex to be used in the new file.
      */
    static pointer create(const rcstring &filename, byte_sex_t byte_sex);

    /**
      * The candidate class method is used to determine wheter or not
      * the block given, the first block of the codefile, containg the
      * segment dictionary, looks like a II.1 codefile.
      *
      * @param block0
      *     The data of the first 512-byte block of the file.
      * @returns
      *     true if it looks right, false if it does not.
      */
    static bool candidate(const void *block0);

protected:
    // See base class for documentation.
    segment::pointer new_segment(const rcstring &name, int segnum,
        segkind_t segkind, mtype_t mtype, segversion_t version);

    // See base class for documentation.
    void write_segment_dictionary(void);

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
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param filename
      *     The name of the existing codefile to be opened.
      */
    codefile_file_ii_1(const rcstring &filename);

    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param filename
      *     The name of the new codefile to be created.
      * @param byte_sex
      *     The byte sex to be used in the new file.
      */
    codefile_file_ii_1(const rcstring &filename, byte_sex_t byte_sex);

    typedef std::vector<segment::pointer> segments_t;

    /**
      * The segments instance variable is used to remember the segments in the
      * codefile.  Segment numbers are required to be in the range 0..63, but
      * only 16 at a time can be placed into any one codefile.
      */
    segments_t segments;

    /**
      * The default constructor.  Do not use.
      */
    codefile_file_ii_1();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    codefile_file_ii_1(const codefile_file_ii_1 &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    codefile_file_ii_1 &operator=(const codefile_file_ii_1 &rhs);

    //
    // The UCSD 16-bit Pascal (II.1) declarations for the segment
    // dictionary look like this...
    //
    // CONST
    //   MAXSEG = 15;
    // TYPE
    //   SEGRANGE = 0..MAXSEG;
    //   SEGDESC =
    //     RECORD
    //       DISKADDR: INTEGER;
    //       CODELENG: INTEGER
    //     END;
    //   SEGKIND_T =
    //     (LINKED, HOSTSEG, SEGPROC, UNITSEG, SEPRTSEG,
    //     UNLINKED_INTRINS, LINKED_INTRINS, DATASEG);
    //   SEGINFO_T =
    //     PACKED RECORD
    //       SEGNUM: 0..255
    //       MTYPE: 0..15;
    //       UNUSED: 0..1;
    //       VERSION: 0..7;
    //     END;
    //   SEGDICT =
    //     RECORD
    //       SEGDSC: ARRAY [SEGRANGE] OF SEGDESC;   // 16 * 4 =  64 bytes
    //       SEGNAME: ARRAY [SEGRANGE] OF
    //                 PACKED ARRAY [0..7] OF CHAR; // 16 * 8 = 128 bytes
    //       SEGKIND: ARRAY [SEGRANGE] OF INTEGER;  // 16 * 2 =  32 bytes
    //       TEXTSTART: ARRAY[SEGRANGE] OF INTEGER; // 16 * 2 =  32 bytes
    //       SEGINFO: ARRAY[SEGRANGE] OF SEGINFO_T; // 16 * 2 =  32 bytes
    //       INTRINSEGS: SET OF 0..63;              //            8 bytes
    //       FILLER: ARRAY [1..68] OF INTEGER;      //          136 bytes
    //       NOTICE: STRING[79]                     //           80 bytes
    //     END;                                     //   Total: 512 bytes
    //
    // Don't forget: little-endian and big-endian issues make all of the
    // integers and bit-packing crazy-making.
    //
    struct block0
    {
        void clear(void);
        unsigned char diskinfo[16][2][2];
        char segname[16][8];
        unsigned char segkind[16][2];
        unsigned char textaddr[16][2];
        unsigned char seginfo[16][2];
        unsigned char intrin_segs[8];
        unsigned char filler[68 * 2];
        char notice[80];
    };
    // assert(sizeof(blk0) == 512);

    static bool looks_like_valid_segment_table(const block0 &buffer,
        byte_sex_t byte_sex);
};

#endif // LIB_CODEFILE_FILE_II_1_H
// vim: set ts=8 sw=4 et :
