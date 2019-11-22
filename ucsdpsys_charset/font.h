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

#ifndef UCSDPSYS_CHARSET_FONT_H
#define UCSDPSYS_CHARSET_FONT_H

#include <ucsdpsys_charset/glyph.h>

/**
  * The font class is used to represent the collection of characters
  * that make a UCSD font.  It doesn't grok unicode, because the
  * original UCSD Pascal didn't.
  */
class font
{
public:
    typedef boost::shared_ptr<font> pointer;

    /**
      * The destructor.
      */
    virtual ~font();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param width
      *     The width of the largest glyph, in pixels.
      * @param height
      *     The height of the largest glyph, in pixels.
      */
    static pointer create(unsigned width, unsigned height);

    /**
      * The invert_top_half factory method is sued to create a new font from
      * this font, by inverting the "bottom half" 0..127 characters, and
      * forming the "top half" 128..255 from the inverted bitmaps.
      */
    pointer invert_top_half(void) const;

    /**
      * The create_glyph method is used to create a new glyph instance,
      * of the appropriate size for this font.
      * It will be entirely white, so there is no need ot clear the data.
      *
      * There is no need to call set_glyph, this is done automagically.
      * This is why this method is <i>not</i> const.
      *
      * @param c
      *     The number of the glyph within the font (0..255).
      * @returns
      *     pointer to the new glyph, never NULL.
      */
    glyph::pointer create_glyph(unsigned char c);

    /**
      * The get_glyph method is used to ask this font for the glyph at
      * the code number given.
      *
      * @param c
      *     The number of the glyph of interest.
      * @returns
      *     Pointer to the existing glyph, or NULL if not present.
      */
    glyph::pointer get_glyph(unsigned char c) const;

    /**
      * The set_glyph method is used to insert or replace a glyph in a
      * font.
      *
      * @param gp
      *     the glyph of interest
      */
    void set_glyph(const glyph::pointer &gp);

    /**
      * The clear_glyph method is used to remove a glyph from a font.
      *
      * @param c
      *     The number of the glyph to be removed.
      */
    void clear_glyph(unsigned char c);

    /**
      * The read_whole_font_begin method is called by the
      * read_whole_font method before it starts calling read_one_glyph
      * for a font.
      */
    virtual void read_whole_font_begin(void);

    /**
      * The read_whole_font_end method is called by the
      * read_whole_font method after it finishes calling read_one_glyph
      * for a font.
      */
    virtual void read_whole_font_end(void);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param width
      *     The width of the largest glyph, in pixels.
      * @param height
      *     The height of the largest glyph, in pixels.
      */
    font(unsigned width, unsigned height);

    /**
      * The width instance variable is used to remember the width of the
      * largest glyph, in pixels.
      */
    unsigned width;

    /**
      * The height instance variable is used to remember the height of
      * the largest glyph, in pixels.
      */
    unsigned height;

    /**
      * The default constructor.
      * Do not use.
      */
    font();

    /**
      * The glyphs instance variable is used to remember the set of
      * glyphs in this font.
      */
    glyph::pointer glyphs[256];

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    font(const font &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    font &operator=(const font &rhs);
};

#endif // UCSDPSYS_CHARSET_FONT_H
// vim: set ts=8 sw=4 et :
