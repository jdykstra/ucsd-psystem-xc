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

#ifndef LIB_OUTPUT_FILTER_EXPAND_H
#define LIB_OUTPUT_FILTER_EXPAND_H

#include <lib/output/filter.h>

/**
  * The output_filter_expand class is used to represent the processing
  * required to expand tab characters to the appropriate number of
  * spaces.
  */
class output_filter_expand:
    public output_filter
{
public:
    typedef boost::shared_ptr<output_filter_expand> pointer;

    /**
      * The destructor.
      */
    virtual ~output_filter_expand();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The deeper stream to write our filtered output to.
      * @param tab_width
      *     The column width of tab characters.
      */
    static pointer create(const output::pointer &deeper,
        unsigned tab_width = 8);

protected:
    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    void flush_inner(void);

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper stream to write our filtered output to.
      * @param tab_width
      *     The column width of tab characters.
      */
    output_filter_expand(const output::pointer &deeper, unsigned tab_width);

    /**
      * The tab_width instance variable is used to remember the column
      * width of tab characters.
      */
    unsigned tab_width;

    /**
      * The in_col instance variable is used to remember the column the
      * input is up to.
      */
    unsigned in_col;

    /**
      * The out_col instance variable is used to remember the column the
      * output is up to (it trails #in_col when white space is present).
      */
    unsigned out_col;

    /**
      * The default constructor.  Do not use.
      */
    output_filter_expand();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    output_filter_expand(const output_filter_expand &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    output_filter_expand &operator=(const output_filter_expand &rhs);
};

#endif // LIB_OUTPUT_FILTER_EXPAND_H
// vim: set ts=8 sw=4 et :
