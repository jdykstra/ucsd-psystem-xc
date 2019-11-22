//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#include <lib/ac/assert.h>
#include <lib/ac/ctype.h>
#include <lib/ac/stdio.h>
#include <lib/ac/string.h>
#include <libexplain/output.h>
#include <unistd.h>

#include <lib/cardinal.h>
#include <lib/debug.h>
#include <lib/expression.h>
#include <lib/pascal/lex.h>
#include <lib/input.h>
#include <lib/input/string.h>
#include <lib/pascal_id.h>
#include <lib/path_join.h>
#include <lib/sizeof.h>
#include <lib/statement.h>
#include <lib/type/record.h>
#include <lib/rcstring/accumulator.h>
#include <lib/symtab/template.h>

class bit_address; // forward
class case_list; // forward
class case_range; // forward
class expression_list; // forward
class name_type; // forward
class name_type_list; // forward
class statement_list; // forward
class type_list; // forward
class variable_name; // forward
class variable_name_list; // forward
#include <lib/pascal/grammar.yacc.h>

class source
{
public:
    ~source()
    {
        is.reset();
        prev = 0;
        delete snoop;
        snoop = 0;
    }

    source() :
        line_number(1),
        crank_line_number(0),
        prev(0),
        snoop(0)
    {
    }

    source(source *sp) :
        line_number(1),
        crank_line_number(0),
        prev(sp),
        snoop(0)
    {
    }

    void
    open(const input::pointer &p)
    {
        DEBUG(2, "%s", __PRETTY_FUNCTION__);
        is = p;
    }

    int
    getch(void)
    {
        DEBUG(2, "%s", __PRETTY_FUNCTION__);
        int c = is->getch();
        if (snoop && c > 0)
            snoop->push_back((char)c);
        if (c == '\n')
            ++crank_line_number;
        return c;
    }

    void
    ungetc(int c)
    {
        DEBUG(2, "%s", __PRETTY_FUNCTION__);
        if (c >= 0)
            is->ungetc(c);
        if (snoop && c > 0)
            snoop->pop_back();
        if (c == '\n')
            --crank_line_number;
    }

    void
    open(const rcstring &filename)
    {
        DEBUG(2, "%s", __PRETTY_FUNCTION__);
        is = input::factory(filename);
    }

    source *get_prev(void) const { return prev; }

    rcstring
    name_and_line(void)
        const
    {
        return get_location().representation();
    }

    void
    next_line_is_now(void)
    {
        line_number += crank_line_number;
        crank_line_number = 0;
    }

    location
    get_location(void)
        const
    {
        return location(is->name(), line_number);
    }

    void
    snoop_begin(void)
    {
        DEBUG(2, "%s", __PRETTY_FUNCTION__);
        // This can get ugly if the input text has two INTERFACE
        // keywords in it.
        if (snoop)
            delete snoop;
        snoop = new rcstring_accumulator();
    }

    rcstring
    snoop_end(size_t less)
    {
        DEBUG(2, "%s", __PRETTY_FUNCTION__);
        // This can get ugly if the IMPLEMENTATION keywords is misplaced
        // or unbalanced.
        if (!snoop)
            return "";
        assert(snoop->size() >= less);
        while (less > 0)
        {
            --less;
            snoop->pop_back();
        }
        while (!snoop->empty() && isspace((unsigned char)snoop->back()))
            snoop->pop_back();
        rcstring result = snoop->mkstr();
        delete snoop;
        snoop = 0;
        return result;
    }

private:
    input::pointer is;
    int line_number;
    int crank_line_number;
    source *prev;
    rcstring_accumulator *snoop;
};


static source *src;
static int number_of_errors;
static location curloc;
static rcstring_list include_path;
static rcstring_list view_path;
static int total_lines;

#define TOO_MANY_ERRORS 50


static bool
exists(const rcstring &path)
{
    return (access(path.c_str(), F_OK) == 0);
}


static rcstring
view_path_find(const rcstring &filename)
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


void
pascal_lex_open(const rcstring &filename)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring fn = view_path_find(filename);
    if (fn.empty())
        fn = filename;
    src = new source;
    src->open(fn);
    number_of_errors = 0;
}


static rcstring
include_path_search(const rcstring &filename)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (filename[0] == '/')
        return filename;
    if (include_path.empty() || include_path[0] != ".")
        include_path.push_front(".");
    for (size_t j = 0; j < include_path.size(); ++j)
    {
        rcstring dir = include_path[j];
        rcstring path = path_join(dir, filename);
        rcstring ok = view_path_find(path);
        if (!ok.empty())
            return ok;
    }
    return rcstring();
}


void
pascal_lex_include(const rcstring &filename)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring path = include_path_search(filename);
    if (path.empty())
    {
        path = include_path_search(filename + ".text");
        if (path.empty())
            path = filename;
    }

    source *sp = new source(src);
    sp->open(path);
    src = sp;
}


void
pascal_lex_include_directory(const rcstring &path)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (path.starts_with("./"))
    {
        rcstring path2 = path.substring(2, path.size());
        if (path2.empty())
            path2 = ".";
        include_path.push_back(path2);
    }
    else
    {
        include_path.push_back(path);
    }
}


void
pascal_lex_viewpath(const rcstring &path)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (path.starts_with("./"))
    {
        rcstring path2 = path.substring(2, path.size());
        if (path2.empty())
            path2 = ".";
        view_path.push_back(path2);
    }
    else
    {
        view_path.push_back(path);
    }
}


void
pascal_lex_close(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    while (src)
    {
        source *p = src;
        src = p->get_prev();
        delete p;
    }
    pascal_lex_error_summary();
}


void
pascal_lex_error_summary(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (number_of_errors > 0)
    {
        int style = cardinal_style_all;
        pascal_grammar_fail_hook();
        explain_output_error_and_die
        (
            "found %s fatal error%s",
            cardinal(number_of_errors, style).c_str(),
            (number_of_errors == 1 ? "" : "s")
        );
    }
}


static int
lex_getc(void)
{
    for (;;)
    {
        if (!src)
            return -1;
        int c = src->getch();
        if (c < 0 && src->get_prev())
        {
            source *p = src;
            src = p->get_prev();
            delete p;
            continue;
        }
        if (c == '\n')
        {
            ++total_lines;
            return c;
        }
        return c;
    }
}


static void
lex_ungetc(int c)
{
    assert(src);
    if (c == '\n')
    {
        --total_lines;
    }
    if (c >= 0)
        src->ungetc(c);
}


static int
lex_peek(void)
{
    int c = lex_getc();
    lex_ungetc(c);
    return c;
}


static rcstring
an_from_a(const rcstring &s)
{
    rcstring_accumulator ac;
    const char *cp = s.c_str();
    for (;;)
    {
        unsigned char c = *cp++;
        if (!c)
            return ac.mkstr();
        if
        (
            c == ' '
        &&
            cp[0] == 'a'
        &&
            cp[1] == ' '
        &&
            isalpha((unsigned char)cp[2])
        &&
            strchr("AaEeIiOoUu", cp[2])
        )
        {
            ac.push_back(" an ");
            cp += 2;
        }
        else
            ac.push_back(c);
    }
}


static void
pascal_lex_error_v(const location &loc, const char *fmt, va_list ap)
{
    rcstring msg(an_from_a(rcstring::vprintf(fmt, ap)));

    rcstring locn = (loc.empty() ? curloc : loc).representation();
    explain_output_error("%s: %s", locn.c_str(), msg.c_str());

    // Don't bump the error count for the second line of error messages;
    // continuation lines start with "..."
    if (*fmt != '.')
        ++number_of_errors;

    //
    // Bail if there are too many errors (so long as the message isn't
    // continued on the next line).
    //
    if
    (
        number_of_errors >= TOO_MANY_ERRORS
    &&
        !msg.empty()
    &&
        msg[msg.size() - 1] != '.'
    )
    {
        pascal_lex_error_summary();
    }

    //
    // If we have reached the end of input, and the error occurs after
    // that, exit immediately.
    //
    if (!src)
    {
        pascal_grammar_fail_hook();
        explain_output_exit_failure();
    }
}


void
pascal_grammar_error(const char *text)
{
    pascal_lex_error(curloc, "%s", text);
}


void
pascal_lex_error(const location &loc, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    pascal_lex_error_v(loc, fmt, ap);
    va_end(ap);
}


static void
pascal_lex_warning_v(const location &loc, const char *fmt, va_list ap)
{
    if (pascal_grammar_warnings_are_errors())
    {
        pascal_lex_error_v(loc, fmt, ap);
        return;
    }

    rcstring msg(an_from_a(rcstring::vprintf(fmt, ap)));
    // Even if warnings are silent, we still need to "use up" all of the
    // variable arguments on some quirky architectures (otherwise they
    // can't find their return address).
    if (pascal_grammar_warnings_are_silent())
        return;
    rcstring locn = (loc.empty() ? curloc : loc).representation();
    explain_output_error("%s: warning: %s", locn.c_str(), msg.c_str());
}


void
pascal_lex_warning(const location &loc, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    pascal_lex_warning_v(loc, fmt, ap);
    va_end(ap);
}


void
pascal_lex_warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    pascal_lex_warning_v(curloc, fmt, ap);
    va_end(ap);
}


void
pascal_lex_trace_print(void *, const char *fmt, ...)
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
            "%s: %.*s",
            src->name_and_line().c_str(),
            len,
            buffer
        );
        ++ep;
        len2 = strlen(ep) + 1;
        memmove(buffer, ep, len2);
    }
}


static symtab<int> reserved_words;


static int
reserved(const rcstring &s)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (reserved_words.empty())
    {
        struct table_t
        {
            const char *name;
            int value;
        };
        static const table_t table[] =
        {
            { "and",       AND       },
            { "array",     ARRAY     },
            { "begin",     BEGIN     },
            { "case",      CASE      },
            // { "code", CODE_tok }, // not always a keyword
            { "const",     CONST     },
            // { "data", DATA_tok }, // not always a keyword
            { "div",       DIV       },
            { "do",        DO        },
            { "downto",    DOWNTO    },
            { "else",      ELSE      },
            { "end",       END       },
            { "external",  EXTERNAL  },
            { "file",      FILE_tok  },
            { "for",       FOR       },
            { "forward",   FORWARD   },
            { "function",  FUNCTION  },
            { "goto",      GOTO      },
            { "if",        IF        },
            { "implementation", IMPLEMENTATION },
            { "in",        IN        },
            { "interface", INTERFACE },
            // { "intrinsic", INTRINSIC_tok }, // not always a keyword
            { "label",     LABEL     },
            { "mod",       MOD       },
            { "not",       NOT       },
            { "of",        OF        },
            { "or",        OR        },
            { "otherwise", OTHERWISE },
            { "packed",    PACKED    },
            { "procedure", PROCEDURE },
            { "program",   PROGRAM   },
            { "record",    RECORD    },
            { "repeat",    REPEAT    },
            { "scan",      SCAN      },
            { "segment",   SEGMENT   },
            { "separate",  SEPARATE  },
            { "set",       SET       },
            { "sizeof",    SIZEOF_tok },
            { "then",      THEN      },
            { "to",        TO        },
            { "type",      TYPE      },
            { "unit",      UNIT      },
            { "until",     UNTIL     },
            { "uses",      USES      },
            { "var",       VAR       },
            { "while",     WHILE     },
            { "with",      WITH      },
            { "write",     WRITE     },
            { "writeln",   WRITELN   },
        };
        for (const table_t *tp = table; tp < ENDOF(table); ++tp)
            reserved_words.assign(pascal_id(tp->name), tp->value);
    }
    int *p = reserved_words.query(pascal_id(s));
    DEBUG(2, "  %s=>%d", s.c_str(), (p?*p:0));
    return (p ? *p : 0);
}


void
pascal_lex_unit_keywords_enable(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    struct table_t
    {
        const char *name;
        int value;
    };
    static const table_t table[] =
    {
        { "code",      CODE_tok  },
        { "data",      DATA_tok  },
        { "intrinsic", INTRINSIC },
    };
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
        reserved_words.assign(pascal_id(tp->name), tp->value);
}


void
pascal_lex_unit_keywords_disable(void)
{
    reserved_words.remove(pascal_id("code"));
    reserved_words.remove(pascal_id("data"));
    reserved_words.remove(pascal_id("intrinsic"));
}


static inline bool
safe_isdigit(int c)
{
    return (c >= 0 && isdigit((unsigned char)c));
}


static inline bool
safe_isxdigit(int c)
{
    return (c >= 0 && isxdigit((unsigned char)c));
}


location
pascal_lex_location(void)
{
    return curloc;
}


static void
bad_comment(void)
{
    pascal_lex_warning("this is not an ISO 7185 conforming comment");
}


static char
escape_character(void)
{
    int c = lex_getc();
    switch (c)
    {
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
        {
            int n = 0;
            int ndig = 0;
            for (;;)
            {
                n = n * 8 + 'c' - '0';
                ++ndig;
                if (ndig >= 3)
                    break;
                c = lex_getc();
                switch (c)
                {
                case '0': case '1': case '2': case '3':
                case '4': case '5': case '6': case '7':
                    continue;

                default:
                    break;
                }
                break;
            }
        }
        break;

    case 'a':
        return '\a';

    case 'b':
        return '\b';

    case 'f':
        return '\f';

    case 'n':
        return '\n';

    case 'r':
        return '\r';

    case 't':
        return '\t';

    case 'v':
        return '\v';

    case '\'':
    case '\"':
    case '\\':
        return c;

    default:
        lex_ungetc(c);
        pascal_grammar_error("unknown escape");
        break;
    }
    return '\\';
}


int
pascal_grammar_lex(void)
{
    static bool c_string_warning;
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);

    //
    // This is where yacc expects us to put attributes of the tokens.
    //
    extern pascal_grammar_STYPE pascal_grammar_lval;

    //
    // By using the one string accumulator for all the parsing, it will
    // reach a steady state fairly quickly, and rarely need to allocate
    // more space.
    //
    static rcstring_accumulator buf;

    for (;;)
    {
        //
        // The line number incriment is deferred until after the token
        // has been given to the parser (yacc generated code) so that
        // the line number for error messages is still right.
        //
        assert(src);
        src->next_line_is_now();

        curloc = src->get_location();
        bool isa_doxygen_comment = false;
        buf.clear();
        int c = lex_getc();
        bool c_start_of_line = false;
        switch (c)
        {
        case EOF:
            DEBUG(1, "}");
            return 0;

        case '?':
            return QUESTION_MARK;

        case '-':
            c = lex_getc();
            if (c == '>')
            {
                pascal_grammar_error
                (
                    "this is Pascal, not C, "
                        "the \"->\" operator is spelled \"^.\""
                );
                DEBUG(1, "}");
                return ARROW;
            }
            lex_ungetc(c);
            DEBUG(1, "}");
            return MINUS;

        case '+':
            DEBUG(1, "}");
            return PLUS;

        case '*':
            DEBUG(1, "}");
            return MUL;

        case '/':
            DEBUG(1, "}");
            return SLASH;

        case '^':
            DEBUG(1, "}");
            return CARAT;

        case '&':
            c = lex_getc();
            if (c == '&')
            {
                static bool and_warning;
                if (!and_warning)
                {
                    and_warning = true;
                    pascal_grammar_error
                    (
                        "this is Pascal, not C, the \"&&\" operator is "
                            "written \"and\""
                    );
                }
                DEBUG(1, "}");
                return AND;
            }
            lex_ungetc(c);
            DEBUG(1, "}");
            return BITWISE_AND;

        case '@':
            DEBUG(1, "}");
            return AT;

        case '[':
            DEBUG(1, "}");
            return LB;

        case ']':
            DEBUG(1, "}");
            return RB;

        case ':':
            c = lex_getc();
            if (c == '=')
            {
                DEBUG(1, "}");
                return ASSIGN;
            }
            lex_ungetc(c);
            DEBUG(1, "}");
            return COLON;

        case '(':
            c = lex_getc();
            if (c != '*')
            {
                lex_ungetc(c);
                DEBUG(1, "}");
                return LP;
            }
            isa_doxygen_comment = (lex_peek() == '*');
            c_start_of_line = true;
            for (;;)
            {
                for (;;)
                {
                    c = lex_getc();
                    if (c == EOF)
                        break;
                    if (c == '}')
                        bad_comment();
                    if (c == '\n')
                    {
                        if (!buf.empty())
                            buf.push_back('\n');
                        c_start_of_line = true;
                    }
                    else
                    {
                        if
                        (
                            !c_start_of_line
                        ||
                            (c != '*' && !isspace((unsigned char)c))
                        )
                        {
                            buf.push_back((char)c);
                            c_start_of_line = false;
                        }
                    }
                    if (c == '*')
                        break;
                }
                if (c == EOF)
                    break;
                for (;;)
                {
                    c = lex_getc();
                    if (c == EOF)
                        break;
                    if (c == '\n')
                    {
                        if (!buf.empty())
                            buf.push_back('\n');
                        c_start_of_line = true;
                    }
                    else
                    {
                        if
                        (
                            !c_start_of_line
                        ||
                            (c != '*' && !isspace((unsigned char)c))
                        )
                        {
                            buf.push_back((char)c);
                            c_start_of_line = false;
                        }
                    }
                    if (c != '*')
                        break;
                }
                if (c == ')' || c == EOF)
                    break;
            }
            while
            (
                !buf.empty()
            &&
                (
                    buf.back() == ')'
                ||
                    buf.back() == '*'
                ||
                    isspace((unsigned char)(buf.back()))
                )
            )
                buf.pop_back();
            if (isa_doxygen_comment && !memchr(buf.get_data(), '@', buf.size()))
                buf.push_back("\n@note");
            pascal_grammar_comment(rcstring(buf.mkstr()));
            break;

        case '{':
            for (;;)
            {
                c = lex_getc();
                if (c == EOF)
                    break;
                if (c == '}')
                    break;
                if (buf.ends_with("*)"))
                    bad_comment();
                buf.push_back(c);
            }
            while (!buf.empty() && isspace((unsigned char)(buf.back())))
                buf.pop_back();
            pascal_grammar_comment(rcstring(buf.mkstr()));
            break;

        case '.':
            c = lex_getc();
            if (c == '.')
            {
                for (;;)
                {
                    c = lex_getc();
                    if (c != '.')
                        break;
                }
                lex_ungetc(c);
                DEBUG(1, "}");
                return ELIPSIS;
            }
            lex_ungetc(c);
            DEBUG(1, "}");
            return DOT;

        case ' ':
        case '\t':
        case '\f':
        case '\n':
            break;

        case ')':
            DEBUG(1, "}");
            return RP;

        case ',':
            DEBUG(1, "}");
            return COMMA;

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_':
            {
                for (;;)
                {
                    buf.push_back((char)c);
                    c = lex_getc();
                    switch (c)
                    {
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                    case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                    case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                    case 'y': case 'z':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                    case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                    case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                    case 'Y': case 'Z':
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case '_':
                        continue;

                    case EOF:
                        break;

                    default:
                        lex_ungetc(c);
                        break;
                    }
                    break;
                }
                rcstring name = buf.mkstr();
                int n = reserved(name);
                if (n)
                {
                    switch (n)
                    {
                    case UNIT:
                        pascal_lex_unit_keywords_enable();
                        break;

                    case INTERFACE:
                        pascal_lex_unit_keywords_disable();
                        src->snoop_begin();
                        break;

                    case IMPLEMENTATION:
                        {
                            rcstring text = src->snoop_end(name.size());
                            pascal_grammar_notify_interface_text(text);
                        }
                        break;
                    }
                    DEBUG(1, "}");
                    return n;
                }
                pascal_grammar_lval.lv_string = new rcstring(name);
                if (pascal_grammar_type_name(name))
                {
                    DEBUG(1, "}");
                    return TYPE_NAME;
                }
                DEBUG(1, "}");
                return NAME;
            }

        case '0':
            c = lex_getc();
            if (c != 'x' && c != 'X')
            {
                lex_ungetc(c);
                c = '0';
                goto normal_digit;
            }
            {
                // Parse a C hex constant.
                char begin = c;
                buf.push_back('$');
                for (;;)
                {
                    c = lex_getc();
                    if (!safe_isxdigit(c))
                        break;
                    buf.push_back(c);
                }
                if (c == 'L' || c == 'l')
                    buf.push_back('L');
                else
                    lex_ungetc(c);
                if (buf.empty())
                {
                    lex_ungetc(begin);
                    c = '0';
                    goto normal_digit;
                }
                pascal_lex_warning
                (
                    "this is Pascal, not C, that hexadecimal constant "
                        "should be written %s instead",
                    buf.mkstr().quote_c().c_str()
                );
                pascal_grammar_lval.lv_string = new rcstring(buf.mkstr());
                DEBUG(1, "}");
                if (buf.back() == 'L')
                    return LONG_INTEGER_CONSTANT;
                return INTEGER_CONSTANT;
            }

        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            normal_digit:
            for (;;)
            {
                buf.push_back(c);
                c = lex_getc();
                if (!safe_isdigit(c))
                    break;
            }
            switch (c)
            {
            case '.':
                // Watch out for 1..2 because this case is 3 tokens:
                // integer elipsis integer.
                if (lex_peek() == '.')
                    break;
                buf.push_back('.');
                for (;;)
                {
                    c = lex_getc();
                    if (!safe_isdigit(c))
                        break;
                    buf.push_back(c);
                }
                if (c != 'e' && c != 'E')
                {
                    lex_ungetc(c);
                    pascal_grammar_lval.lv_string = new rcstring(buf.mkstr());
                    DEBUG(1, "}");
                    return REAL_CONSTANT;
                }
                // fall through...

            case 'e':
            case 'E':
                buf.push_back(c);
                c = lex_getc();
                if (c == '+' || c == '-')
                {
                    buf.push_back(c);
                    c = lex_getc();
                }
                if (!safe_isdigit(c))
                {
                    pascal_grammar_error("malformed real exponent");
                    buf.push_back('0');
                }
                else
                {
                    for (;;)
                    {
                        buf.push_back(c);
                        c = lex_getc();
                        if (!safe_isdigit(c))
                            break;
                    }
                }
                lex_ungetc(c);
                pascal_grammar_lval.lv_string = new rcstring(buf.mkstr());
                DEBUG(1, "}");
                return REAL_CONSTANT;
            }
            if (c == '#')
            {
                // ISO 10206 integer constant with radix.
                rcstring s = buf.mkstr();
                buf.push_back('#');
                long radix = strtol(s.c_str(), 0, 10);
                if (radix < 2)
                {
                    pascal_lex_error
                    (
                        pascal_lex_location(),
                        "radix %ld not permitted, must be 2..36",
                        radix
                    );
                    radix = 2;
                }
                else if (radix > 36)
                {
                    pascal_lex_error
                    (
                        pascal_lex_location(),
                        "radix %ld not permitted, must be 2..36",
                        radix
                    );
                    radix = 36;
                }
                for (;;)
                {
                    c = lex_getc();
                    switch (c)
                    {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                        if (c - '0' >= radix)
                            break;
                        buf.push_back((char)c);
                        continue;

                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                    case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                    case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                    case 'y': case 'z':
                        if (c - 'a' + 10 >= radix)
                            break;
                        buf.push_back((char)c);
                        continue;

                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                    case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                    case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                    case 'Y': case 'Z':
                        if (c - 'A' + 10 >= radix)
                            break;
                        buf.push_back((char)c);
                        continue;

                    default:
                        break;
                    }
                    break;
                }
                if (buf.back() == '#')
                {
                    pascal_grammar_error("malformed radix constant");
                    buf.pop_back();
                }
            }
            if (c == 'L' || c == 'l')
                buf.push_back('L');
            else
                lex_ungetc(c);
            pascal_grammar_lval.lv_string = new rcstring(buf.mkstr());
            DEBUG(1, "}");
            if (buf.back() == 'L')
                return LONG_INTEGER_CONSTANT;
            return INTEGER_CONSTANT;

        case '$':
            // hex constant
            for (;;)
            {
                buf.push_back(c);
                c = lex_getc();
                if (!safe_isxdigit(c))
                    break;
            }
            if (buf.size() == 1)
            {
                lex_ungetc(c);
                DEBUG(1, "}");
                return JUNK;
            }
            if (c == 'L' || c == 'l')
                buf.push_back('L');
            else
                lex_ungetc(c);
            pascal_grammar_complain_about_hex_constant();
            pascal_grammar_lval.lv_string = new rcstring(buf.mkstr());
            DEBUG(1, "}");
            if (buf.back() == 'L')
                return LONG_INTEGER_CONSTANT;
            return INTEGER_CONSTANT;

        case '%':
            // binary constant
            for (;;)
            {
                buf.push_back(c);
                c = lex_getc();
                if (!safe_isxdigit(c))
                    break;
            }
            if (buf.size() == 1)
            {
                lex_ungetc(c);
                pascal_grammar_error
                (
                    "this is Pascal, not C, the modulo operator is "
                    "written \"mod\", not \"%%\""
                );
                DEBUG(1, "}");
                return MOD;
            }
            if (c == 'L' || c == 'l')
                buf.push_back('L');
            else
                lex_ungetc(c);
            pascal_grammar_complain_about_binary_constant();
            pascal_grammar_lval.lv_string = new rcstring(buf.mkstr());
            DEBUG(1, "}");
            if (buf.back() == 'L')
                return LONG_INTEGER_CONSTANT;
            return INTEGER_CONSTANT;

        case '"':
            if (!c_string_warning)
            {
                c_string_warning = true;
                pascal_grammar_error
                (
                    "this is Pascal, not C, you must use single quote strings"
                );
            }
            for (;;)
            {
                c = lex_getc();
                if (c == EOF || c == '\n')
                {
                    pascal_grammar_error("unterminated string constant");
                    lex_ungetc(c);
                    break;
                }
                if (c == '"')
                {
                    c = lex_getc();
                    if (c != '"')
                    {
                        lex_ungetc(c);
                        break;
                    }
                }
                if (pascal_grammar_string_escapes() && c == '\\')
                     c = escape_character();
                buf.push_back((char)c);
            }
            pascal_grammar_lval.lv_string = new rcstring(buf.mkstr());
            DEBUG(1, "}");
            return STRING_CONSTANT;

        case '\'':
            for (;;)
            {
                c = lex_getc();
                if (c == EOF || c == '\n')
                {
                    pascal_grammar_error("unterminated string constant");
                    lex_ungetc(c);
                    break;
                }
                if (c == '\'')
                {
                    c = lex_getc();
                    if (c != '\'')
                    {
                        lex_ungetc(c);
                        break;
                    }
                }
                if (pascal_grammar_string_escapes() && c == '\\')
                     c = escape_character();
                buf.push_back((char)c);
            }
            pascal_grammar_lval.lv_string = new rcstring(buf.mkstr());
            DEBUG(1, "}");
            return STRING_CONSTANT;

        case ';':
            DEBUG(1, "}");
            return SEMICOLON;

        case '>':
            c = lex_getc();
            if (c == '=')
            {
                DEBUG(1, "}");
                return GE;
            }
            lex_ungetc(c);
            DEBUG(1, "}");
            return GT;

        case '<':
            c = lex_getc();
            if (c == '=')
            {
                DEBUG(1, "}");
                return LE;
            }
            if (c == '>')
            {
                DEBUG(1, "}");
                return NE;
            }
            lex_ungetc(c);
            DEBUG(1, "}");
            return LT;

        case '=':
            c = lex_getc();
            if (c == '=')
            {
                static bool eq_warning;
                if (!eq_warning)
                {
                    eq_warning = true;
                    pascal_grammar_error
                    (
                        "this is Pascal, not C, the equality operator is "
                            "written \"=\""
                    );
                }
                DEBUG(1, "}");
                return EQ;
            }
            lex_ungetc(c);
            DEBUG(1, "}");
            return EQ;

        case '!':
            c = lex_getc();
            if (c == '=')
            {
                static bool ne_warning;
                if (!ne_warning)
                {
                    ne_warning = true;
                    pascal_grammar_error
                    (
                        "this is Pascal, not C, the inequality operator "
                            "is written <>"
                    );
                }
                DEBUG(1, "}");
                return NE;
            }
            else
            {
                static bool not_warning;
                if (!not_warning)
                {
                    not_warning = true;
                    pascal_grammar_error
                    (
                        "this is Pascal, not C, the logical not operator is "
                            "written \"not\""
                    );
                }
                lex_ungetc(c);
            }
            DEBUG(1, "}");
            return NOT;

        case '|':
            c = lex_getc();
            if (c == '|')
            {
                static bool or_warning;
                if (!or_warning)
                {
                    or_warning = true;
                    pascal_grammar_error
                    (
                        "this is Pascal, not C, the logical or operator is "
                            "written \"or\""
                    );
                }
                DEBUG(1, "}");
                return OR;
            }
            lex_ungetc(c);
            DEBUG(1, "}");
            return BITWISE_OR;

        case '~':
            DEBUG(1, "}");
            return BITWISE_NOT;

        default:
            // Unknown symbols cause a syntax error.
            DEBUG(1, "}");
            return JUNK;
        }
    }
}


int
pascal_lex_number_of_lines(void)
{
    return total_lines;
}


static rcstring
to_str(int n)
{
    return rcstring::printf("%d", n);
}


void
pascal_lex_include_unit(const rcstring &code_file_name, const rcstring &name,
    int code_seg, int data_seg, int segkind, const rcstring &text)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    rcstring filename =
        code_file_name + "/seg=" + to_str(code_seg) + "/" + name;

    // The white space either side of the "text" string is essential
    // for token separation, just in case "text" is in one of the
    // pathological corner cases.
    rcstring body = "unit " + name + " " + to_str(code_seg);
    if (data_seg >= 0)
        body += " " + to_str(data_seg);
    body += " " + to_str((int)segkind);
    body += " begin\n";
    body += text;
    body += "\nunit end;\n";

    source *sp = new source(src);
    sp->open(input_string::create(body, filename));
    src = sp;
    DEBUG(1, "}");
}


int
pascal_lex_integer_constant_to_int(const rcstring &text)
{
    if (text[0] == '$')
        return strtol(text.c_str() + 1, 0, 16);
    if (text[0] == '%')
        return strtol(text.c_str() + 1, 0, 2);

    char *ep = 0;
    long n = strtol(text.c_str(), &ep, 10);
    if (*ep != '#')
        return n;
    assert(n >= 2);
    assert(n <= 26);
    return strtol(ep + 1, 0, n);
}


// vim: set ts=8 sw=4 et :
