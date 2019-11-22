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

#ifndef LIB_OUTPUT_STRING_LIST_H
#define LIB_OUTPUT_STRING_LIST_H

#include <lib/output.h>
#include <lib/rcstring/accumulator.h>
#include <lib/rcstring/list.h>

/**
  * The output_string_list class is used to represent an accumulatoin of
  * text in memory.  Each line is remembered as a separate string, but
  * with the newline removed.
  */
class output_string_list:
    public output
{
public:
    typedef boost::shared_ptr<output_string_list> pointer;

    /**
      * The destructor.
      */
    virtual ~output_string_list();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param width
      *     The nominal width of the output.
      */
    static pointer create(unsigned width);

    // See base class for documentation.
    rcstring filename(void);

    // See base class for documentation.
    void utime(struct utimbuf &utb);

    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    void flush_inner(void);

    // See base class for documentation.
    unsigned get_width(void) const;

    // See base class for documentation.
    unsigned get_height(void) const;

    /**
      * The get method is used to get the content of the output,
      * one line per string (with the newline removed).
      *
      * @note
      *     You must remember to call flush() before this method,
      *     otherwise some of the content may be missing.
      */
    const rcstring_list &get(void) const { return content; }

    /**
      * The clear method is used to discard any content of this output,
      * in preparation for being recycled.
      */
    void clear(void);

private:
    /**
      * The constructor.
      *
      * @param width
      *     The nominal width of the output.
      */
    output_string_list(unsigned width);

    /**
      * The content instance variable is used to remember each completed
      * line.
      */
    rcstring_list content;

    /**
      * The current_line instance variable is used to remember the
      * current in-progress line.
      */
    rcstring_accumulator current_line;

    /**
      * The width instance variable is used to remember the nominal
      * character width of this output.
      */
    unsigned width;

    /**
      * The default constructor.  Do not use.
      */
    output_string_list(void);

    /**
      * The copy constructor.  Do not use.
      */
    output_string_list(const output_string_list &);

    /**
      * The assignment operator.  Do not use.
      */
    output_string_list &operator=(const output_string_list &);
};

#endif // LIB_OUTPUT_STRING_LIST_H
// vim: set ts=8 sw=4 et :
