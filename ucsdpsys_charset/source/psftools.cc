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

#include <lib/ac/stdlib.h>
#include <lib/ac/string.h>
#include <libexplain/fclose.h>
#include <libexplain/fopen.h>
#include <libexplain/fgets.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>

#include <lib/get_filename.h>
#include <lib/rcstring/accumulator.h>
#include <lib/sizeof.h>

#include <ucsdpsys_charset/source/psftools.h>


source_psftools::~source_psftools()
{
    explain_fclose_or_die(fp);
    fp = 0;
}


source_psftools::source_psftools(const rcstring &a_filename) :
    filename(a_filename),
    fp(stdin),
    line_number(1),
    hdr_version(0),
    hdr_flags(0),
    hdr_length(0),
    hdr_width(0),
    hdr_height(0)
{
    if (looks_like_a_stdin_synonym(filename))
        filename = filename_from_stream(stdin);
    else
        fp = explain_fopen_or_die(filename.c_str(), "r");
}


source_psftools::pointer
source_psftools::create(const rcstring &a_filename)
{
    return pointer(new source_psftools(a_filename));
}


bool
source_psftools::candidate(const rcstring &fn)
{
    if (looks_like_a_stdin_synonym(fn))
        return true;

    FILE *fp = explain_fopen_or_die(fn.c_str(), "r");
    char buffer[20];
    memset(buffer, 0, sizeof(buffer));
    explain_fgets_or_die(buffer, sizeof(buffer), fp);
    explain_fclose_or_die(fp);
    return (0 == memcmp(buffer, "%PSF", 4));
}


int
source_psftools::lex_getc(void)
{
    for (;;)
    {
        int c = explain_getc_or_die(fp);
        switch (c)
        {
        case '\\':
            c = explain_getc_or_die(fp);
            if (c == '\n')
            {
                ++line_number;
                continue;
            }
            ungetc(c, fp);
            return '\\';

        case '\n':
            ++line_number;
            break;
        }
        return c;
    }
}


void
source_psftools::lex_ungetc(int c)
{
    if (c != EOF)
        ungetc(c, fp);
    if (c == '\n')
        --line_number;
}


source_psftools::token_t
source_psftools::keyword(const rcstring &text)
{
    struct table_t
    {
        const char *name;
        token_t value;
    };
    static const table_t table[] =
    {
        { "bitmap", token_bitmap },
        { "flags", token_flags },
        { "height", token_height },
        { "length", token_length },
        { "psf2", token_psf2 },
        { "unicode", token_unicode },
        { "version", token_version },
        { "width", token_width },
    };
    rcstring lc_text = text.downcase();
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (rcstring(tp->name) == lc_text)
            return tp->value;
    }
    return token_name;
}


source_psftools::token_t
source_psftools::token_next(void)
{
    static rcstring_accumulator acc;
    for (;;)
    {
        acc.clear();
        int c = lex_getc();
        switch (c)
        {
        case EOF:
            tok = token_eof;
            return tok;

        case '\t':
            break;

        case '\n':
            tok = token_eoln;
            return tok;

        case ' ':
            break;

        case '%':
            tok = token_percent;
            return tok;

        case '#':
        case '-':
            acc.push_back((char)c);
            for (;;)
            {
                c = lex_getc();
                switch (c)
                {
                case ' ':
                case '\t':
                    continue;

                case '-':
                case '#':
                    acc.push_back((char)c);
                    continue;

                case EOF:
                    break;

                case '\n':
                default:
                    lex_ungetc(c);
                    break;
                }
                break;
            }
            token_string_value = acc.mkstr();
            tok = token_bitmap_constant;
            return tok;

        case '/':
            c = lex_getc();
            if (c == '/')
            {
                for (;;)
                {
                    c = lex_getc();
                    if (c == EOF)
                        break;
                    if (c == '\n')
                    {
                        lex_ungetc(c);
                        break;
                    }
                }
                continue;
            }
            lex_ungetc(c);
            tok = token_junk;
            return tok;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            for (;;)
            {
                acc.push_back((char)c);
                c = lex_getc();
                switch (c)
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    continue;

                case EOF:
                    break;

                default:
                    lex_ungetc(c);
                    break;
                }
                break;
            }
            token_string_value = acc.mkstr();
            token_integer_value = strtol(token_string_value.c_str(), 0, 10);
            tok = token_integer_constant;
            return tok;

        case ':':
            tok = token_colon;
            return tok;

        case ';':
            tok = token_semicolon;
            return tok;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '_':
            for (;;)
            {
                acc.push_back((char)c);
                c = lex_getc();
                switch (c)
                {
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                case 'Y': case 'Z':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                case 'y': case 'z':
                case '_':
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    continue;

                case EOF:
                    break;

                default:
                    lex_ungetc(c);
                    break;
                }
                break;
            }
            token_string_value = acc.mkstr();
            tok = keyword(token_string_value);
            return tok;

        case '[':
            // looks like "[1234]"
            for (;;)
            {
                c = lex_getc();
                switch (c)
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    acc.push_back((char)c);
                    continue;

                case ']':
                    if (acc.empty())
                        syntax_error();
                    break;

                default:
                    syntax_error();
                    //NOTREACHED
                    break;
                }
                break;
            }
            token_string_value = acc.mkstr();
            token_integer_value = strtol(token_string_value.c_str(), 0, 16);
            tok = token_unicode_constant;
            return tok;

        default:
            tok = token_junk;
            return tok;
        }
    }
}


void
source_psftools::fatal_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[1000];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    explain_output_error_and_die
    (
        "%s: %d: %s",
        filename.c_str(),
        line_number,
        buffer
    );
}


void
source_psftools::syntax_error(void)
{
    fatal_error("syntax error");
}


int
source_psftools::colon_number_eoln(void)
{
    if (token_next() != token_colon)
        syntax_error();
    if (token_next() != token_integer_constant)
        fatal_error("number expected");
    int n = token_integer_value;
    if (token_next() != token_eoln)
        syntax_error();
    token_next();
    return n;
}


void
source_psftools::read_whole_font_begin(void)
{
    hdr_width = 0;
    hdr_height = 0;
    line_number = 1;

    //
    // read the magic number
    //
    rewind(fp);
    if (token_next() != token_percent)
        syntax_error();
    if (token_next() != token_psf2)
        syntax_error();
    if (token_next() != token_eoln)
        syntax_error();
    token_next();

    //
    // read the header fields
    //
    for (;;)
    {
        switch (tok)
        {
        case token_eoln:
            // ignore blank lines
            // (and comments look a lot like blank lines)
            token_next();
            break;

        case token_percent:
            if (token_next() != token_eoln)
                syntax_error();
            token_next();
            // Fall through...

        case token_eof:
            //
            // Check that all the header fields we need have beeen defined.
            //
            if (hdr_width <= 0)
                fatal_error("the Width has not been defined");
            if (hdr_height <= 0)
                fatal_error("the Height has not been defined");
            return;

        case token_version:
            hdr_version = colon_number_eoln();
            break;

        case token_flags:
            hdr_flags = colon_number_eoln();
            break;

        case token_length:
            hdr_length = colon_number_eoln();
            break;

        case token_width:
            hdr_width = colon_number_eoln();
            if (hdr_width < 1)
                fatal_error("the Width %d is out of range", hdr_width);
            break;

        case token_height:
            hdr_height = colon_number_eoln();
            if (hdr_height < 1)
                fatal_error("the Height %d is out of range", hdr_height);
            break;

        case token_name:
            // ignore fields we don't understand
            if (token_next() != token_colon)
                syntax_error();
            for (;;)
            {
                token_next();
                if (tok == token_eof)
                    break;
                if (tok == token_eoln)
                {
                    token_next();
                    break;
                }
            }
            break;

        case token_bitmap:
        case token_bitmap_constant:
        case token_colon:
        case token_integer_constant:
        case token_junk:
        case token_psf2:
        case token_semicolon:
        case token_unicode:
        case token_unicode_constant:
        default:
            syntax_error();
            // NOTREACHED
        }
    }
}


static glyph::pointer
build_glyph(unsigned number, unsigned width, unsigned height,
    const rcstring &text)
{
    assert(width * height == text.size());
    unsigned row_bytes = (width + 7) / 8;
    size_t nbytes = row_bytes * height;
    unsigned char *data = new unsigned char [nbytes];
    memset(data, 0, nbytes);

    //
    // Turn the string into the bitmap.
    //
    unsigned char *dp = data;
    unsigned char bit = 1;
    const char *cp = text.c_str();
    for (;;)
    {
        unsigned char c = *cp++;
        if (c == '\0')
            break;
        if (c == '#')
            *dp |= bit;
        bit <<= 1;
        if (!bit)
        {
            ++dp;
            bit = 1;
        }
    }

    //
    // build the glyph
    //
    glyph::pointer gp = glyph::create(number, width, height, data);
    delete [] data;
    return gp;
}


glyph::pointer
source_psftools::read_one_glyph(void)
{
    rcstring bitmap;
    int unicode = -1;
    for (;;)
    {
        rcstring location =
            rcstring::printf("%s: %d", filename.c_str(), line_number);
        switch (tok)
        {
        case token_eoln:
            // ignore blank lines
            token_next();
            break;

        case token_bitmap:
            if (token_next() != token_colon)
                syntax_error();
            if (token_next() != token_bitmap_constant)
                fatal_error("bitmap constant expected");
            bitmap = token_string_value;
            if (hdr_width * hdr_height != bitmap.size())
                fatal_error("bitmap data wrong shape");
            if (token_next() != token_eoln)
                syntax_error();
            token_next();
            break;

        case token_unicode:
            if (token_next() != token_colon)
                syntax_error();
            if (token_next() == token_eoln)
                unicode = 0x7FFFFFFF;
            for (;;)
            {
                switch (tok)
                {
                case token_eof:
                    goto ucdone;

                case token_eoln:
                    token_next();
                    goto ucdone;

                case token_unicode_constant:
                    if (unicode == -1 || token_integer_value < unicode)
                    {
                        unicode = token_integer_value;
                    }
                    if (token_next() != token_semicolon)
                        syntax_error();
                    token_next();
                    break;

                case token_bitmap:
                case token_bitmap_constant:
                case token_colon:
                case token_flags:
                case token_height:
                case token_integer_constant:
                case token_junk:
                case token_length:
                case token_name:
                case token_percent:
                case token_psf2:
                case token_semicolon:
                case token_unicode:
                case token_version:
                case token_width:
                default:
                    syntax_error();
                    // NOTREACHED
                }
            }
            ucdone:
            break;

        case token_eof:
            if (bitmap.empty() && unicode == -1)
                return glyph::pointer();
            goto normal;

        case token_percent:
            if (token_next() != token_eoln)
                syntax_error();
            normal:
            if (bitmap.empty())
            {
                explain_output_error_and_die
                (
                    "%s: missing bitmap",
                    location.c_str()
                );
            }
            if (unicode < 0)
            {
                explain_output_error_and_die
                (
                    "%s: missing unicode",
                    location.c_str()
                );
            }
            if (unicode < 0x0100)
            {
                glyph::pointer gp =
                    build_glyph(unicode, hdr_width, hdr_height, bitmap);
                gp->set_location(location);
                return gp;
            }
            bitmap.clear();
            unicode = -1;
            break;

        case token_bitmap_constant:
        case token_colon:
        case token_flags:
        case token_height:
        case token_integer_constant:
        case token_junk:
        case token_length:
        case token_name:
        case token_psf2:
        case token_semicolon:
        case token_unicode_constant:
        case token_version:
        case token_width:
        default:
            syntax_error();
            break;
        }
    }
}


void
source_psftools::read_whole_font_end(void)
{
}


// vim: set ts=8 sw=4 et :
