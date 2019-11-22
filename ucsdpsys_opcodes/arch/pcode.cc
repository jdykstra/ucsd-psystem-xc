//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/fread.h>
#include <libexplain/fwrite.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>

#include <lib/rcstring/accumulator.h>
#include <lib/sizeof.h>

#include <ucsdpsys_opcodes/arch/pcode.h>


arch_pcode::~arch_pcode()
{
}


arch_pcode::arch_pcode(mtype_t a_mtype) :
    arch(a_mtype)
{
    for (unsigned j = 0; j < SIZEOF(optype); ++j)
        optype[j] = 0;
}


arch::pointer
arch_pcode::create_le(void)
{
    return pointer(new arch_pcode(mtype_pcode_le));
}


arch::pointer
arch_pcode::create_be(void)
{
    return pointer(new arch_pcode(mtype_pcode_be));
}


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
    rcstring buffer = rcstring::vprintf(fmt, ap);
    va_end(ap);

    explain_output_error_and_die
    (
        "%s: %d: %s",
        ifn.c_str(),
        linum,
        buffer.c_str()
    );
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
lex_getc(void)
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
escape(void)
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


static const rcstring optype_name[] =
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


static int
lookup(const rcstring &name)
{
    for (size_t j = 0; j < SIZEOF(optype_name); ++j)
    {
        if (name == optype_name[j])
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
arch_pcode::read_text(const rcstring &infile)
{
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
        optype[c] = token_value_integer;
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
            opname[c] = token_value_string;
            token_next();
        }
        else if (c >= 52)
            parse_error("opcode %d must have a name", c);
        if (tok != token_semicolon)
            syntax_error();
    }
    if (ifp != stdin)
        explain_fclose_or_die(ifp);
}


void
arch_pcode::write_binary(const rcstring &outfile)
    const
{
    FILE *ofp =
        (
            stdio_stream(outfile)
        ?
            stdout
        :
            explain_fopen_or_die(outfile.c_str(), "wb")
        );
    for (unsigned j = 52; j < 256; ++j)
    {
        rcstring buf = opname[j] + "        ";
        explain_fwrite_or_die(buf.c_str(), 1, 8, ofp);
    }
    for (unsigned j = 0; j < 256; ++j)
    {
        unsigned char data[2];
        put_word(data, optype[j]);
        explain_fwrite_or_die(data, 1, sizeof(data), ofp);
    }
    explain_fflush_or_die(ofp);
    if (ofp != stdout)
        explain_fclose_or_die(ofp);
}


void
arch_pcode::write_text(const rcstring &ofn)
    const
{
    FILE *ofp =
        (
            stdio_stream(ofn)
        ?
            stdout
        :
            explain_fopen_or_die(ofn.c_str(), "w")
        );
    for (size_t k = 0; k < 256; ++k)
    {
        fprintf(ofp, "%3u = ", (unsigned)k);

        rcstring otype;
        {
            int ot = optype[k];
            if (ot >= 0 && (size_t)ot < SIZEOF(optype_name))
                otype = optype_name[ot];
            else
                otype = rcstring::printf("%d", ot);
        }
        fprintf(ofp, "%s", otype.c_str());

        if (k >= 52)
        {
            fprintf(ofp, "%-*s", int(9 - otype.size()), ",");
            fprintf(ofp, "%s", opname[k].quote_c().c_str());
        }
        fprintf(ofp, ";\n");
    }
    explain_fflush_or_die(ofp);
    if (ofp != stdout)
        explain_fclose_or_die(ofp);
}


void
arch_pcode::random_test_data(void)
{
    for (unsigned j = 0; j < 256; ++j)
        optype[j] = random() % SIZEOF(optype_name);
    for (unsigned j = 56; j < 256; ++j)
        opname[j] = rname();
}


void
arch_pcode::read_binary(const rcstring &filename)
{
    FILE *fp = stdin;
    if (!stdio_stream(filename))
        fp = explain_fopen_or_die(filename.c_str(), "r");
    for (unsigned j = 52; j < 256; ++j)
    {
        char data[8];
        if (explain_fread_or_die(data, 1, sizeof(data), fp) != sizeof(data))
            explain_output_error_and_die("%s: short file", filename.c_str());
        const char *ep = data + sizeof(data);
        while (ep > data && (ep[-1] == ' ' || ep[-1] == '\0'))
            --ep;
        opname[j] = rcstring(data, ep - data);
    }
    for (unsigned j = 0; j < 256; ++j)
    {
        unsigned char data[2];
        if (explain_fread_or_die(data, 1, sizeof(data), fp) != sizeof(data))
            explain_output_error_and_die("%s: short file", filename.c_str());
        optype[j] = get_word(data);
    }
    if (fp != stdin)
        explain_fclose_or_die(fp);
}


// vim: set ts=8 sw=4 et :
