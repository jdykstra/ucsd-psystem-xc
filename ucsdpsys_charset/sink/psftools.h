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

#ifndef UCSDPSYS_CHARSET_SINK_PSFTOOLS_H
#define UCSDPSYS_CHARSET_SINK_PSFTOOLS_H

#include <ucsdpsys_charset/glyph.h>
#include <ucsdpsys_charset/sink.h>

/**
  * The sink_psftools class is used to represent the processing required
  * to write a PSF Tools formatted textual font definition file.
  */
class sink_psftools:
    public sink
{
public:
    /**
      * The destructor.
      */
    virtual ~sink_psftools();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The name of the file to be written.
      */
    static pointer create(const rcstring &filename);

    /**
      * The candidate class method is used to examine file information
      * to determine if the file could be a PSF Tools font file.
      *
      * @param filename
      *     The name of the file to be decided.
      */
    static bool candidate(const rcstring &filename);

protected:
    // See base class for documentation.
    void write_one_glyph(const glyph::pointer &gp);

    // See base class for documentation.
    void write_whole_font_end(void);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param filename
      *     The name of the file to be written.
      */
    sink_psftools(const rcstring &filename);

    rcstring filename;

    glyph::pointer glyphs[256];

    /**
      * The default constructor.
      * Do not use.
      */
    sink_psftools();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    sink_psftools(const sink_psftools &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    sink_psftools &operator=(const sink_psftools &rhs);
};

#endif // UCSDPSYS_CHARSET_SINK_PSFTOOLS_H


// vim: set ts=8 sw=4 et :
