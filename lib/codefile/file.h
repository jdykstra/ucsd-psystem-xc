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

#ifndef LIB_CODEFILE_FILE_H
#define LIB_CODEFILE_FILE_H

#include <lib/ac/vector>

#include <lib/codefile.h>
#include <lib/p_machine.h>

/**
  * The codefile_file class is used to represent an actual codefile on
  * disk.  It can be read and written.
  */
class codefile_file:
    public codefile
{
public:
    typedef boost::shared_ptr<codefile_file> pointer;

    /**
      * The destructor.
      */
    virtual ~codefile_file();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The path of the file to be read.
      */
    static pointer create(const rcstring &filename);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The path of the file to be read.
      * @param byte_sex
      *     The byte sex to be used in the new file.
      */
    static pointer create(const rcstring &filename, byte_sex_t byte_sex);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The path of the file to be read.
      * @param mtype
      *     The machine type to be used in the new file.
      */
    static pointer create(const rcstring &filename, mtype_t mtype);

protected:
    /**
      * The constructor, for openning existing codefiles.
      * For use by derived classes only.
      *
      * @param filename
      *     The path of the file to be read.
      */
    codefile_file(const rcstring &filename);

    /**
      * The constructor, for new codefiles to be created from scratch.
      * For use by derived classes only.
      *
      * @param filename
      *     The path of the file to be read.
      * @param byte_sex
      *     The byte sex to be used in the new file.
      */
    codefile_file(const rcstring &filename, byte_sex_t byte_sex);

    // See base class for documentation.
    void read_block(unsigned block_num, void *data, unsigned nblocks) const;

    // See base class for documentation.
    void write_block(unsigned block_num, const void *data, unsigned nblocks)
        const;

    // See base class for documentation.
    byte_sex_t get_byte_sex(void) const;

    // See base class for documentation.
    void set_copyright_notice(const rcstring &text);

    // See base class for documentation.
    rcstring get_copyright_notice(void) const;

    // See base class for documentation.
    rcstring get_filename(void) const;

    // See base class for documentation.
    virtual unsigned allocate(unsigned nblocks);

    enum { sizeof_block = 512 };

    /**
      * The get_size_in_blocks method may be used to obtain the number
      * of 512-byte blocks in the codefile.
      */
    unsigned get_size_in_blocks(void) const { return size_in_blocks; }

    /**
      * The is_valid_segment_name method may be used to determine
      * whether or not a segment name in a segment dictionary is valid.
      *
      * @param data
      *     The array of characatres to be checked.  It is at least 8
      *     bytes in length.
      * @returns
      *     True if acceptable, false if not.
      */
    static bool is_valid_segment_name(const char *data);

    /**
      * The set_byte_sex method may be used to set the byte sex.
      *
      * @note
      *     Use of this method is restricted to the constructors that
      *     are reading existing files.  Any use beyond that will result
      *     in an assert() failure.
      */
    void set_byte_sex(byte_sex_t bs);

    /**
      * The all_spaces class method is used to determine whether or not
      * the given data buffer is all ascii spaces.
      *
      * @param data
      *     The base address of the buffer to be tested
      * @param data_size
      *     The size of the buffer to be tested.
      */
    static bool all_spaces(const char *data, size_t data_size);

private:
    /**
      * The filename instance variable is used to remember the name of
      * the code file.
      */
    rcstring filename;

    /**
      * The fd instance variable is used to remember the file descriptor
      * of the open code file.
      */
    int fd;

    /**
      * The byte_sex instance variable is used to remember whether the
      * file is little-endian (LSB first) or big-endian (MSB first).
      */
    byte_sex_t byte_sex;

    mutable bool may_set_byte_sex;

    /**
      * The notice instance variable is used to remember the copyright
      * notice from the file, if one was present, or one is to be written.
      */
    rcstring notice;

    /**
      * The size_in_blocks instance variable is used to remember the
      * size of the code file in blocks.  This is useful when it come
      * time to grow the codefile.
      */
    unsigned size_in_blocks;

    static p_machine_t guess_the_release(const rcstring &filename);

    static bool kinda_close(const char *buffer);

    /**
      * The default constructor.  Do not use.
      */
    codefile_file();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    codefile_file(const codefile_file &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    codefile_file &operator=(const codefile_file &rhs);
};

#endif // LIB_CODEFILE_FILE_H
// vim: set ts=8 sw=4 et :
