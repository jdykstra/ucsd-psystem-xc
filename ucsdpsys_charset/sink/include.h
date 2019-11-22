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

#ifndef UCSDPSYS_CHARSET_SINK_INCLUDE_H
#define UCSDPSYS_CHARSET_SINK_INCLUDE_H

#include <ucsdpsys_charset/sink.h>

/**
  * The sink_include class is used to represent an output that writes
  * a C include file, containg the font data, if you want static font
  * data in a C array.  Useful for obtaining a default font for use in an
  * emulator.
  */
class sink_include:
    public sink
{
public:
    /**
      * The destructor.
      */
    virtual ~sink_include();

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
      */
    sink_include(FILE *fp);

    FILE *fp;

    unsigned char used[256];

    /**
      * The default constructor.
      * Do not use.
      */
    sink_include();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    sink_include(const sink_include &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    sink_include &operator=(const sink_include &rhs);
};

#endif // UCSDPSYS_CHARSET_SINK_INCLUDE_H
// vim: set ts=8 sw=4 et :
