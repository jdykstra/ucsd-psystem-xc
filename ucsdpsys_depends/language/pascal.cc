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
#include <libexplain/output.h>

#include <lib/rcstring/accumulator.h>

#include <ucsdpsys_depends/language/pascal.h>


language_pascal::~language_pascal()
{
}


language_pascal::language_pascal() :
    language()
{
}


language::pointer
language_pascal::create(void)
{
    return pointer(new language_pascal());
}


void
language_pascal::comment(const rcstring &arg)
{
    if (arg.empty() || arg[0] != '$' || arg[1] != 'I' || arg[2] != ' ')
        return;
    rcstring filename = arg.field(' ', 1);
    if (lex_open_include(filename))
        return;
    if (!filename.ends_with(".text") && lex_open_include(filename + ".text"))
        return;
    filename = filename.downcase();
    if (lex_open_include(filename))
        return;
    if (!filename.ends_with(".text"))
        lex_open_include(filename + ".text");
}


bool
language_pascal::next_token(void)
{
    for (;;)
    {
        static rcstring_accumulator ac;
        location token_start = get_lex_location();
        ac.clear();
        int c = lex_getc();
        switch (c)
        {
        case EOF:
            return false;

        case ' ':
        case '\t':
        case '\f':
        case '\r':
        case '\n':
            break;

        case '\'':
            for (;;)
            {
                c = lex_getc();
                if (c == '\n' || c == EOF)
                {
                    explain_output_error_and_die
                    (
                        "%s: unterminated string",
                        token_start.representation().c_str()
                    );
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
                ac.push_back((char)c);
            }
            tokval = ac.mkstr();
            return true;

        case '{':
            for (;;)
            {
                c = lex_getc();
                if (c == EOF)
                {
                    explain_output_error_and_die
                    (
                        "%s: unterminated { comment }",
                        token_start.representation().c_str()
                    );
                }
                if (c == '}')
                    break;
                ac.push_back((char)c);
            }
            tokval = ac.mkstr();
            comment(tokval);
            break;

        case '(':
            c = lex_getc();
            if (c != '*')
            {
                lex_ungetc(c);
                c = '(';
                goto normal;
            }
            for (;;)
            {
                for (;;)
                {
                    c = lex_getc();
                    if (c == EOF)
                    {
                        explain_output_error_and_die
                        (
                            "%s: unterminated (* comment *)",
                            token_start.representation().c_str()
                        );
                    }
                    ac.push_back((char)c);
                    if (c == '*')
                        break;
                }
                for (;;)
                {
                    c = lex_getc();
                    if (c == EOF)
                    {
                        explain_output_error_and_die
                        (
                            "%s: unterminated (* comment *)",
                            token_start.representation().c_str()
                        );
                    }
                    ac.push_back((char)c);
                    if (c != '*')
                        break;
                }
                if (c == ')')
                {
                    ac.pop_back();
                    ac.pop_back();
                    break;
                }
            }
            tokval = ac.mkstr();
            comment(tokval);
            break;

        default:
            normal:
            ac.push_back((char)c);
            tokval = ac.mkstr();
            return true;
        }
    }
}


void
language_pascal::depends(const rcstring &infile)
{
    //
    // Read the file, looking for include files.
    //
    lex_open(infile);
    while (next_token())
        ;
}


// vim: set ts=8 sw=4 et :
