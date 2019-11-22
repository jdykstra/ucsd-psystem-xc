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

#ifndef LIB_INPUT_DIVERT_H
#define LIB_INPUT_DIVERT_H

#include <lib/input.h>
#include <lib/output.h>

/**
  * The input_divert class is used to represent an input class that
  * siphons off a copy of the data to an output stream, before returning
  * it to the caller.
  */
class input_divert:
    public input
{
public:
    typedef boost::shared_ptr<input_divert> pointer;

    /**
      * The destructor.
      */
    virtual ~input_divert();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The deeper source of input.
      * @param copy
      *     Where to send the duplicate copy of the input.
      */
    static pointer create(const input::pointer &deeper,
        const output::pointer &copy);

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
      * The constructor.
      *
      * @param deeper
      *     The deeper source of input.
      * @param copy
      *     Where to send the duplicate copy of the input.
      */
    input_divert(const input::pointer &deeper, const output::pointer &copy);

    /**
      * The deeper instance variable is used to remember the deeper
      * source of input.
      */
    input::pointer deeper;

    /**
      * The copy instance variable is used to remember where to send the
      * duplicate copy of the input.
      */
    output::pointer copy;

    /**
      * The default constructor.  Do not use.
      */
    input_divert();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    input_divert(const input_divert &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    input_divert &operator=(const input_divert &rhs);
};

#endif // LIB_INPUT_DIVERT_H
// vim: set ts=8 sw=4 et :
