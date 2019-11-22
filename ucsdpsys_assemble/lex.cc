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
#include <libexplain/output.h>
#include <lib/ac/list>
#include <unistd.h>

#include <lib/cardinal.h>
#include <lib/debug.h>
#include <lib/expression.h>
#include <lib/input/divert.h>
#include <lib/input/string.h>
#include <lib/location.h>
#include <lib/output/filter/prefix.h>
#include <lib/path_join.h>
#include <lib/rcstring/accumulator.h>
#include <lib/sizeof.h>
#include <lib/symtab.h>

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode.h>
#include <ucsdpsys_assemble/grammar.yacc.h> // MUST be last


static bool
exists(const rcstring &path)
{
    return (access(path.c_str(), F_OK) >= 0);
}


static rcstring_list include_path;
static rcstring_list view_path;


void
lex_view_path(const rcstring &dir)
{
    view_path.push_back_unique(dir);
}


void
lex_include_path(const rcstring &dir)
{
    include_path.push_back_unique(dir);
}


static rcstring
view_path_search(const rcstring &filename)
{
    if (filename[0] == '/')
        return filename;
    if (view_path.empty() || view_path[0] != ".")
        view_path.push_front(".");
    for (size_t j = 0; j < view_path.size(); ++j)
    {
        rcstring path = path_join(view_path[j], filename);
        if (exists(path))
            return path;
    }
    return rcstring();
}


static rcstring
include_path_search(const rcstring &filename)
{
    if (filename[0] == '/')
        return filename;
    if (include_path.empty() || include_path[0] != ".")
        include_path.push_front(".");
    for (size_t j = 0; j < include_path.size(); ++j)
    {
        rcstring path = path_join(include_path[j], filename);
        rcstring ok = view_path_search(path);
        if (!ok.empty())
            return ok;
    }
    return rcstring();
}


//
// This is really stupid, but consistent with the Apple Pascal assembler.
//
static int default_radix = 16;
static bool prev_was_include;


void
lex_default_radix_set(int n)
{
    assert(n >= 2);
    assert(n <= 36);
    default_radix = n;
}


static rcstring
unctl(int c)
{
    if (c < 0)
        return "EOF";
    c &= 0xFF;
    if (isprint(c))
        return rcstring::printf("'%c'", c);
    const char *esc = "\aa\bb\ff\nn\rr\tt\vv";
    const char *p = strchr(esc, c);
    if (p)
        return rcstring::printf("'\\%c'", p[1]);
    return rcstring::printf("'\\%03o'", c);
}

struct source
{
    ~source() { }
    source() : line_number(1), crank_line_number(false), prev(0) { }

    input::pointer is;
    int line_number;
    bool crank_line_number;
    source *prev;

    location
    get_location(void)
        const
    {
        return location(is->name(), line_number);
    }

    rcstring
    get_file_name(void)
        const
    {
        return is->name();
    }

    int
    fgetc(void)
    {
        if (crank_line_number)
        {
            crank_line_number = false;
            ++line_number;
        }
        int c = is->getch();
        if (c == '\n')
            crank_line_number = true;
        DEBUG(4, "fgetc() => %s", unctl(c).c_str());
        return c;
    }

    void
    ungetc(int c)
    {
        DEBUG(4, "ungetc(%s)", unctl(c).c_str());
        if (c == '\n')
            crank_line_number = false;
        if (c >= 0)
            is->ungetc(c);
    }

    void
    unread(const rcstring &text)
    {
        is->unread(text.c_str(), text.size());
    }
};


static int number_of_errors;
static source *src;
static location location_of_most_recent_token;


void
lex_open(const rcstring &filename)
{
    rcstring fn = view_path_search(filename);
    if (fn.empty())
        fn = filename;
    assert(!src);
    src = new source();
    src->is =
        input_divert::create
        (
            input::factory(fn),
            assembler::get_source_column()
        );
}


void
lex_open_include(const rcstring &filename)
{
    rcstring fn = include_path_search(filename);
    if (fn.empty())
        fn = filename;
    source *nsrc = new source();
    nsrc->is = input::factory(fn);
    nsrc->prev = src;
    src = nsrc;
}


void
lex_close(void)
{
    assert(src);
    while (src->prev)
    {
        source *old = src;
        src = old->prev;
        delete old;
    }
    if (number_of_errors)
    {
        explain_output_error_and_die
        (
            "%s: found %s fatal error%s",
            src->get_file_name().c_str(),
            cardinal(number_of_errors).c_str(),
            (number_of_errors == 1 ? "" : "s")
        );
    }
    delete src;
    src = 0;
}


static int
reserved(const rcstring &name)
{
    struct table_t
    {
        const char *name;
        int value;
    };

    static const table_t table[] =
    {
        { "$", DOLLAR },
        { ".", DOLLAR },
        { ".else", DOT_ELSE },
        { ".end", DOT_END },
        { ".endc", DOT_ENDC },
        { ".endif", DOT_ENDC },
        { ".eq", EQ },
        { ".equ", DOT_EQU },
        { ".func", DOT_FUNC },
        { ".ge", GE },
        { ".gt", GT },
        { ".if", DOT_IF },
        { ".le", LE },
        { ".lt", LT },
        { ".macro", DOT_MACRO },
        { ".ne", NE },
        { ".proc", DOT_PROC },
    };

    static symtab_t *stp;
    if (!stp)
    {
        stp = new symtab_t(SIZEOF(table));
        for (const table_t *tp = table; tp < ENDOF(table); ++tp)
        {
            stp->assign(tp->name, (void *)tp);
        }
    }

    const table_t *p = (const table_t *)stp->query(name.downcase());
    return (p ? p->value : 0);
}


static bool
next_is_hex(void)
{
    int c1 = src->fgetc();
    if (c1 < 0)
        return false;
    switch (c1)
    {
    case 'x':
    case 'X':
        break;

    default:
        src->ungetc(c1);
        return false;
    }

    int c2 = src->fgetc();
    if (c2 < 0)
    {
        src->ungetc(c1);
        return false;
    }
    src->ungetc(c2);
    switch (c2)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        break;

    default:
        src->ungetc(c1);
        return false;
    }

    // we have consumed the 'X'
    return true;
}


static bool
next_is(char candidate)
{
    int c = src->fgetc();
    if (c < 0)
        return false;
    if (c == candidate)
        return true;
    src->ungetc(c);
    return false;
}


//
// Numbers are ugly.
//
static int
number(void)
{
    DEBUG(3, "%s {", __PRETTY_FUNCTION__);
    static rcstring_accumulator ac;

    //
    // Look for decimal numbers ending with a dot.
    // Look for floating-point numbers containing a dot.
    // Look for radix numbers containing a hash.
    //
    {
        int c = src->fgetc();
        switch (c)
        {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                // digit+ '.' digit+   => real
                // digit+ '.'          => decimal
                // digit+ '#' rdigit+  => dec radix followed by number
                // digit+              => undo and ignore

                enum state_t
                {
                    // @ digit+ '.' digit+
                    // @ digit+ '.'
                    // @ digit+ '#' rdigit+
                    // digit+ @ digit+ '#' rdigit+
                    // digit+ @ '#' rdigit+
                    // digit+ @ digit+ '.'
                    // digit+ @ digit+ '.' digit+
                    // digit+ @ '.' digit+
                    // digit+ @ '.'
                    state_could_be_anything,

                    // digit+ '#' @ rdigit+
                    state_seen_hash,

                    // digit+ '.' @
                    // digit+ '.' @ digit+
                    state_seen_dot,

                    // digit+ '.' digit @ digit+
                    // digit+ '.' digit @
                    state_definitely_real,

                    // digit+ '#' rdigit @ rdigit+
                    // digit+ '#' rdigit @
                    state_definitely_radix
                };

                state_t state = state_could_be_anything;
                ac.clear();
                int hash_radix = 10;
                int d = 0;
                for (;;)
                {
                    ac.push_back((char)c);

                    c = src->fgetc();
                    switch (state)
                    {
                    case state_could_be_anything:
                        if (c < 0)
                        {
                            src->is->unread(ac.get_data(), ac.size());
                            goto not_explicit_decimal;
                        }
                        switch (c)
                        {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            break;

                        default:
                            src->ungetc(c);
                            src->is->unread(ac.get_data(), ac.size());
                            goto not_explicit_decimal;

                        case '.':
                            state = state_seen_dot;
                            break;

                        case '#':
                            hash_radix = strtol(ac.mkstr().c_str(), 0, 10);
                            if (hash_radix < 2 || hash_radix > 36)
                            {
                                grammar_error
                                (
                                    "radix %d is outside range 2..36",
                                    hash_radix
                                );
                                hash_radix = (hash_radix < 10 ? 10 : 36);
                            }
                            state = state_seen_hash;
                            break;
                        }
                        break;

                    case state_seen_dot:
                        if (c < 0)
                        {
                            decimal:
                            grammar_lval.lv_integer =
                                strtol(ac.mkstr().c_str(), 0, 10);
                            DEBUG(3, "dec }");
                            return INTEGER_CONSTANT;
                        }
                        switch (c)
                        {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            state = state_definitely_real;
                            break;

                        default:
                            src->ungetc(c);
                            goto decimal;
                        }
                        break;

                    case state_seen_hash:
                        if (c < 0)
                        {
                            hash_is_bogus:
                            grammar_error("malformed explicit-radix constant");
                            grammar_lval.lv_integer = 0;
                            DEBUG(3, "yuck }");
                            return INTEGER_CONSTANT;
                        }
                        ac.clear();
                        switch (c)
                        {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            d = c - '0';
                            break;

                        case 'a': case 'b': case 'c': case 'd': case 'e':
                        case 'f': case 'g': case 'h': case 'i': case 'j':
                        case 'k': case 'l': case 'm': case 'n': case 'o':
                        case 'p': case 'q': case 'r': case 's': case 't':
                        case 'u': case 'v': case 'w': case 'x': case 'y':
                        case 'z':
                            d = 10 + c - 'a';
                            break;

                        case 'A': case 'B': case 'C': case 'D': case 'E':
                        case 'F': case 'G': case 'H': case 'I': case 'J':
                        case 'K': case 'L': case 'M': case 'N': case 'O':
                        case 'P': case 'Q': case 'R': case 'S': case 'T':
                        case 'U': case 'V': case 'W': case 'X': case 'Y':
                        case 'Z':
                            d = 10 + c - 'A';
                            break;

                        default:
                            src->ungetc(c);
                            goto hash_is_bogus;
                        }
                        if (d >= hash_radix)
                        {
                            src->ungetc(c);
                            goto hash_is_bogus;
                        }
                        state = state_definitely_radix;
                        break;

                    case state_definitely_radix:
                        if (c < 0)
                        {
                            radix_constant:
                            grammar_lval.lv_integer =
                                strtol(ac.mkstr().c_str(), 0, hash_radix);
                            DEBUG(3, "rad }");
                            return INTEGER_CONSTANT;
                        }
                        switch (c)
                        {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            d = c - '0';
                            break;

                        case 'a': case 'b': case 'c': case 'd': case 'e':
                        case 'f': case 'g': case 'h': case 'i': case 'j':
                        case 'k': case 'l': case 'm': case 'n': case 'o':
                        case 'p': case 'q': case 'r': case 's': case 't':
                        case 'u': case 'v': case 'w': case 'x': case 'y':
                        case 'z':
                            d = 10 + c - 'a';
                            break;

                        case 'A': case 'B': case 'C': case 'D': case 'E':
                        case 'F': case 'G': case 'H': case 'I': case 'J':
                        case 'K': case 'L': case 'M': case 'N': case 'O':
                        case 'P': case 'Q': case 'R': case 'S': case 'T':
                        case 'U': case 'V': case 'W': case 'X': case 'Y':
                        case 'Z':
                            d = 10 + c - 'A';
                            break;

                        default:
                            src->ungetc(c);
                            goto radix_constant;
                        }
                        if (d >= hash_radix)
                        {
                            src->ungetc(c);
                            goto radix_constant;
                        }
                        break;

                    case state_definitely_real:
                        if (c < 0)
                        {
                            real_constant:
                            grammar_lval.lv_real =
                                strtod(ac.mkstr().c_str(), 0);
                            DEBUG(3, "real }");
                            return REAL_CONSTANT;
                        }
                        switch (c)
                        {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            break;

                        default:
                            src->ungetc(c);
                            goto real_constant;
                        }
                        break;
                    }
                }
            }

        default:
            // not our sort of number
            src->ungetc(c);
            break;
        }
        not_explicit_decimal:
        ;
    }

    //
    // Look for C-style hexadecimal constants.
    //
    if (default_radix < 10 + 'X' - 'A')
    {
        int c = src->fgetc();
        if (c == '0' && next_is_hex())
        {
            // C-style hex constant
            ac.clear();
            for (;;)
            {
                ac.push_back((char)c);

                c = src->fgetc();
                if (c < 0)
                    break;
                switch (c)
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    continue;

                default:
                    src->ungetc(c);
                    break;
                }
                break;
            }
            grammar_lval.lv_integer =
                strtol(ac.mkstr().c_str(), 0, 16);
            DEBUG(3, "C hex }");
            return INTEGER_CONSTANT;
        }
        src->ungetc(c);
    }

    if (default_radix < 10 + 'H' - 'A')
    {
        //
        // Look for hexadecimal constants that end with 'H'.
        //
        int c = src->fgetc();
        switch (c)
        {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                ac.clear();
                for (;;)
                {
                    ac.push_back((char)c);
                    c = src->fgetc();
                    if (c < 0)
                        break;
                    switch (c)
                    {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                        continue;

                    default:
                        break;
                    }
                    break;
                }
                switch (c)
                {
                case 'H':
                case 'h':
                    grammar_lval.lv_integer =
                        strtol(ac.mkstr().c_str(), 0, 16);
                    DEBUG(3, "hex h }");
                    return INTEGER_CONSTANT;

                default:
                    src->ungetc(c);
                    break;
                }
                // Nope, not a hex constant.  Give all the input back.
                src->is->unread(ac.get_data(), ac.size());
            }
            break;

        default:
            // not our sort of number
            src->ungetc(c);
            break;
        }
    }

    //
    // Look for numbers using the default radix.  The only bright spot
    // is that this sort of number must start with a digit.
    //
    {
        int c = src->fgetc();
        switch (c)
        {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            assert(default_radix >= 2);
            assert(default_radix <= 36);
            if (c - '0' >= default_radix)
            {
                // not our sort of number
                src->ungetc(c);
                break;
            }

            ac.clear();
            for (;;)
            {
                ac.push_back((char)c);
                c = src->fgetc();
                switch (c)
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    if (c - '0' < default_radix)
                        continue;
                    break;

                case 'a': case 'b': case 'c': case 'd': case 'e':
                case 'f': case 'g': case 'h': case 'i': case 'j':
                case 'k': case 'l': case 'm': case 'n': case 'o':
                case 'p': case 'q': case 'r': case 's': case 't':
                case 'u': case 'v': case 'w': case 'x': case 'y':
                case 'z':
                    if (c + 10 - 'a' < default_radix)
                        continue;
                    break;

                case 'A': case 'B': case 'C': case 'D': case 'E':
                case 'F': case 'G': case 'H': case 'I': case 'J':
                case 'K': case 'L': case 'M': case 'N': case 'O':
                case 'P': case 'Q': case 'R': case 'S': case 'T':
                case 'U': case 'V': case 'W': case 'X': case 'Y':
                case 'Z':
                    if (c + 10 - 'A' < default_radix)
                        continue;
                    break;

                default:
                    break;
                }
                break;
            }
            src->ungetc(c);
            grammar_lval.lv_integer =
                strtol(ac.mkstr().c_str(), 0, default_radix);
            DEBUG(3, "def rad }");
            return INTEGER_CONSTANT;

        default:
            // not our sort of number
            src->ungetc(c);
            break;
        }
    }

    // Not looking at a number
    DEBUG(3, "not }");
    return 0;
}


static int
grammar_lex_inner(void)
{
    bool previnc = prev_was_include;
    prev_was_include = false;
    assert(src);
    for (;;)
    {
        location_of_most_recent_token = src->get_location();

        if (previnc)
        {
            int c = src->fgetc();
            while (c == ' ' || c == '\t')
                c = src->fgetc();
            if (c >= 0 && c != ';' && c != '\n')
            {
                rcstring_accumulator ac;
                for (;;)
                {
                    ac.push_back((char)c);
                    c = src->fgetc();
                    if (c < 0)
                        break;
                    if (c == ';' || c == '\n')
                    {
                        src->ungetc(c);
                        break;
                    }
                }
                rcstring text = ac.mkstr();
                grammar_lval.lv_string = new rcstring(text);
                return STRING_CONSTANT;
            }
            else
                src->ungetc(c);
        }

        // Numbers are ugly.  The number function looks for all the
        // weird and ugly sorts of numbers before anything else.
        {
            int tok = number();
            if (tok)
                return tok;
        }

        int c = src->fgetc();
        if (c < 0)
        {
            if (src->prev)
            {
                source *old = src;
                src = old->prev;
                delete old;
                continue;
            }
            return 0;
        }
        switch (c)
        {
        case ' ':
        case '\t':
        case '\f':
        case '\r':
            break;

        case '\n':
            return EOLN;

        case '!':
            if (next_is('='))
                return NE;
            return NOT;

        case '"':
            string_constant:
            {
                rcstring_accumulator ac;
                char term = c;
                for (;;)
                {
                    c = src->fgetc();
                    if (c < 0)
                        break;
                    if (c == '\n')
                    {
                        src->ungetc(c);
                        break;
                    }

                    if (c == term)
                    {
                        c = src->fgetc();
                        if (c < 0)
                            break;
                        if (c != term)
                        {
                            src->ungetc(c);
                            break;
                        }
                    }
                    ac.push_back((char)c);
                }
                rcstring text = ac.mkstr();
                if (term == '\'' && text.size() == 1)
                {
                    grammar_lval.lv_integer = (unsigned char)text[0];
                    return INTEGER_CONSTANT;
                }
                grammar_lval.lv_string = new rcstring(text);
                return STRING_CONSTANT;
            }

        case '#':
            return HASH;

        case '$':
            goto name;

        case '%':
            return MOD;

        case '&':
            if (next_is('&'))
                return AND_AND;
            return BIT_AND;

        case '\'':
            goto string_constant;

        case '(':
            return LP;

        case ')':
            return RP;

        case '*':
            return STAR;

        case '+':
            if (next_is('+'))
                return PLUS_PLUS;
            return PLUS;

        case ',':
            return COMMA;

        case '-':
            if (next_is('-'))
                return MINUS_MINUS;
            return MINUS;

        case '.':
            goto name;

        case '/':
            return DIV;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // All numbers should have been taken
            // care of before we reach this point.
            assert(false);

        case ':':
            if (next_is('='))
                return ASSIGN;
            return COLON;

        case ';':
            // comment
            for (;;)
            {
                c = src->fgetc();
                if (c < 0)
                    break;
                if (c == '\n')
                {
                    return EOLN;
                }
            }
            break;

        case '<':
            if (next_is('>'))
                return NE;
            if (next_is('='))
                return LE;
            //
            // This is from using PDP-11 assembler as the UCSD p-System's
            // first assembly language, because the PDP-11 assembler ^%$@*ed
            // up parentheses to mean indirection (sort of), they had to have
            // another way of grouping expressions to override operator
            // precedence (i.e. what perentheses do in any sane language).
            //
            // This leaves you with no way to write "less than", or we
            // could use "<<" to mean less than, and then have no way
            // to write "shift left".  Sheesh.  Of course, historically
            // they never could, so it probably isn't a problem.
            //
            return LGROUP;

        case '=':
            if (next_is('='))
                return EQ;
            return EQ;

        case '>':
            if (next_is('='))
                return GE;
            // See rant above.
            return RGROUP;

        case '?':
            return QUESTION_MARK;

        case '@':
            return AT;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            name:
            {
                rcstring_accumulator ac;
                for (;;)
                {
                    ac.push_back((char)c);
                    c = src->fgetc();
                    if (c < 0)
                        break;
                    switch (c)
                    {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case '_':
                    case 'a': case 'b': case 'c': case 'd': case 'e':
                    case 'f': case 'g': case 'h': case 'i': case 'j':
                    case 'k': case 'l': case 'm': case 'n': case 'o':
                    case 'p': case 'q': case 'r': case 's': case 't':
                    case 'u': case 'v': case 'w': case 'x': case 'y':
                    case 'z':
                    case 'A': case 'B': case 'C': case 'D': case 'E':
                    case 'F': case 'G': case 'H': case 'I': case 'J':
                    case 'K': case 'L': case 'M': case 'N': case 'O':
                    case 'P': case 'Q': case 'R': case 'S': case 'T':
                    case 'U': case 'V': case 'W': case 'X': case 'Y':
                    case 'Z':
                        continue;

                    default:
                        src->ungetc(c);
                        break;
                    }
                    break;
                }
                rcstring name = ac.mkstr();
                int tok = reserved(name);
                if (tok)
                {
                    return tok;
                }

                //
                // Check to see if it is an opcode name.
                //
                opcode::pointer op = grammar_lookup_opcode(name);
                if (op)
                {
                    prev_was_include = op->next_argument_always_a_string();
                    grammar_lval.lv_opcode = new opcode::pointer(op);
                    switch (op->get_kind())
                    {
                    default:
                    case opcode::kind_standard:
                        return OPCODE;

                    case opcode::kind_6502:
                        // Conceivably, other chip makers could have
                        // used the same stupid syntax; but, since they
                        // all suffered from NIH, probably not.
                        return OPCODE_6502;

                    case opcode::kind_prelude:
                        return OPCODE_PRELUDE;

                    case opcode::kind_name_list:
                        return OPCODE_NAME_LIST;
                    }
                }

                //
                // Check to see if it is a special name (e.g. a CPU
                // register name).
                //
                expression::pointer ep = grammar_lookup_special(name);
                if (ep)
                {
                    ep = ep->clone(location_of_most_recent_token);
                    grammar_lval.lv_expression = new expression::pointer(ep);
                    return SPECIAL;
                }

                //
                // No, nothing special.
                //
                grammar_lval.lv_string = new rcstring(name);
                return NAME;
            }

        case '[':
            return LB;

        case ']':
            return RB;

        case '^':
            return CARAT;

        case '_':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            goto name;

        case '{':
            return LBRACE;

        case '|':
            if (next_is('|'))
                return OR_OR;
            return BIT_OR;

        case '}':
            return RBRACE;

        case '~':
            return TILDE;

        default:
            return JUNK;
        }
    }
}


static std::list<int> conditional_stack;


static bool
lex_conditional_active(void)
{
    return conditional_stack.empty() || (conditional_stack.back() & 1);
}


void
lex_conditional_push(bool yesno)
{
    // bit 0: effective result
    // bit 1: what was given
    int n = (yesno ? 2 : 0) | ((lex_conditional_active() && yesno) ? 1 : 0);
    conditional_stack.push_back(n);
}


static bool
lex_conditional_pop(void)
{
    bool result = !!(2 & conditional_stack.back());
    conditional_stack.pop_back();
    return result;
}

static void
eat_until_eoln(void)
{
    for (;;)
    {
        int tok = grammar_lex_inner();
        switch (tok)
        {
        case EOLN:
        case 0:
            return;
        }
    }
}


int
grammar_lex(void)
{
    for (;;)
    {
        int tok = grammar_lex_inner();
        switch (tok)
        {
        case 0:
            if (!conditional_stack.empty())
                grammar_error("conditional stack not empty");
            return 0;

        case DOT_ENDC:
            if (conditional_stack.empty())
                return tok;
            eat_until_eoln();
            lex_conditional_pop();
            break;

        case DOT_IF:
            if (lex_conditional_active())
                return tok;
            eat_until_eoln();
            lex_conditional_push(true);
            break;

        case DOT_ELSE:
            if (conditional_stack.empty())
                return tok;
            eat_until_eoln();
            lex_conditional_push(!lex_conditional_pop());
            break;

        default:
            if (lex_conditional_active())
                return tok;
            // discard everything else
            break;
        }
    }
}


void
lex_error(const location &locn, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    rcstring msg = rcstring::vprintf(fmt, ap);
    va_end(ap);
    explain_output_error("%s: %s", locn.representation().c_str(), msg.c_str());
    ++number_of_errors;

    output::pointer op =
        output_filter_prefix::create(assembler::get_source_column(), "error: ");
    op->fputs(msg);
    op->fputc('\n');
}


void
lex_message(const location &locn, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    rcstring msg = rcstring::vprintf(fmt, ap);
    va_end(ap);
    explain_output_error("%s: %s", locn.representation().c_str(), msg.c_str());

    output::pointer op =
        output_filter_prefix::create(assembler::get_source_column(), "print: ");
    op->fputs(msg);
    op->fputc('\n');
}


void
lex_warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    rcstring msg = rcstring::vprintf(fmt, ap);
    va_end(ap);
    explain_output_error
    (
        "%s: warning: %s",
        lex_location().representation().c_str(),
        msg.c_str()
    );

    output::pointer op =
      output_filter_prefix::create(assembler::get_source_column(), "warning: ");
    op->fputs(msg);
    op->fputc('\n');
}


void
grammar_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    rcstring msg = rcstring::vprintf(fmt, ap);
    va_end(ap);
    explain_output_error
    (
        "%s: %s",
        location_of_most_recent_token.representation().c_str(),
        msg.c_str()
    );
    ++number_of_errors;
}


void
grammar_warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    rcstring msg = rcstring::vprintf(fmt, ap);
    va_end(ap);
    explain_output_error
    (
        "%s: warning: %s",
        location_of_most_recent_token.representation().c_str(),
        msg.c_str()
    );
}


location
lex_location(void)
{
    assert(!location_of_most_recent_token.empty());
    return location_of_most_recent_token;
}


void
lex_trace_print(void *, const char *fmt, ...)
{
    static char buffer[2000];
    va_list ap;
    va_start(ap, fmt);
    rcstring msg = rcstring::vprintf(fmt, ap);
    va_end(ap);
    strcat(buffer, msg.c_str());
    for (;;)
    {
        int len2;
        int len;
        char *ep = strchr(buffer, '\n');
        if (!ep)
            return;
        len = ep - buffer;
        explain_output_error
        (
            "%s: %d: %.*s",
            src->is->name().c_str(),
            src->line_number,
            len,
            buffer
        );
        ++ep;
        len2 = strlen(ep) + 1;
        memmove(buffer, ep, len2);
    }
}


static bool
lex_macro_line(rcstring &line)
{
    static rcstring_accumulator ac;
    ac.clear();
    for (;;)
    {
        int c = src->fgetc();
        if (c < 0)
        {
            if (ac.empty())
                return false;
            ac.push_back('\n');
            line = ac.mkstr();
            return true;
        }
        ac.push_back((unsigned char)c);
        if (c == '\n')
        {
            line = ac.mkstr();
            return true;
        }
    }
}


static inline bool
safe_isspace(char c)
{
    return isspace((unsigned char)c);
}


static bool
lex_macro_match(const rcstring &line, const char *pattern)
{
    const char *cp = line.c_str();
    while (*cp && safe_isspace(*cp))
        ++cp;
    size_t plen = strlen(pattern);
    if (0 != strncasecmp(cp, pattern, plen))
        return false;
    cp += plen;
    return (!*cp || safe_isspace(*cp));
}


void
lex_macro_slurp(rcstring_list &contents)
{
    DEBUG(2, "%s", __PRETTY_FUNCTION__);
    for (;;)
    {
        rcstring line;
        if (!lex_macro_line(line))
        {
            no_endm:
            grammar_error("macro missing .endm");
            return;
        }
        if (lex_macro_match(line, ".end"))
        {
            src->unread(line);
            goto no_endm;
        }
        if (lex_macro_match(line, ".endm"))
            return;
        contents.push_back(line);
    }
}


void
lex_macro_insert(const rcstring &text)
{
    DEBUG(2, "%s", __PRETTY_FUNCTION__);
    source *nsrc = new source();
    nsrc->is =
        input_divert::create
        (
            input_string::create(text, "macro"),
            assembler::get_source_column()
        );
    nsrc->prev = src;
    src = nsrc;
}


void
lex_discard_input(void)
{
    assert(src);
    while (src->prev)
    {
        source *old = src;
        src = old->prev;
        delete old;
    }
    while (src->fgetc() >= 0)
        ;
}


// vim: set ts=8 sw=4 et :
