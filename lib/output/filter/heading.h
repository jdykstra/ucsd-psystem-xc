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

#ifndef LIB_OUTPUT_FILTER_HEADING_H
#define LIB_OUTPUT_FILTER_HEADING_H

#include <lib/output/filter.h>
#include <lib/rcstring.h>

/**
  * The output_filter_heading class is used to represent a filter to add
  * headings and page numbers to output.
  */
class output_filter_heading:
    public output_filter
{
public:
    typedef boost::shared_ptr<output_filter_heading> pointer;

    /**
      * The destructor.
      */
    virtual ~output_filter_heading();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The deeper stream to write our filtered output to.
      */
    static pointer create(const output::pointer &deeper);

    /**
      * The set_heading1 method is used to set the first line of the header.
      *
      * @param text
      *     The text of the first line.
      */
    void set_heading1(const rcstring &text);

    /**
      * The set_heading2 method is used to set the second line of the header.
      *
      * @param text
      *     The text of the second line.
      */
    void set_heading2(const rcstring &text);

protected:
    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    unsigned get_height(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param deeper
      *     The deeper stream to write our filtered output to.
      */
    output_filter_heading(const output::pointer &deeper);

    unsigned high_water;
    unsigned line;
    unsigned column;
    bool small;
    rcstring heading1;
    rcstring heading2;
    unsigned page;

    void draw_heading(void);

    void draw_footing(void);

    /**
      * The default constructor.  Do not use.
      */
    output_filter_heading();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    output_filter_heading(const output_filter_heading &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    output_filter_heading &operator=(const output_filter_heading &rhs);
};

#endif // LIB_OUTPUT_FILTER_HEADING_H
// vim: set ts=8 sw=4 et :
