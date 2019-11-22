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

#ifndef LIB_OUTPUT_NULL_H
#define LIB_OUTPUT_NULL_H

#include <lib/output.h>

/**
  * The output_null class is used to represent an output stream that is
  * always discarded.
  */
class output_null:
    public output
{
public:
    typedef boost::shared_ptr<output_null> pointer;

    /**
      * The destructor.
      */
    virtual ~output_null();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

protected:
    // See base class for documentation.
    rcstring filename(void);

    // See base class for documentation.
    void utime(struct utimbuf &utb);

    // See base class for documentation.
    unsigned get_width(void) const;

    // See base class for documentation.
    unsigned get_height(void) const;

    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    void flush_inner(void);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    output_null();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    output_null(const output_null &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    output_null &operator=(const output_null &rhs);
};

#endif // LIB_OUTPUT_NULL_H
// vim: set ts=8 sw=4 et :
