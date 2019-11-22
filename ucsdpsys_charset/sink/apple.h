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

#ifndef UCSDPSYS_CHARSET_SINK_APPLE_H
#define UCSDPSYS_CHARSET_SINK_APPLE_H

#include <lib/ac/stdio.h>

#include <ucsdpsys_charset/sink.h>

/**
  * The sink_apple class is used to represent a font file in the format
  * used by the Apple Pascal system, with its 7x8 glyphs.
  */
class sink_apple:
    public sink
{
public:
    //typedef boost::shared_ptr<sink_apple> pointer;

    /**
      * The destructor.
      */
    virtual ~sink_apple();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const rcstring &filename);

    static bool candidate(const rcstring &filename);

protected:
    // See base class for documentation.
    void write_one_glyph(const glyph::pointer &gp);

    // See base class for documentation.
    void write_whole_font_begin(void);

    // See base class for documentation.
    void write_whole_font_end(void);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    sink_apple(FILE *fp);

    FILE *fp;

    unsigned char data[128 * 8];

    unsigned next_glyph_number;

    /**
      * The default constructor.
      * Do not use.
      */
    sink_apple();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    sink_apple(const sink_apple &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    sink_apple &operator=(const sink_apple &rhs);
};

#endif // UCSDPSYS_CHARSET_SINK_APPLE_H
// vim: set ts=8 sw=4 et :
