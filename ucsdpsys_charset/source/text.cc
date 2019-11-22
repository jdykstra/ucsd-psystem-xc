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

#include <lib/config.h>
#include <libexplain/fclose.h>
#include <libexplain/fopen.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>

#include <lib/get_filename.h>
#include <lib/rcstring/accumulator.h>

#include <ucsdpsys_charset/source/text.h>
#include <ucsdpsys_charset/pretty_char_name.h>


source_text::~source_text()
{
    explain_fclose_or_die(fp);
    fp = 0;
}


source_text::source_text(const rcstring &a_filename) :
    filename(a_filename),
    fp(stdin),
    line_number(0),
    tok(token_junk),
    token_value_integer(0)
{
    if (looks_like_a_stdin_synonym(filename))
        filename = filename_from_stream(stdin);
    else
        fp = explain_fopen_or_die(filename.c_str(), "r");
    assert(fp);
}


source_text::pointer
source_text::create(const rcstring &filename)
{

    return pointer(new source_text(filename));
}


bool
source_text::candidate(const rcstring &filename)
{
    return (filename.ends_with(".text") || filename.ends_with(".txt"));
}


void
source_text::parse_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[200];
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
source_text::syntax_error(void)
{
    explain_output_error_and_die
    (
        "%s: %d: syntax error",
        filename.c_str(),
        line_number
    );
}


void
source_text::string_to_bits(const glyph::pointer &gp, unsigned y, const char *s)
{
    for (unsigned x = 0; ; ++x)
    {
        switch (*s++)
        {
        case '\0':
            return;

        case ' ':
        case '-':
            break;

        case '#':
        case 'x':
        case 'X':
        case 'o':
        case 'O':
        default:
            gp->set_pixel(x, y);
            break;
        }
    }
}


int
source_text::lex_getc(void)
{
    int c = explain_getc_or_die(fp);
    if (c == '\n')
        ++line_number;
    return c;
}


void
source_text::lex_ungetc(int c)
{
    if (c != EOF)
    {
        if (c == '\n')
            --line_number;
        ungetc(c, fp);
    }
}


int
source_text::escape(void)
{
    int c = lex_getc();
    switch (c)
    {
    case EOF:
    case '\n':
    case '\r':
        syntax_error();

    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
        {
            int n = 0;
            for (int ndig = 0; ndig < 3; ++ndig)
            {
                n = n * 8 + c - '0';
                c = lex_getc();
                switch (c)
                {
                case '0': case '1': case '2': case '3':
                case '4': case '5': case '6': case '7':
                    break;

                default:
                    lex_ungetc(c);
                    return n;
                }
            }
            return n;
        }

    default:
        return c;
    }
}


source_text::token_t
source_text::token_next(void)
{
    for (;;)
    {
        int c = lex_getc();
        switch (c)
        {
        case EOF:
            tok = token_eof;
            return tok;

        case ' ':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
            break;

        case '#':
            for (;;)
            {
                c = lex_getc();
                if (c == EOF || c == '\n')
                    break;
            }
            break;

        case '=':
            tok = token_eq;
            return tok;

        case ',':
            tok = token_comma;
            return tok;

        case ';':
            tok = token_semicolon;
            return tok;

        case '{':
            tok = token_lbrace;
            return tok;

        case '}':
            tok = token_rbrace;
            return tok;

        case '\'':
            c = lex_getc();
            if (c == EOF || c == '\'')
                syntax_error();
            if (c == '\\')
                c = escape();
            token_value_integer = c;
            c = lex_getc();
            if (c != '\'')
                syntax_error();
            tok = token_character;
            return tok;

        case '"':
            {
                rcstring_accumulator ac;
                for (;;)
                {
                    c = lex_getc();
                    if (c == EOF || c == '"')
                        break;
                    if (c == '\n')
                        syntax_error();
                    if (c == '\\')
                        c = escape();
                    ac.push_back((char)c);
                }
                token_value_string = ac.mkstr();
                tok = token_string;
            }
            return tok;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            token_value_integer = 0;
            for (;;)
            {
                token_value_integer = token_value_integer * 10 + c - '0';
                c = lex_getc();
                switch (c)
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    break;

                default:
                    lex_ungetc(c);
                    tok = token_integer;
                    return tok;
                }
            }

        default:
            tok = token_junk;
            return tok;
        }
    }
}


void
source_text::read_whole_font_begin(void)
{
    rewind(fp);
    line_number = 1;

    // pre-load first token
    token_next();
}


glyph::pointer
source_text::read_one_glyph(void)
{
    for (;;)
    {
        if (tok == token_eof)
            return glyph::pointer();

        if (tok != token_integer && tok != token_character)
            syntax_error();
        bool keep = true;
        int c = token_value_integer;
        if (c < 0 || c >= 0x100)
        {
            parse_error
            (
                "character %s out of range",
                pretty_char_name(c).c_str()
            );
            c = 'A';
            keep = false;
        }
        if (token_next() != token_eq)
            syntax_error();
        if (token_next() != token_lbrace)
            syntax_error();
        token_next();

        // we will build the glyph here.
        // we start at the smallest possible size.
        glyph::pointer result = glyph::create(c, 1, 1);
        result->set_location
        (
            rcstring::printf("%s: %d", filename.c_str(), line_number)
        );

        for (unsigned y = 0; ; ++y)
        {
            if (tok != token_string)
                syntax_error();
            string_to_bits(result, y, token_value_string.c_str());
            token_next();
            if (tok != token_comma)
                break;
            token_next();
            if (tok == token_rbrace)
                break;
        }
        if (tok != token_rbrace)
            syntax_error();
        if (token_next() != token_semicolon)
            syntax_error();

        // pre-load next token
        token_next();

        if (keep)
        {
            // return the glyph we just constructed
            return result;
        }
    }
}


// vim: set ts=8 sw=4 et :
