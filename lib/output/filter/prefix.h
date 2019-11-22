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

#ifndef LIB_OUTPUT_PREFIX_H
#define LIB_OUTPUT_PREFIX_H

#include <lib/output/filter.h>
#include <lib/rcstring.h>

/**
  * The output_filter_prefix class is used to represent an output filter which
  * prefixes each output line with a constant string.
  */
class output_filter_prefix:
    public output_filter
{
public:
    typedef boost::shared_ptr<output_filter_prefix> pointer;

    /**
      * The destructor.
      */
    virtual ~output_filter_prefix();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The deeper stream to write our filtered output to.
      * @param pfx
      *     The string to prefix each output line.
      */
    static pointer create(const output::pointer &deeper, const rcstring &pfx);

protected:
    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    unsigned get_width(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instaed.
      *
      * @param deeper
      *     The deeper stream to write our filtered output to.
      * @param pfx
      *     The string to prefix each output line.
      */
    output_filter_prefix(const output::pointer &deeper, const rcstring &pfx);

    /**
      * The pfx instance variable is used to remember the string to
      * prefix each output line.
      */
    rcstring pfx;

    /**
      * The pfx0 instance variable is used to remember the string to
      * prefix empty lines.
      */
    rcstring pfx0;

    /**
      * The column instance variable is used to remember the printing
      * column position.  This is useful when tracking whether or not we
      * are at the beginning of a line.
      */
    int column;

    /**
      * The default constructor.
      */
    output_filter_prefix();

    /**
      * The copy constructor.
      */
    output_filter_prefix(const output_filter_prefix &);

    /**
      * The assignment operator.
      */
    output_filter_prefix &operator=(const output_filter_prefix &);
};

#endif // LIB_OUTPUT_PREFIX_H
// vim: set ts=8 sw=4 et :
