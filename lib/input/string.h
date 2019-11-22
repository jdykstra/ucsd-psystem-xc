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

#ifndef LIB_INPUT_STRING_H
#define LIB_INPUT_STRING_H

#include <lib/input.h>

/**
  * The input_string class is used to represent input that comes from
  * aan in-memory string.
  */
class input_string:
    public input
{
public:
    typedef boost::shared_ptr<input_string> pointer;

    /**
      * The destructor.
      */
    virtual ~input_string();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param text
      *     The text that is to be input.
      * @param filename
      *     The pretend filename we are calling this input.
      */
    input_string(const rcstring &text, const rcstring &filename);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param text
      *     The text that is to be input.
      * @param filename
      *     The pretend filename we are calling this input.
      */
    static pointer create(const rcstring &text, const rcstring &filename);

protected:
    // See base class for documentation.
    rcstring name();

    // See base class for documentation.
    long length();

    // See base class for documentation.
    int fpathconf_name_max();

    // See base class for documentation.
    void fstat(struct stat &);

    // See base class for documentation.
    long read_inner(void *data, size_t nbytes);

private:
    /**
      * The text instance variable is used to remember the text we are
      * delivering as input.
      */
    rcstring text;

    /**
      * The filename instance variable is used to remember the
      * pesudo-filename we are calling this input.
      */
    rcstring filename;

    /**
      * The pos instance variable is used to remember the current
      * position within the text.
      */
    size_t pos;

    /**
      * The default constructor.  Do not use.
      */
    input_string();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    input_string(const input_string &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    input_string &operator=(const input_string &rhs);
};

#endif // LIB_INPUT_STRING_H
// vim: set ts=8 sw=4 et :
