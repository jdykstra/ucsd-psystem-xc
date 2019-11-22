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

#ifndef LIB_INPUT_UCSD_TEXT_H
#define LIB_INPUT_UCSD_TEXT_H

#include <lib/input.h>

/**
  * The input_ucsd_text class is used to represent the processing
  * necessary to turn a raw UCSD p-system text file into (what looks
  * like) natural Unix text.
  */
class input_ucsd_text:
    public input
{
public:
    typedef boost::shared_ptr<input_ucsd_text> pointer;

    /**
      * The destructor.
      */
    virtual ~input_ucsd_text();

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param path
      *     the path of the input file to be opened for reading.
      */
    input_ucsd_text(const rcstring &path);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param path
      *     the path of the input file to be opened for reading.
      */
    static pointer create(const rcstring &path);

    /**
      * The candidate class method is used to examine a pathname, and
      * open a input_ucsd_text instance if it appears to contain valid
      * raw UCSD text file contents.  It returns NULL otherwise.  This
      * method is used by input::factory to distinguish different types
      * of input.
      *
      * @param path
      *     the path of the input file to be opened for reading.
      */
    static pointer candidate(const rcstring &path);

protected:
    // See base class for documentation.
    rcstring name(void);

    // See base class for documentation.
    long length(void);

    // See base class for documentation.
    int fpathconf_name_max(void);

    // See base class for documentation.
    void fstat(struct stat &);

    // See base class for documentation.
    long read_inner(void *data, size_t nbytes);

private:
    /**
      * The path instance variable is used to remember the pathname of
      * the open file.
      */
    rcstring path;

    /**
      * The fd instance variable is used to remember the open file
      * descriptor for the file.  It will be -1 whenn it is not valid.
      */
    int fd;

    /**
      * The pos instance variable is used to remember the position
      * within the file.  Note that this is a text position, within the
      * decoded text, it has nothing to do with the file position in the
      * underlying Unix file.
      */
    unsigned pos;

    /**
      * The block instance variable is used to remember the data read
      * from the most recent read of the underlying binary file.
      */
    unsigned char block[1024];

    /**
      * The block_size instance variable is used to remember the upper
      * bound of the data in the #block instance variable.  It is
      * adjusted downwards to discard the NUL padding that UCSD p-system
      * text files have at the end of each 1kB block.
      *
      * assert(block_size <= sizeof(block))
      */
    unsigned block_size;

    /**
      * The block_posn instance variable is used to remember where to
      * read the next character from in the #block data.
      *
      * assert(block_posn <= block_size);
      */
    unsigned block_posn;

    /**
      * The dle_seen instance variable is used to remember the number
      * of space charactres remaining to be inserted as a result of the
      * most recent DLE character.
      */
    unsigned dle_seen;

    /**
      * The format_error method is used to issue a fatal error if the
      * file is not a valid raw UCSD text file.
      */
    void format_error(void);

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    input_ucsd_text(const input_ucsd_text &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    input_ucsd_text &operator=(const input_ucsd_text &rhs);
};

#endif // LIB_INPUT_UCSD_TEXT_H
// vim: set ts=8 sw=4 et :
