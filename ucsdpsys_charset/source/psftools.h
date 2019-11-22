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

#ifndef UCSDPSYS_CHARSET_SOURCE_PSFTOOLS_H
#define UCSDPSYS_CHARSET_SOURCE_PSFTOOLS_H

#include <lib/ac/stdio.h>

#include <lib/format_printf.h>

#include <ucsdpsys_charset/source.h>

/**
  * The source_psftools class is used to represent the processing
  * required to read a font defintion in PSF Tools format.
  */
class source_psftools:
    public source
{
public:
    /**
      * The destructor.
      */
    virtual ~source_psftools();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     the name of the file to be read
      */
    static pointer create(const rcstring &filename);

    /**
      * The candidate class method is used to examine the filename, and
      * possibly file contents, to determine wjether or not this looks
      * like a psftools text file.
      *
      * @param filename
      *     The name of the file to examine.
      */
    static bool candidate(const rcstring &filename);

protected:
    // See base class for documentation.
    void read_whole_font_begin(void);

    // See base class for documentation.
    glyph::pointer read_one_glyph(void);

    // See base class for documentation.
    void read_whole_font_end(void);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param filename
      *     the name of the file to be read
      */
    source_psftools(const rcstring &filename);

    rcstring filename;

    FILE *fp;

    int line_number;

    unsigned hdr_version;
    unsigned hdr_flags;
    unsigned hdr_length;
    unsigned hdr_width;
    unsigned hdr_height;

    enum token_t
    {
        token_eof,
        token_bitmap,
        token_bitmap_constant,
        token_colon,
        token_eoln,
        token_flags,
        token_height,
        token_integer_constant,
        token_junk,
        token_length,
        token_name,
        token_percent,
        token_psf2,
        token_semicolon,
        token_unicode,
        token_unicode_constant,
        token_version,
        token_width,
    };

    token_t tok;

    rcstring token_string_value;

    long token_integer_value;

    token_t token_next(void);

    int lex_getc(void);

    void lex_ungetc(int c);

    static token_t keyword(const rcstring &text);

    void syntax_error(void);

    void fatal_error(const char *fmt, ...)                  FORMAT_PRINTF(2, 3);

    int colon_number_eoln(void);

    /**
      * The default constructor.
      * Do not use.
      */
    source_psftools();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    source_psftools(const source_psftools &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    source_psftools &operator=(const source_psftools &rhs);
};

#endif // UCSDPSYS_CHARSET_SOURCE_PSFTOOLS_H


// vim: set ts=8 sw=4 et :
