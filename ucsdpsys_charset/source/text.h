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

#ifndef UCSDPSYS_CHARSET_SOURCE_TEXT_H
#define UCSDPSYS_CHARSET_SOURCE_TEXT_H

#include <lib/ac/stdio.h>

#include <lib/format_printf.h>
#include <lib/rcstring.h>

#include <ucsdpsys_charset/source.h>

/**
  * The source_text class is used to represent a font defintion file in
  * textual format.  It can cope with variable heigth and width glyphs.
  */
class source_text:
    public source
{
public:
    /**
      * The destructor.
      */
    virtual ~source_text();

    static bool candidate(const rcstring &filename);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const rcstring &filename);

protected:
    // See base class for documentation.
    glyph::pointer read_one_glyph(void);

    // See base class for documentation.
    void read_whole_font_begin(void);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    source_text(const rcstring &filename);

    rcstring filename;

    FILE *fp;

    int line_number;

    enum token_t
    {
        token_eof,
        token_integer,
        token_character,
        token_comma,
        token_string,
        token_eq,
        token_semicolon,
        token_lbrace,
        token_rbrace,
        token_junk
    };

    token_t tok;

    int token_value_integer;

    rcstring token_value_string;

    void parse_error(const char *fmt, ...)                  FORMAT_PRINTF(2, 3);

    void syntax_error(void);

    void string_to_bits(const glyph::pointer &gp, unsigned y, const char *s);

    int lex_getc(void);

    void lex_ungetc(int c);

    int escape(void);

    token_t token_next(void);

    /**
      * The default constructor.
      * Do not use.
      */
    source_text();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    source_text(const source_text &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    source_text &operator=(const source_text &rhs);
};

#endif // UCSDPSYS_CHARSET_SOURCE_TEXT_H
// vim: set ts=8 sw=4 et :
