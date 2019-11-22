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

#ifndef UCSDPSYS_CHARSET_SOURCE_ADD_BOOT_LOGO_H
#define UCSDPSYS_CHARSET_SOURCE_ADD_BOOT_LOGO_H

#include <ucsdpsys_charset/source.h>

/**
  * The source_add_boot_logo class is used to represent the processing
  * required to supplement a source that has no boot logo support, with
  * a PNG file.
  */
class source_add_boot_logo:
    public source
{
public:
    /**
      * The destructor.
      */
    virtual ~source_add_boot_logo();

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
    static pointer create(const source::pointer &deeper,
        const rcstring &png_file_name);

protected:
    // See base class for documentation.
    bool is_binary(void) const;

    // See base class for documentation.
    font::pointer read_whole_font(void);

    // See base class for documentation.
    bool get_boot_logo(unsigned char *boot_logo);

    // See base class for documentation.
    glyph::pointer read_one_glyph(void);

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
    source_add_boot_logo(const source::pointer &deeper,
        const rcstring &png_file_name);

    /**
      * The deeper instance variable is used to remember the underlying
      * file from which to obtain the font data
      */
    source::pointer deeper;

    /**
      * The png_file_name instance variable is used to remember the name
      * of the supplemental PNG file (demand loaded).
      */
    rcstring png_file_name;

    /**
      * The default constructor.
      * Do not use.
      */
    source_add_boot_logo();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    source_add_boot_logo(const source_add_boot_logo &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    source_add_boot_logo &operator=(const source_add_boot_logo &rhs);
};

#endif // UCSDPSYS_CHARSET_SOURCE_ADD_BOOT_LOGO_H
// vim: set ts=8 sw=4 et :
