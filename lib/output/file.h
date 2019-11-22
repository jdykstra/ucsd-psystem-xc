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

#ifndef LIB_OUTPUT_FILE_H
#define LIB_OUTPUT_FILE_H

#include <lib/output.h>
#include <lib/rcstring.h>


/**
  * The output_file class is used to represent the state of an output
  * stream to a regular file.
  */
class output_file:
    public output
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file();

private:
    /**
      * The constructor
      *
      * @param file_name
      *     The name of the file to be opened for output.
      *     (Make absolutely sure it is not NULL or empty.)
      * @param binary
      *     Whether the file is binary (true) or text (false).
      */
    output_file(const rcstring &file_name, bool binary);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be opened for output.
      *     (Make absolutely sure it is not NULL or empty.)
      * @param binary
      *     Whether the file is binary (true) or text (false).
      */
    static pointer create(const rcstring &file_name, bool binary = false);

    // See base class for documentation.
    rcstring filename();

    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    void flush_inner();

    // See base class for documentation.
    void utime(struct utimbuf &utb);

    // See base class for documentation.
    unsigned get_width(void) const;

    // See base class for documentation.
    unsigned get_height(void) const;

private:
    /**
      * The file_name instance variable is used to remember the name of
      * the file opened for writing.
      */
    rcstring file_name;

    /**
      * The fd instance variable is used to remember the number of the
      * file descriptor opened for writing.
      */
    int fd;

    /**
      * The bol instance variable is used to remember whether or not we
      * are at the start of a line.
      */
    bool bol;

    /**
      * The pos instance variable is used to remember the current
      * position in the output file.
      */
    size_t pos;

    /**
      * The width of the output file, in character columns (assumes
      * fixed pitch font, like a VDU terminal).
      */
    int width;

    /**
      * The height of the output file, in lines of text.
      */
    int height;

    /**
      * The default constructor.  Do not use.
      */
    output_file();

    /**
      * The copy constructor.  Do not use.
      */
    output_file(const output_file &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file &operator=(const output_file &);
};

#endif // LIB_OUTPUT_FILE_H
// vim: set ts=8 sw=4 et :
