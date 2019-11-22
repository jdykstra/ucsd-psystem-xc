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

#ifndef UCSDPSYS_CHARSET_SINK_ADD_BOOT_LOGO_H
#define UCSDPSYS_CHARSET_SINK_ADD_BOOT_LOGO_H

#include <ucsdpsys_charset/sink.h>

/**
  * The sink_add_boot_logo class is used to represent the processing
  * required to supplement a sink that has no boot logo support, with
  * a PNG file.
  */
class sink_add_boot_logo:
    public sink
{
public:
    /**
      * The destructor.
      */
    virtual ~sink_add_boot_logo();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The underlying file from which to obtain the font data
      * @param png_file_name
      *     The name of the supplemental PNG file (demand loaded).
      * @returns
      *     managed pointer to new instance
      */
    static pointer create(const sink::pointer &deeper,
        const rcstring &png_file_name);

protected:
    // See base class for documentation.
    bool is_binary(void) const;

    // See base class for documentation.
    void read_whole_font_begin(void);

    // See base class for documentation.
    void write_one_glyph(const glyph::pointer &gp);

    // See base class for documentation.
    void read_whole_font_end(void);

    // See base class for documentation.
    bool put_boot_logo(const unsigned char *data);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param deeper
      *     The underlying file from which to obtain the font data
      * @param png_file_name
      *     The name of the supplemental PNG file (demand loaded).
      */
    sink_add_boot_logo(const sink::pointer &deeper,
        const rcstring &png_file_name);

    /**
      * The deeper instance variable is used to remember the underlying
      * file from which to obtain the font data
      */
    sink::pointer deeper;

    /**
      * The png_file_name instance variable is used to remember the name
      * of the supplemental PNG file (demand loaded).
      */
    rcstring png_file_name;

    /**
      * The default constructor.
      * Do not use.
      */
    sink_add_boot_logo();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    sink_add_boot_logo(const sink_add_boot_logo &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    sink_add_boot_logo &operator=(const sink_add_boot_logo &rhs);
};

#endif // UCSDPSYS_CHARSET_SINK_ADD_BOOT_LOGO_H
// vim: set ts=8 sw=4 et :
