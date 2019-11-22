//
// UCSD p-System operating system
// Copyright (C) 2006, 2010, 2012 Peter Miller
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

#include <lib/ac/stdarg.h>
#include <lib/ac/stdio.h>
#include <lib/ac/string.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>

#include <lib/endof.h>
#include <lib/rcstring.h>
#include <lib/rcstring/accumulator.h>

#include <ucsdpsys_opcodes/bitflip.h>
#include <ucsdpsys_opcodes/encode.h>


static rcstring ifn;
static FILE *ifp;
static int linum;


static bool
stdio_stream(const rcstring &fn)
{
    return (fn.empty() || fn == '-');
}


static void
parse_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[200];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    explain_output_error_and_die("%s: %d: %s", ifn.c_str(), linum, buffer);
}


static void
syntax_error(void)
{
    explain_output_error_and_die("%s: %d: syntax error", ifn.c_str(), linum);
}


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
    token_type,
    token_junk
};


static token_t tok;
static int token_value_integer;
static rcstring token_value_string;


static int
lex_getc()
{
    int c = explain_getc_or_die(ifp);
    if (c == '\n')
        ++linum;
    return c;
}


static void
lex_ungetc(int c)
{
    if (c != EOF)
    {
        if (c == '\n')
            --linum;
        ungetc(c, ifp);
    }
}


static int
escape()
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


static int
lookup(const rcstring &name)
{
    static const rcstring opc[] =
    {
        "short",
        "one",
        "opt",
        "two",
        "lopt",
        "words",
        "chrs",
        "blk",
        "cmprss",
        "cmprss2",
        "word",
    };
    for (size_t j = 0; j < SIZEOF(opc); ++j)
    {
        if (name == opc[j])
            return j;
    }
    return -1;
}


static token_t
token_next()
{
    static rcstring_accumulator ac;
    ac.clear();
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

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
            for (;;)
            {
                ac.push_back(c);
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
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    break;

                default:
                    lex_ungetc(c);
                    token_value_string = ac.mkstr();
                    token_value_integer = lookup(token_value_string);
                    tok = token_type;
                    if (token_value_integer < 0)
                    {
                        tok = token_junk;
                        parse_error
                        (
                            "opcode type %s unknown",
                            token_value_string.quote_c().c_str()
                        );
                    }
                    return tok;
                }
            }

        case '"':
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
encode(const rcstring &infile, const rcstring &outfile)
{
    size_t size = (256 - 52) * 8 + 256 * 2;
    unsigned char *data = new unsigned char [size];
    unsigned char *tp = data + 8 * (256 - 52);
    memset(data, ' ', tp - data);
    memset(tp, 0, data + size - tp);

    ifn = infile;
    ifp = stdin;
    if (stdio_stream(ifn))
        ifn = "stdin";
    else
        ifp = explain_fopen_or_die(ifn.c_str(), "r");
    linum = 1;
    int used[256];
    for (int j = 0; j < 256; ++j)
        used[j] = 0;
    for (;;)
    {
        token_next();
        if (tok == token_eof)
            break;
        if (tok != token_integer)
            syntax_error();
        int c = token_value_integer;
        if (c < 0 || c >= 256)
            parse_error("character %d out of range", c);
        if (used[c])
        {
            parse_error
            (
                "opcode %d duplicate definition (first defined on line %d)",
                c,
                used[c]
            );
        }
        else
            used[c] = linum;
        if (token_next() != token_eq)
            syntax_error();
        token_next();

        if (tok != token_type)
            syntax_error();
        two_byte_encode(&tp[c * 2], token_value_integer);
        token_next();

        if (tok == token_comma)
        {
            if (c < 52)
                parse_error("opcode %d must not have a name", c);
            token_next();
            if (tok != token_string)
                syntax_error();
            if (token_value_string.size() < 1 || token_value_string.size() > 8)
                parse_error("invalid string size");
            memcpy
            (
                data + 8 * (c - 52),
                token_value_string.c_str(),
                token_value_string.size()
            );
            token_next();
        }
        else if (c >= 52)
            parse_error("opcode %d must have a name", c);
        if (tok != token_semicolon)
            syntax_error();
    }
    if (ifp != stdin)
        fclose(ifp);

    //
    // Now write the data back out.
    //
    rcstring ofn = outfile;
    FILE *ofp = stdout;
    if (stdio_stream(ofn))
        ofn = "stdout";
    else
    {
        ofp = explain_fopen_or_die(ofn.c_str(), "wb");
    }
    fwrite(data, 1, size, ofp);
    explain_fflush_or_die(ofp);
    if (ofp != stdout)
        explain_fclose_or_die(ofp);
}


// vim: set ts=8 sw=4 et :
