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

#ifndef UCSDPSYS_CHARSET_SOURCE_APPLE_H
#define UCSDPSYS_CHARSET_SOURCE_APPLE_H

#include <ucsdpsys_charset/source.h>

/**
  * The source_apple class is used to represent reading a font in the
  * Apple Pascal 7x8 system.charset format.
  */
class source_apple:
    public source
{
public:
    /**
      * The destructor.
      */
    virtual ~source_apple();

    static bool candidate(const rcstring &filename);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const rcstring &filename);

protected:
    // See base class for documentation.
    void read_whole_font_begin(void);

    // See base class for documentation.
    glyph::pointer read_one_glyph(void);

    // See base class for documentation.
    bool is_binary(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    source_apple(const rcstring &filename);

    rcstring filename;

    FILE *fp;

    /**
      * The default constructor.
      * Do not use.
      */
    source_apple();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    source_apple(const source_apple &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    source_apple &operator=(const source_apple &rhs);
};

#endif // UCSDPSYS_CHARSET_SOURCE_APPLE_H
// vim: set ts=8 sw=4 et :
