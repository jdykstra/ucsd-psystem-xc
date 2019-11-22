//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#ifndef UCSDPSYS_CHARSET_SINK_TEXT_H
#define UCSDPSYS_CHARSET_SINK_TEXT_H

#include <ucsdpsys_charset/sink.h>

/**
  * The sink_text class is used to represent
  */
class sink_text:
    public sink
{
public:
    /**
      * The destructor.
      */
    virtual ~sink_text();

    static bool candidate(const rcstring &filename);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const rcstring &filename);

protected:
    // See base class for documentation.
    void write_whole_font_begin(void);

    // See base class for documentation.
    void write_one_glyph(const glyph::pointer &gp);

    // See base class for documentation.
    void write_whole_font_end(void);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    sink_text(FILE *fp);

    FILE *fp;

    void print_bits(const unsigned char *data, size_t data_size_in_bits);

    /**
      * The default constructor.
      * Do not use.
      */
    sink_text();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    sink_text(const sink_text &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    sink_text &operator=(const sink_text &rhs);
};

#endif // UCSDPSYS_CHARSET_SINK_TEXT_H
// vim: set ts=8 sw=4 et :
