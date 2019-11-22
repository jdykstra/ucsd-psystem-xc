//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
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

#include <lib/ac/assert.h>
#include <lib/ac/stdio.h>
#include <lib/ac/stdlib.h>
#include <lib/ac/string.h>
#include <lib/ac/getopt.h>
#include <libexplain/fclose.h>
#include <libexplain/fopen.h>
#include <libexplain/fread.h>
#include <libexplain/fwrite.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <unistd.h>

#include <lib/byte_sex.h>
#include <lib/mtype.h>
#include <lib/p_machine.h>
#include <lib/rcstring/accumulator.h>
#include <lib/sizeof.h>
#include <lib/symtab.h>
#include <lib/version.h>

enum kind_t
{
    kind_boolean,
    kind_char,
    kind_integer,
};

struct table_t
{
    const char *name;
    unsigned word;
    unsigned bit;
    unsigned bit_width;
    kind_t kind;
};

static const table_t table[] =
{
    { "Has clock",                          29,  0,  1, kind_boolean },
    { "Has 8510a",                          29,  1,  1, kind_boolean },
    { "Has lower case",                     29,  2,  1, kind_boolean },
    { "Has random cursor addressing",       29,  3,  1, kind_boolean },
    { "Has slow terminal",                  29,  4,  1, kind_boolean },
    { "Student",                            29,  5,  1, kind_boolean },
    { "Has byte flipped machine",           29,  9,  1, kind_boolean },
    { "Has word oriented machine",          29, 10,  1, kind_boolean },
    { "Lead in to screen",                  31,  0,  8, kind_char },
    { "Move cursor home",                   31,  8,  8, kind_char },
    { "Erase to end of screen",             32,  0,  8, kind_char },
    { "Erase to end of line",               32,  8,  8, kind_char },
    { "Move cursor right",                  33,  0,  8, kind_char },
    { "Move cursor up",                     33,  8,  8, kind_char },
    { "Vertical move delay",                34,  8,  8, kind_integer },
    { "Backspace",                          34,  0,  8, kind_char },
    { "Erase line",                         35,  0,  8, kind_char },
    { "Erase screen",                       35,  8,  8, kind_char },
    { "Prefixed[Move cursor up]",           36,  0,  1, kind_boolean },
    { "Prefixed[Move cursor right]",        36,  1,  1, kind_boolean },
    { "Prefixed[Erase to end of line]",     36,  2,  1, kind_boolean },
    { "Prefixed[Erase to end of screen]",   36,  3,  1, kind_boolean },
    { "Prefixed[Move cursor home]",         36,  4,  1, kind_boolean },
    { "Prefixed[Delete character]",         36,  5,  1, kind_boolean },
    { "Prefixed[Erase screen]",             36,  6,  1, kind_boolean },
    { "Prefixed[Erase line]",               36,  7,  1, kind_boolean },
    { "Screen height",                      37,  0, 16, kind_integer },
    { "Screen width",                       38,  0, 16, kind_integer },
    { "Key to move cursor up",              39,  0,  8, kind_char },
    { "Key to move cursor down",            39,  8,  8, kind_char },
    { "Key to move cursor left",            40,  0,  8, kind_char },
    { "Key to move cursor right",           40,  8,  8, kind_char },
    { "Key to end file",                    41,  0,  8, kind_char },
    { "Key for flush",                      41,  8,  8, kind_char },
    { "Key for break",                      42,  0,  8, kind_char },
    { "Key for stop",                       42,  8,  8, kind_char },
    { "Key to delete character",            43,  0,  8, kind_char },
    { "Non-printing character",             43,  8,  8, kind_char },
    { "Key to delete line",                 44,  0,  8, kind_char },
    { "Editor escape key",                  44,  8,  8, kind_char },
    { "Lead in from keyboard",              45,  0,  8, kind_char },
    { "Editor accept key",                  45,  8,  8, kind_char },
    { "Prefixed[Key to move cursor right]", 47,  0,  1, kind_boolean },
    { "Prefixed[Key to move cursor left]",  47,  1,  1, kind_boolean },
    { "Prefixed[Key to move cursor up]",    47,  3,  1, kind_boolean },
    { "Prefixed[Key to move cursor down]",  47,  2,  1, kind_boolean },
    { "Prefixed[Non-printing character]",   47,  4,  1, kind_boolean },
    { "Prefixed[Key for stop]",             47,  6,  1, kind_boolean },
    { "Prefixed[Key for break]",            47,  7,  1, kind_boolean },
    { "Prefixed[Key for flush]",            47,  8,  1, kind_boolean },
    { "Prefixed[Key to end file]",          47,  9,  1, kind_boolean },
    { "Prefixed[Editor escape key]",        47, 10,  1, kind_boolean },
    { "Prefixed[Key to delete line]",       47, 11,  1, kind_boolean },
    { "Prefixed[Key to delete character]",  47, 12,  1, kind_boolean },
    { "Prefixed[Editor accept key]",        47, 13,  1, kind_boolean },
};


static unsigned char data[512];
static int number_of_errors;
static int line_number;


static void
barf_on_errors(const char *filename)
{
    if (number_of_errors > 0)
    {
        explain_output_error_and_die
        (
            "%s: found %d error%s\n",
            filename,
            number_of_errors,
            (number_of_errors == 1 ? "" : "s")
        );
    }
}


static byte_sex_t byte_sex = little_endian;


static void
set_byte_sex_by_arch(const rcstring &name)
{
    mtype_t mt = mtype_from_name(name);
    if (mt == mtype_undefined)
    {
        mt = mtype_from_name_fuzzy(name);
        if (mt != mtype_undefined)
        {
            explain_output_error_and_die
            (
                "achitecture %s unknown, did you mean %s instead?",
                name.quote_c().c_str(),
                mtype_name(mt).quote_c().c_str()
            );
        }
        explain_output_error_and_die
        (
            "achitecture %s unknown",
            name.quote_c().c_str()
        );
    }
    byte_sex = byte_sex_from_mtype(mt);
}


static void
set_byte_sex_by_host(const rcstring &name)
{
    mtype_t mt = mtype_from_host(name);
    if (mt == mtype_undefined)
    {
        explain_output_error_and_die("host %s unknown", name.quote_c().c_str());
    }
    byte_sex = byte_sex_from_mtype(mt);
}


static unsigned
get_word(const unsigned char *p)
{
    return byte_sex_get_word(byte_sex, p);
}


static void
put_word(unsigned char *p, unsigned short n)
{
    byte_sex_put_word(byte_sex, p, n);
}


static bool
split(const rcstring &line, rcstring &name, rcstring &value)
{
    const char *cp = strstr(line.c_str(), ":=");
    if (cp)
    {
        name = rcstring(line.c_str(), cp - line.c_str()).trim();
        value = rcstring(cp + 2).trim();
        return true;
    }

    cp = strchr(line.c_str(), '=');
    if (cp)
    {
        name = rcstring(line.c_str(), cp - line.c_str()).trim();
        value = rcstring(cp + 1).trim();
        return true;
    }

    cp = strchr(line.c_str(), ':');
    if (cp)
    {
        name = rcstring(line.c_str(), cp - line.c_str()).trim();
        value = rcstring(cp + 1).trim();
        return true;
    }

    return false;
}


static void
slurp_binary(const char *filename)
{
    FILE *fp = explain_fopen_or_die(filename, "r");
    size_t n = explain_fread_or_die(data, 1, sizeof(data), fp);
    int min_size = 96 * 2;
    if (n < 96 * 2)
    {
        fprintf
        (
            stderr,
            "%s: file size too small (%d < %d)",
            filename,
            (int)n,
            min_size
        );
        ++number_of_errors;
    }
    if (n < 512)
        memset(data + n, 0, 512 - n);
    explain_fclose_or_die(fp);

    barf_on_errors(filename);
}


static int
getc_without_comments(FILE *fp)
{
    int c = explain_getc_or_die(fp);

    // Ugly Comments.
    if (c == '{')
    {
        for (;;)
        {
            c = explain_getc_or_die(fp);
            if (c == '\n')
                ++line_number;
            if (c == EOF)
                return EOF;
            if (c == '}')
                return ' ';
        }
    }

    // Slightly Less Ugly Comments
    if (c == '(')
    {
        c = explain_getc_or_die(fp);
        if (c == '*')
        {
            for (;;)
            {
                c = explain_getc_or_die(fp);
                if (c == '\n')
                    ++line_number;
                if (c == EOF)
                    return EOF;
                if (c == '*')
                {
                    c = explain_getc_or_die(fp);
                    if (c == ')')
                        return ' ';
                    if (c != EOF)
                        ungetc(c, fp);
                }
            }
        }
        if (c != EOF)
            ungetc(c, fp);
        c = '(';
    }

    // Not a comment.
    return c;
}


static bool
read_one_line(FILE *fp, rcstring &result)
{
    rcstring_accumulator ac;
    for (;;)
    {
        int c = getc_without_comments(fp);
        if (c == EOF)
        {
            if (!ac.empty())
            {
                result = ac.mkstr();
                return true;
            }
            return false;
        }
        if (c == '\n')
        {
            result = ac.mkstr();
            return true;
        }
        ac.push_back((char)c);
    }
}


static int
parse_boolean(const rcstring &name, const char *filename)
{
    struct btab_t
    {
        const char *name;
        int value;
    };

    static const btab_t btab[] =
    {
        { "disable", 0 },
        { "enable", 1 },
        { "f", 0 },
        { "false", 0 },
        { "no", 0 },
        { "true", 1 },
        { "y", 1 },
        { "yes", 1 },
    };

    static symtab_t *stp;
    if (!stp)
    {
        stp = new symtab_t();
        for (const btab_t *tp = btab; tp < ENDOF(btab); ++tp)
            stp->assign(tp->name, (void *)tp);
    }

    rcstring lc_name = name.downcase();
    btab_t *tp = (btab_t *)stp->query(lc_name);
    if (!tp)
    {
        rcstring guess = stp->query_fuzzy(lc_name);
        if (guess.empty())
            return -1;
        tp = (btab_t *)stp->query(guess);
        assert(tp);
        if (!tp)
            return -1;
        fprintf
        (
            stderr,
            "%s: %d: value %s not understood, guessing you meant %s instead\n",
            filename, line_number,
            name.quote_c().c_str(),
            rcstring(tp->name).quote_c().c_str()
        );
        ++number_of_errors;
    }
    return tp->value;
}


static const char *char_names[] =
{
    "nul",
    "soh",
    "stx",
    "etx",
    "eot",
    "enq",
    "ack",
    "bel",
    "bs",
    "ht",
    "lf",
    "vt",
    "ff",
    "cr",
    "so",
    "si",
    "dle",
    "dc1",
    "dc2",
    "dc3",
    "dc4",
    "nak",
    "syn",
    "etb",
    "can",
    "em",
    "sub",
    "esc",
    "fs",
    "gs",
    "rs",
    "us",
    "sp",
};


static int
parse_char(const rcstring &name, const char *filename)
{
    // Look for character constants.
    if (name == "''''")
        return '\'';
    if (name.size() == 3 && name[0] == '\'' && name[2] == '\'')
        return name[1];

    // See if it looks like a number
    char *ep = 0;
    long n = strtol(name.c_str(), &ep, 10);
    if (ep != name.c_str())
        return n;

    static symtab_t *stp;
    if (!stp)
    {
        stp = new symtab_t();
        for (size_t j = 0; j < SIZEOF(char_names); ++j)
            stp->assign(char_names[j], (void *)(char_names + j));
        stp->assign("space", (void *)(char_names + ' '));
        stp->assign("newline", (void *)(char_names + '\n'));
        stp->assign("escape", (void *)(char_names + '\33'));
    }

    rcstring lc_name = name.downcase();
    void *tp = stp->query(lc_name);
    if (!tp)
    {
        rcstring guess = stp->query_fuzzy(lc_name);
        if (guess.empty())
            return -1;
        tp = stp->query(guess);
        assert(tp);
        if (!tp)
            return -1;
        fprintf
        (
            stderr,
            "%s: %d: value %s not understood, guessing you meant %s instead\n",
            filename,
            line_number,
            name.quote_c().c_str(),
            *(char **)tp
        );
        ++number_of_errors;
    }
    return ((const char **)tp - char_names);
}


static void
packed_assignment(unsigned word_index, unsigned bit_num, unsigned bit_width,
    unsigned short value)
{
    unsigned char *p = data + 2 * word_index;
    unsigned short n = get_word(p);
    unsigned mask = (1 << bit_width) - 1;
    n  = (n & ~(mask << bit_num)) | ((value & mask) << bit_num);
    put_word(p, n);
}


static void
slurp_text(const char *filename)
{
    symtab_t symbols;

    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        rcstring key = rcstring(tp->name).downcase();
        symbols.assign(key, (void *)tp);
    }

    memset(data, 0, sizeof(data));

    FILE *fp = explain_fopen_or_die(filename, "r");
    line_number = 0;
    for (;;)
    {
        ++line_number;
        rcstring line;
        if (!read_one_line(fp, line))
            break;
        line = line.trim();

        //
        // Ignore blabk lines.
        //
        if (line.empty())
            continue;

        //
        // Split the line into a name=value pair.
        //
        rcstring name;
        rcstring value;
        if (!split(line, name, value))
        {
            fprintf
            (
                stderr,
                "%s: %d: could not find a name := value pair\n",
                filename,
                line_number
            );
            ++number_of_errors;
            continue;
        }

        //
        // Look for the name in the table.
        //
        rcstring key = name.downcase();
        const table_t *tp = (const table_t *)symbols.query(key);
        if (!tp)
        {
            rcstring guess = symbols.query_fuzzy(key);
            if (guess.empty())
            {
                oops:
                fprintf
                (
                    stderr,
                    "%s: %d: field %s unknown\n",
                    filename,
                    line_number,
                    name.quote_c().c_str()
                );
                ++number_of_errors;
                continue;
            }
            else
            {
                tp = (const table_t *)symbols.query(guess);
                assert(tp);
                if (!tp)
                    goto oops;
                fprintf
                (
                    stderr,
                    "%s: %d: field %s unknown, guessing you meant %s instead\n",
                    filename,
                    line_number,
                    name.quote_c().c_str(),
                    rcstring(tp->name).quote_c().c_str()
                );
                ++number_of_errors;
            }
        }

        //
        // What to do with the value depends on the kind of the symbol.
        //
        int n = 0;
        switch (tp->kind)
        {
        case kind_boolean:
            n = parse_boolean(value, filename);
            if (n < 0)
            {
                fprintf
                (
                    stderr,
                    "%s: %d: value does not appear to be boolean\n",
                    filename,
                    line_number
                );
                ++number_of_errors;
                continue;
            }
            break;

        case kind_char:
            n = parse_char(value, filename);
            if (n < 0)
            {
                fprintf
                (
                    stderr,
                    "%s: %d: value does not appear to be a character\n",
                    filename,
                    line_number
                );
                ++number_of_errors;
                continue;
            }
            break;

        case kind_integer:
            // FIXME: the original UCSD setup tool provided octal and
            // hex as well (using O, D and H prefixes).  Should we do
            // the same, or do what the lib/pascal/lex.cc code does for
            // integer constants?
            n = strtol(value.c_str(), 0, 10);
            break;

        default:
            assert(!"unknown field kind");
            break;
        }

        //
        // Park the value.
        //
        packed_assignment(tp->word, tp->bit, tp->bit_width, n);
    }
    fclose(fp);

    barf_on_errors(filename);
}


static void
spew_binary(const char *filename)
{
    FILE *fp = explain_fopen_or_die(filename, "wb");
    explain_fwrite_or_die(data, 1, sizeof(data), fp);
    explain_fclose_or_die(fp);
}


static void
spew_text(const char *filename)
{
    size_t width = 0;
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        size_t len = strlen(tp->name);
        if (width < len)
            width = len;
    }
    width += 3;

    FILE *fp = explain_fopen_or_die(filename,"w");
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        rcstring s = rcstring(tp->name) + " :=";
        fprintf(fp, "%-*s ", (int)width, s.c_str());

        unsigned char *w = data + 2 * tp->word;
        unsigned field = get_word(w);
        unsigned mask = (1 << tp->bit_width) - 1;
        unsigned shift = tp->bit;
        field = (field >> shift) & mask;

        switch (tp->kind)
        {
        case kind_boolean:
            fprintf(fp, "%s", (field ? "true" : "false"));
            break;

        case kind_char:
            if (field < SIZEOF(char_names))
            {
                fprintf(fp, "%s", rcstring(char_names[field]).upcase().c_str());
                break;
            }
            // fall through...

        case kind_integer:
            fprintf(fp, "%d", field);
            break;
        }
        putc('\n', fp);
    }
    explain_fclose_or_die(fp);
}


static void
byte_sex_override(void)
{
    packed_assignment(29, 9, 1, byte_sex == big_endian);
}


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <infile> <outfile>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    bool decode = false;
    bool encode = false;
    bool byte_sex_force = false;
    for (;;)
    {
        static const struct option options[] =
        {
            { "architecture", 1, 0, 'A' },
            { "decode", 0, 0, 'd' },
            { "encode", 0, 0, 'e' },
            { "host", 1, 0, 'H' },
            { "machine", 1, 0, 'A' },
            { "p-machine", 1, 0, 'P' },
            { "version", 0, 0, 'V' },
            { 0, 0, 0, 0 }
        };
        int c = getopt_long(argc, argv, "A:edH:P:V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'A':
            set_byte_sex_by_arch(optarg);
            byte_sex_force = true;
            break;

        case 'H':
            set_byte_sex_by_host(optarg);
            byte_sex_force = true;
            break;

        case 'd':
            decode = true;
            break;

        case 'e':
            encode = true;
            break;

        case 'P':
            // The p-machine dictates the fields available in the SYSCOM area.
            p_machine_set(optarg);
            break;

        case 'V':
            version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind + 2 != argc)
        usage();
    if (!encode && !decode)
        explain_output_error_and_die("no function specified");
    if (encode && decode)
        explain_output_error_and_die("too many functions specified");
    if (decode)
    {
        slurp_binary(argv[optind++]);
        spew_text(argv[optind++]);
    }
    else
    {
        slurp_text(argv[optind++]);
        if (byte_sex_force)
            byte_sex_override();
        spew_binary(argv[optind++]);
    }
    return 0;
}


// vim: set ts=8 sw=4 et :
