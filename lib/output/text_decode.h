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

#ifndef LIB_OUTPUT_TEXT_DECODE_H
#define LIB_OUTPUT_TEXT_DECODE_H

#include <lib/output.h>

/**
  * The output_text_decode class is used to represent a filter for
  * translating UCSD p-System text files into Unix text files.
  */
class output_text_decode:
    public output
{
public:
    typedef boost::shared_ptr<output_text_decode> pointer;

    /**
      * The destructor.
      */
    virtual ~output_text_decode();

private:
    /**
      * The constructor.
      *
      * It is private on purpose, use the #create class method instead.
      *
      * @param deeper
      *     the deeper output destination to which we write the filtered
      *     text.
      * @param use_tabs
      *     true if 8-character tabs should be used, or false if spaces
      *     should be used
      */
    output_text_decode(const output::pointer &deeper, bool use_tabs);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     the deeper output destination to which we write the filtered
      *     text.
      * @param use_tabs
      *     true if 8-character tabs should be used, or false if spaces
      *     should be used
      */
    static pointer create(const output::pointer &deeper, bool use_tabs = true);

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
      * The deeper instance variable is used to remember the deeper
      * output destination to which we write the filtered text.
      */
    output::pointer deeper;

    /**
      * The column instance variable is used to remember the character
      * column number of the cursor on the current line.
      */
    int column;

    /**
      * The non_white instance variable is used to remember whether ot
      * no a non-white characters has yet been seen on the current line.
      */
    bool non_white;

    int start_of_file;

    bool dle_seen;

    /**
      * The use_tabs instance variable is used to remember whether
      * 8-character tabs should be used (true) or spaces (false).
      */
    bool use_tabs;

    /**
      * The default constructor.
      */
    output_text_decode();

    /**
      * The copy constructor.  Do not use.
      */
    output_text_decode(const output_text_decode &);

    /**
      * The assignment operator.  Do not use.
      */
    output_text_decode &operator=(const output_text_decode &);
};

#endif // LIB_OUTPUT_TEXT_DECODE_H
// vim: set ts=8 sw=4 et :
