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

#ifndef LIB_OUTPUT_STRACC_H
#define LIB_OUTPUT_STRACC_H

#include <lib/output.h>
#include <lib/rcstring/accumulator.h>

/**
  * The output_stracc class is used to represent an accumulatoin of text
  * is memory.
  */
class output_stracc:
    public output
{
public:
    typedef boost::shared_ptr<output_stracc> pointer;

    /**
      * The destructor.
      */
    virtual ~output_stracc();

private:
    /**
      * The constructor.
      *
      * @param width
      *     The nominal width of the output.
      */
    output_stracc(unsigned width);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param width
      *     The nominal width of the output.
      */
    static pointer create(unsigned width);

    // See base class for documentation.
    rcstring filename();

    // See base class for documentation.
    void utime(struct utimbuf &utb);

    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    void flush_inner();

    // See base class for documentation.
    unsigned get_width(void) const;

    // See base class for documentation.
    unsigned get_height(void) const;

    rcstring mkstr() const { return content.mkstr(); }

    void clear() { content.clear(); }

private:
    rcstring_accumulator content;
    unsigned width;

    /**
      * The default constructor.  Do not use.
      */
    output_stracc();

    /**
      * The copy constructor.  Do not use.
      */
    output_stracc(const output_stracc &);

    /**
      * The assignment operator.  Do not use.
      */
    output_stracc &operator=(const output_stracc &);
};

#endif // LIB_OUTPUT_STRACC_H
// vim: set ts=8 sw=4 et :
