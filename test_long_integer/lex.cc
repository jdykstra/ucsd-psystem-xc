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

#include <lib/ac/stdlib.h>
#include <libexplain/fclose.h>
#include <libexplain/fopen.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>

#include <lib/cardinal.h>
#include <lib/long_integer.h>
#include <lib/rcstring/accumulator.h>

#include <test_long_integer/lex.h>
#include <test_long_integer/grammar.yacc.h> // must be last


static FILE *fp;
static rcstring fn;
static int line_number;
static int number_of_errors;
static bool crank_line_number;


void
lex_open(const char *filename)
{
    fn = filename;
    fp = explain_fopen_or_die(filename, "r");
    line_number = 1;
    crank_line_number = false;
    number_of_errors = 0;
}


void
lex_close(void)
{
    if (number_of_errors > 0)
    {
        rcstring msg =
            rcstring::printf
            (
                "%s: found %s fatal error%s",
                fn.c_str(),
                cardinal(number_of_errors).c_str(),
                (number_of_errors == 1 ? "" : "s")
            );
        explain_output_message(msg.c_str());
        explain_output_exit_failure();
    }
    explain_fclose_or_die(fp);
    fn.clear();
}


static bool
next_is(char expect)
{
    int c = explain_getc_or_die(fp);
    if (c == EOF)
        return false;
    if (c == expect)
        return true;
    ungetc(c, fp);
    return false;
}


int
grammar_lex(void)
{
    for (;;)
    {
        if (crank_line_number)
        {
            ++line_number;
            crank_line_number = false;
        }

        int c = explain_getc_or_die(fp);
        switch(c)
        {
        case EOF:
            return 0;

        case '\n':
            crank_line_number = true;
            return EOLN;

        case ' ':
        case '\b':
        case '\f':
        case '\r':
        case '\t':
        case '\v':
            break;

        case '!':
            if (next_is('='))
                return NE;
            return NOT;

        case '#':
            // comment
            for (;;)
            {
                c = explain_getc_or_die(fp);
                if (c == EOF || c == '\n')
                    break;
            }
            return EOLN;

        case '%':
            return PERCENT;

        case '(':
            return LP;

        case ')':
            return RP;

        case '*':
            return STAR;

        case '+':
            return PLUS;

        case '-':
            return MINUS;

        case '/':
            return SLASH;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                rcstring_accumulator ac;
                for (;;)
                {
                    ac.push_back((char)c);
                    c = explain_getc_or_die(fp);
                    switch (c)
                    {
                    case EOF:
                        break;

                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                        continue;

                    default:
                        ungetc(c, fp);
                        break;
                    }
                    break;
                }
                grammar_lval.lv_number = new long_integer(ac.mkstr());
            }
            return NUMBER;

        case '<':
            if (next_is('='))
                return LE;
            return LT;

        case '=':
            if (next_is('='))
                return EQ;
            return ASSIGN;

        case '>':
            if (next_is('='))
                return GE;
            return GT;

        case '@':
            return AT;

        default:
            return JUNK;
        }
    }
}


void
grammar_error(const char *text)
{
    rcstring msg =
        rcstring::printf("%s: %d: %s", fn.c_str(), line_number, text);
    explain_output_message(msg.c_str());
    ++number_of_errors;
}


// vim: set ts=8 sw=4 et :
