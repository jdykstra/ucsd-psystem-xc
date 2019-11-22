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

#include <lib/ac/ctype.h>
#include <lib/ac/string.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/fread.h>
#include <libexplain/fwrite.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>

#include <lib/rcstring/accumulator.h>

#include <ucsdpsys_opcodes/arch/adaptable.h>


arch_adaptable::~arch_adaptable()
{
}


arch_adaptable::arch_adaptable(mtype_t a_mtype) :
    arch(a_mtype)
{
}


static bool
stdio_stream(const rcstring &fn)
{
    return (fn.empty() || fn == '-');
}


void
arch_adaptable::read_binary(const rcstring &filename)
{
    FILE *fp =
        (
            stdio_stream(filename)
        ?
            stdin
        :
            explain_fopen_or_die(filename.c_str(), "rb")
        );
    bool first = true;
    for (;;)
    {
        unsigned char data[12];
        size_t n = explain_fread_or_die(data, 1, sizeof(data), fp);
        if (n == 0)
            break;
        if (n != sizeof(data))
        {
            explain_output_error_and_die
            (
                "%s: size is not an exact multiple of %d",
                filename.c_str(),
                (int)sizeof(data)
            );
        }
        const unsigned char *ep = data + 8;
        while (ep > data && (ep[-1] == ' ' || ep[-1] == '\0'))
            --ep;
        rcstring opname = rcstring(data, ep - data);
        int opvalue = get_word(data + 8);
        atribute_t opatrib = (atribute_t)get_word(data + 10);

        // The first record is magical:
        // it tells us the byte sex of the file.
        if (first && opname.empty())
        {
            if (opvalue == 256)
                flip_byte_sex();
            first = false;
            continue;
        }

        opcodes.push_back(oprec(opname, opvalue, opatrib));
    }
    if (fp != stdin)
        explain_fclose_or_die(fp);
}


void
arch_adaptable::write_binary(const rcstring &filename)
    const
{
    FILE *fp =
        (
            stdio_stream(filename)
        ?
            stdout
        :
            explain_fopen_or_die(filename.c_str(), "wb")
        );

    // The first record is magic:
    // it tells the reader the byte sex of the file.
    {
        unsigned char data[12];
        memset(data, 0, sizeof(data));
        put_word(data + 8, 1);
        explain_fwrite_or_die(data, 1, sizeof(data), fp);
    }

    for
    (
        opcodes_t::const_iterator it = opcodes.begin();
        it != opcodes.end();
        ++it
    )
    {
        unsigned char data[12];
        const oprec &x = *it;
        rcstring temp = x.opname;
        if (temp.size() < 8)
            temp += rcstring::spaces(8 - temp.size());
        memcpy(data, temp.c_str(), 8);
        put_word(data + 8, x.opvalue);
        put_word(data + 10, x.opatrib);
        explain_fwrite_or_die(data, 1, sizeof(data), fp);
    }
    explain_fflush_or_die(fp);
    if (fp != stdout)
        explain_fclose_or_die(fp);
}


void
arch_adaptable::write_text(const rcstring &filename)
    const
{
    FILE *fp =
        (
            stdio_stream(filename)
        ?
            stdout
        :
            explain_fopen_or_die(filename.c_str(), "w")
        );

    // figure out the name column width
    unsigned width = 4;
    unsigned width3 = 1;
    for
    (
        opcodes_t::const_iterator it = opcodes.begin();
        it != opcodes.end();
        ++it
    )
    {
        const oprec &x = *it;
        rcstring pretty = x.opname.quote_c() + ",";
        if (width < pretty.size())
            width = pretty.size();
        rcstring c3 = atribute_name(x.opatrib).c_str();
        if (width3 < c3.size())
            width3 = c3.size();
    }

    for
    (
        opcodes_t::const_iterator it = opcodes.begin();
        it != opcodes.end();
        ++it
    )
    {
        const oprec &x = *it;
        rcstring pretty = x.opname.quote_c() + ",";
        fprintf
        (
            fp,
            "{ %-*s 0x%04X, %-*s },\n",
            width,
            pretty.c_str(),
            x.opvalue,
            width3,
            atribute_name(x.opatrib).c_str()
        );
    }
    explain_fflush_or_die(fp);
    if (fp != stdout)
        explain_fclose_or_die(fp);
}


rcstring
arch_adaptable::atribute_name(atribute_t x)
    const
{
    // this may need to be virtual, one day
    switch (x)
    {
    case defabs:
        return "defabs";

    case procs:
        return "procs";

    case ops1:
        return "ops1";

    case ops2:
        return "ops2";

    case ops3:
        return "ops3";

    case ops4:
        return "ops4";

    case ops5:
        return "ops5";

    case ops6:
        return "ops6";

    case ops7:
        return "ops7";

    case ops8:
        return "ops8";

    case ops9:
        return "ops9";

    case ops10:
        return "ops10";

    case ops11:
        return "ops11";

    case ops12:
        return "ops12";

    case ops13:
        return "ops13";

    case ops14:
        return "ops14";

    case ops15:
        return "ops15";

    case ops16:
        return "ops16";

    case ops17:
        return "ops17";

    case ops18:
        return "ops18";

    case ops19:
        return "ops19";

    case ops20:
        return "ops20";

    case defrp:
        return "defrp";

    case defreg:
        return "defreg";

    case defcc:
        return "defcc";

    case defir:
        return "defir";

    case defunknown:
        return "defunknown";

    case interps:
        return "interps";

    case publics:
        return "publics";

    case consts:
        return "consts";

    case privates:
        return "privates";

    case refs:
        return "refs";

    case defs:
        return "defs";

    case funcs:
        return "funcs";

    case abs:
        return "abs";

    case labels:
        return "labels";

    case unknown:
        return "unknown";

    case macros:
        return "macros";
    }
    return rcstring::printf("%d", x);
}


void
arch_adaptable::random_test_data(void)
{
    opcodes.clear();
    int n = 10 + random() % 128;
    while (n > 0)
    {
        --n;
        rcstring opname = rname();
        int opvalue = random() & 0x7FFF;
        atribute_t opatrib = atribute_t(random() % 24);
        opcodes.push_back(oprec(opname, opvalue, opatrib));
    }
}

static rcstring ifn;
static FILE *ifp;
static int linum;


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
    token_comma,
    token_integer,
    token_lbrace,
    token_name,
    token_rbrace,
    token_string,
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


bool
arch_adaptable::atribute_from_name(const rcstring &name, atribute_t &value)
    const
{
    for (int j = 0; ; ++j)
    {
        rcstring n = arch_adaptable::atribute_name((atribute_t)j);
        if (isdigit(n[0]))
            return false;
        if (name == n)
        {
            value = (atribute_t)j;
            return true;
        }
    }
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

        case ',':
            tok = token_comma;
            return tok;

        case '{':
            tok = token_lbrace;
            return tok;

        case '}':
            tok = token_rbrace;
            return tok;

#if 0
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
#endif

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
                    tok = token_name;
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

        case '0':
            c = lex_getc();
            if (c == 'x' || c == 'X')
            {
                for (;;)
                {
                    c = lex_getc();
                    switch (c)
                    {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                        ac.push_back(c);
                        break;

                    default:
                        lex_ungetc(c);
                        if (ac.empty())
                            parse_error("malformed hex constant");
                        token_value_integer = strtol(ac.mkstr().c_str(), 0, 16);
                        tok = token_integer;
                        return tok;
                    }
                }
            }
            lex_ungetc(c);
            c = '0';
            // Fall through...

        case '1': case '2': case '3': case '4':
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
arch_adaptable::read_text(rcstring const &infile)
{
    ifn = infile;
    ifp = stdin;
    if (stdio_stream(ifn))
        ifn = "stdin";
    else
        ifp = explain_fopen_or_die(ifn.c_str(), "r");
    linum = 1;

    for (;;)
    {
        token_next();
        if (tok == token_eof)
            break;

        if (tok != token_lbrace)
            syntax_error();
        token_next();
        if (tok != token_string)
            syntax_error();
        rcstring opname = token_value_string;

        token_next();
        if (tok != token_comma)
            syntax_error();
        token_next();
        if (tok != token_integer)
            syntax_error();
        int opvalue = token_value_integer;

        token_next();
        if (tok != token_comma)
            syntax_error();
        token_next();
        atribute_t opatrib = (atribute_t)0;
        if (tok == token_integer)
        {
            opatrib = (atribute_t)token_value_integer;
        }
        else if (tok == token_name)
        {
            if (!atribute_from_name(token_value_string, opatrib))
            {
                parse_error
                (
                    "opcode attribute %s unknown",
                    token_value_string.quote_c().c_str()
                );
            }
        }
        else
        {
            syntax_error();
            // NOTREACHED
        }

        opcodes.push_back(oprec(opname, opvalue, opatrib));

        token_next();
        if (tok == token_comma)
            token_next();
        if (tok != token_rbrace)
            syntax_error();
        token_next();
        if (tok != token_comma)
            syntax_error();
    }
    if (ifp != stdin)
        explain_fclose_or_die(ifp);
}


// vim: set ts=8 sw=4 et :
