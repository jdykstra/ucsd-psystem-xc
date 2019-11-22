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

#ifndef OUTPUT_TEXT_ENCODE_H
#define OUTPUT_TEXT_ENCODE_H

#include <lib/ac/stdio.h>

#include <lib/output.h>
#include <lib/rcstring/accumulator.h>

/**
  * The output_text_encode class is used to represent a filter which
  * translates Unix text files into UCSD p-System text files.
  */
class output_text_encode:
    public output
{
public:
    typedef boost::shared_ptr<output_text_encode> pointer;

    /**
      * The destructor.
      */
    virtual ~output_text_encode();

private:
    /**
      * The constructor.
      *
      * It is private on purpose, use the #create class method instead.
      *
      * @param deeper
      *     The destination for this filter to write the encoded data to.
      * @param use_dle
      *     wherther or not to use DLE space encoding
      * @param nul_guarantee
      *     whether or not to ensure there is always at least one NUL at
      *     the end of every 1KB text block.
      */
    output_text_encode(const output::pointer &deeper, bool use_dle,
        bool nul_guarantee);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The destination for this filter to write the encoded data to.
      * @param use_dle
      *     wherther or not to use DLE space encoding
      * @param nul_guarantee
      *     whether or not to ensure there is always at least one NUL at
      *     the end of every 1KB text block.
      */
    static pointer create(const output::pointer &deeper, bool use_dle = true,
        bool nul_guarantee = true);

    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    rcstring filename();

    // See base class for documentation.
    void flush_inner();

    // See base class for documentation.
    void utime(struct utimbuf &utb);

    // See base class for documentation.
    unsigned get_width(void) const;

private:
    /**
      * The deeper instance variable is used to remember the destination
      * for this filter to write the encoded data to.
      */
    output::pointer deeper;

    int line_number;

    unsigned address;

    int column;

    bool non_white;

    /**
      * The use_dle instance variable is used to remember whether or not
      * use use DLE space encoding.
      */
    bool use_dle;

    /**
      * The nul_guarantee instance variable is used to remember whether
      * or not to ensure there is always at least one NUL at the end of
      * every 1KB text block.
      */
    bool nul_guarantee;

    /**
      * The line_accumulator instance variable is used to remember the
      * current line under construction.
      */
    rcstring_accumulator line_accumulator;

    void line_character(unsigned char);

    void write_one_line();

    /**
      * The default constructor.  Do not use.
      */
    output_text_encode();

    /**
      * The copy constructor.  Do not use.
      */
    output_text_encode(const output_text_encode &);

    /**
      * The assignment operator.  Do not use.
      */
    output_text_encode &operator=(const output_text_encode &);
};

#endif // OUTPUT_TEXT_ENCODE_H
// vim: set ts=8 sw=4 et :
