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

#ifndef UCSDPSYS_CHARSET_GLYPH_H
#define UCSDPSYS_CHARSET_GLYPH_H

#include <lib/config.h>
#include <boost/shared_ptr.hpp>

#include <lib/rcstring.h>

/**
  * The glyph class is used to represent a single character image from a
  * font.  It is very general.
  * The glyphs are all assumed to be bit-mapped, lsb on right, top row first.
  */
class glyph
{
public:
    typedef boost::shared_ptr<glyph> pointer;

    /**
      * The destructor.
      */
    virtual ~glyph();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param number
      *     The character number of this glyph (0..255).
      *     No support for unicode.
      * @param width
      *     The width of the glyph, in pixels.
      * @param height
      *     The height of the glyph, in pixels.
      * @param data
      *     The data for the pixel.  Points to the base of array of
      *     (((width + 7) / 8) * height) bytes.
      */
    static pointer create(unsigned char number, unsigned width, unsigned height,
        const unsigned char *data);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.  The data will be all-bits-zero.
      *
      * @param number
      *     The character number of this glyph (0..255).
      *     No support for unicode.
      * @param width
      *     The width of the glyph, in pixels.
      * @param height
      *     The height of the glyph, in pixels.
      */
    static pointer create(unsigned char number, unsigned width,
        unsigned height);

    unsigned char get_number(void) const { return number; }

    rcstring get_number_pretty(void) const;

    unsigned get_width(void) const { return width; }

    unsigned get_row_bytes(void) const { return row_bytes; }

    unsigned get_height(void) const { return height; }

    const unsigned char *
    get_row(unsigned y)
        const
    {
        return data + y * row_bytes;
    }

    void print(FILE *fp) const;

    void set_pixel(unsigned x, unsigned y, bool on_off);
    void set_pixel(unsigned x, unsigned y);
    void clear_pixel(unsigned x, unsigned y);
    bool get_pixel(unsigned x, unsigned y) const;

    rcstring get_location(void) const;
    void set_location(const rcstring &locn);

    glyph::pointer crop(unsigned xo, unsigned yo, unsigned width,
        unsigned height) const;

    glyph::pointer crop(unsigned width, unsigned height) const;

    glyph::pointer vflip(void) const;

    /**
      * The invert factory method is used to manufacture a new glyph
      * from this glyph, by inverting all the pixels.  The character
      * number of the new glyph will be the same as the old glyph.
      */
    glyph::pointer invert(void) const;

    /**
      * The invert factory method is used to manufacture a new glyph
      * from this glyph, by inverting all the pixels.
      *
      * @param c
      *     The character number of the new glyph.
      */
    glyph::pointer invert(unsigned char c) const;

    bool all_zero(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param number
      *     The character number of this glyph (0..255).
      *     No support for unicode.
      * @param width
      *     The width of the glyph, in pixels.
      * @param height
      *     The height of the glyph, in pixels.
      * @param data
      *     The data for the pixel.  Points to the base of array of
      *     (((width + 7) / 8) * height) bytes.
      */
    glyph(unsigned char number, unsigned width, unsigned height,
        const unsigned char *data);

    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      * Data initialised to all-bits-zero.
      *
      * @param number
      *     The character number of this glyph (0..255).
      *     No support for unicode.
      * @param width
      *     The width of the glyph, in pixels.
      * @param height
      *     The height of the glyph, in pixels.
      */
    glyph(unsigned char number, unsigned width, unsigned height);

    unsigned number;
    unsigned width;
    unsigned row_bytes;
    unsigned height;
    unsigned char *data;
    rcstring location;

    /**
      * The default constructor.
      * Do not use.
      */
    glyph();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    glyph(const glyph &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    glyph &operator=(const glyph &rhs);
};

#endif // UCSDPSYS_CHARSET_GLYPH_H
// vim: set ts=8 sw=4 et :
