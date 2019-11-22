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

#ifndef LIB_OUTPUT_FILTER_WRAP_H
#define LIB_OUTPUT_FILTER_WRAP_H

#include <lib/output/filter.h>
#include <lib/rcstring/accumulator.h>

/**
  * The output_filter_wrap class is used to represent the processing required
  * to wrap text as it is written.
  */
class output_filter_wrap:
    public output_filter
{
public:
    typedef boost::shared_ptr<output_filter_wrap> pointer;

    /**
      * The destructor.
      */
    virtual ~output_filter_wrap();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The deeper output being written to, after the text has been
      *     wrapped.
      * @param hard_break
      *     whether or not to break words that are linger than the line width.
      */
    static pointer create(const output::pointer &deeper,
        bool hard_break = false);

protected:
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
      * @param hard_break
      *     whether or not to break words that are linger than the line width.
      */
    output_filter_wrap(const output::pointer &deeper, bool hard_break);

    /**
      * The hard_break instance variable is used to remember
      * whether or not to break words that are linger than the line width.
      */
    bool hard_break;

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
      * The default constructor.  Do not use.
      */
    output_filter_wrap();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    output_filter_wrap(const output_filter_wrap &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    output_filter_wrap &operator=(const output_filter_wrap &rhs);
};

#endif // LIB_OUTPUT_FILTER_WRAP_H
// vim: set ts=8 sw=4 et :
