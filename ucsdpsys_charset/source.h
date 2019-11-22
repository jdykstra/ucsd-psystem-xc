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

#ifndef UCSDPSYS_CHARSET_SOURCE_H
#define UCSDPSYS_CHARSET_SOURCE_H

#include <lib/rcstring.h>

#include <ucsdpsys_charset/font.h>

/**
  * The source class is used to represent a source of font information,
  * usually a font file format, but it could be more exotic.
  */
class source
{
public:
    typedef boost::shared_ptr<source> pointer;

    /**
      * The destructor.
      */
    virtual ~source();

    /**
      * The factory class method is used to create new
      * dynamically allocated instances of this class.
      *
      * @param filename
      *     The name of the file to be read.  The desired format is guessed by
      *     inspecting the file name, which can often be innaccurate.
      * @returns
      *     managed pointer to the new source.
      */
    static pointer factory(const rcstring &filename);

    /**
      * The factory_by_name class method is used to create new
      * dynamically allocated instances of this class.
      *
      * @param format_name
      *     The name of the desired format
      * @param filename
      *     The nam eof the file to be read.
      * @returns
      *     managed pointer to the new source.
      */
    static pointer factory_by_name(const rcstring &format_name,
        const rcstring &filename);

    /**
      * The read_whole_font method is used to read the entire contents
      * of the font into memory.
      *
      * @returns
      *     managed pointer to the new font.
      */
    virtual font::pointer read_whole_font(void);

    /**
      * The is binary method is used to determine whether or not the
      * file format is "binary".  Or, at least, definitely not a text
      * file.
      */
    virtual bool is_binary(void) const;

    /**
      * The get_boot_logo method is used to read the boot logo from the
      * font file, if present.
      *
      * @param boot_logo
      *     Where to place the 512-bytes of boot logo (64x64x1).
      * @returns
      *     true if a logo was present, false of <b>no</b> logo data has
      *     been returned
      */
    virtual bool get_boot_logo(unsigned char *boot_logo);

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    source();

    /**
      * The read_one_glyph method is used to read one glyph from the
      * source.  This is used by the #read_whole_font method, called
      * repeatedly until complete font is loaded.
      *
      * There is no guarantee the glyphs will be in strictly increasing
      * order.  There is no guarantee that all 256 glyph numbers will be
      * present in the font source.
      *
      * @returns
      *     pointer to new glyph, or NULL at end-of-file
      */
    virtual glyph::pointer read_one_glyph(void) = 0;

    /**
      * The read_whole_font_begin method is called by the
      * #read_whole_font method, before it performs any parsing of the
      * source.  The default implementation does nothing.
      */
    virtual void read_whole_font_begin(void);

    /**
      * The read_whole_font_end method is called by the
      * #read_whole_font method, after it has complete parsing the
      * source.  The default implementation does nothing.
      */
    virtual void read_whole_font_end(void);

    /**
      * The looks_like_a_stdin_synonym class method is used to examine a
      * file name, to determine wjether or not it looks like a synonym
      * for the standard input.
      */
    static bool looks_like_a_stdin_synonym(const rcstring &filename);

private:
    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    source(const source &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    source &operator=(const source &rhs);
};

#endif // UCSDPSYS_CHARSET_SOURCE_H
// vim: set ts=8 sw=4 et :
