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

#ifndef LIB_OUTPUT_WRAP_MAKE_H
#define LIB_OUTPUT_WRAP_MAKE_H

#include <lib/output.h>
#include <lib/rcstring/accumulator.h>

/**
  * The output_wrap_make class is used to represent the processing
  * required to wrap the text of a Makefile as it is written.
  * This includes preserving tabs, and using "\\\n" to indicate
  * continuations.
  */
class output_wrap_make:
    public output
{
public:
    typedef boost::shared_ptr<output_wrap_make> pointer;

    /**
      * The destructor.
      */
    virtual ~output_wrap_make();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The deeper output being written to, after the text has been
      *     wrapped.
      */
    static pointer create(const output::pointer &deeper);

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
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param deeper
      *     The deeper output being written to, after the text has been
      *     wrapped.
      */
    output_wrap_make(const output::pointer &deeper);

    /**
      * The buf instance variable is used to remember the current line
      * being constructed.
      */
    rcstring_accumulator buf;

    /**
      * The wrap method is used to wrap the current line, conyained
      * in #buf, to the #deeper output.  When it returns #buf will be
      * empty.
      */
    void wrap(void);

    /**
      * The deeper instance variable is used to remember the deeper
      * output being written to, after the text has been wrapped.
      */
    output::pointer deeper;

    /**
      * The default constructor.
      */
    output_wrap_make();

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    output_wrap_make(const output_wrap_make &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    output_wrap_make &operator=(const output_wrap_make &rhs);
};

#endif // LIB_OUTPUT_WRAP_MAKE_H
// vim: set ts=8 sw=4 et :
