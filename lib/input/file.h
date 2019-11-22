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

#ifndef LIB_INPUT_FILE_H
#define LIB_INPUT_FILE_H

#include <lib/input.h>

/**
  * The input_file class is used to represent an input stream which is
  * read from a file.
  */
class input_file:
    public input
{
public:
    typedef boost::shared_ptr<input_file> pointer;

    /**
      * The destructor.
      */
    virtual ~input_file();

private:
    /**
      * The constructor.
      *
      * It is private on purpose, use the #create class method instead.
      *
      * @param path
      *     The path of the file to be opened.
      */
    input_file(const rcstring &path);

public:
    /**
      * The create class is used to create new dynamically allocated
      * instances of this class.
      *
      * @param path
      *     The path of the file to be opened.
      */
    static pointer create(const rcstring &path);

    // See base class for documentation.
    rcstring name();

    // See base class for documentation.
    long length();

    // See base class for documentation.
    long read_inner(void *data, size_t nbytes);

    // See base class for documentation.
    void fstat(struct stat &);

    // See base class for documentation.
    int fpathconf_name_max();

private:
    rcstring path;
    int fd;
    long pos;

    /**
      * The default constructor.  Do not use.
      */
    input_file();

    /**
      * The copy constructor.  Do not use.
      */
    input_file(const input_file &arg);

    /**
      * The assignment operator.  Do not use.
      */
    input_file &operator=(const input_file &arg);
};

#endif // LIB_INPUT_FILE_H
// vim: set ts=8 sw=4 et :
