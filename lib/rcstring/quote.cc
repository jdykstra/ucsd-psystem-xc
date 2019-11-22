//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#include <lib/ac/ctype.h>

#include <lib/rcstring/accumulator.h>


rcstring
rcstring::quote_shell()
    const
{
    //
    // Work out if the string needs quoting.
    //
    // The empty string is not quoted, even though it could be argued
    // that it needs to be.  It has proven more useful in the present
    // form, because it allows empty filename lists to pass through
    // and remain empty.
    //
    bool needs_quoting = false;
    char mode = 0;
    for (const char *cp = c_str(); *cp; ++cp)
    {
        if (isspace((unsigned char)*cp))
        {
            needs_quoting = true;
            break;
        }
        switch (*cp)
        {
        default:
            continue;

        case '!':
            // special for bash and csh
            if (!mode)
                mode = '\'';
            needs_quoting = true;
            break;

        case '\'':
            if (!mode)
                mode = '"';
            needs_quoting = true;
            break;

        case '"': case '#': case '$': case '&':
        case '(': case ')': case '*': case ':': case ';': case '<':
        case '=': case '>': case '?': case '[': case '\\': case ']':
        case '^': case '`': case '{': case '|': case '}': case '~':
            needs_quoting = true;
            break;
        }
        break;
    }

    //
    // If it doesn't need quoting, return immediately.
    //
    if (!needs_quoting)
        return *this;

    //
    // If we have a choice, use single quote mode,
    // it's shorter and easier to read.
    //
    if (!mode)
        mode ='\'';

    //
    // Form the quoted string, using the minimum number of escapes.
    //
    // The gotcha here is the backquote: the `blah` substitution is
    // still active within double quotes.  And so are a few others.
    //
    // Also, there are some difficulties: the single quote can't be
    // quoted within single quotes, and the exclamation mark can't
    // be quoted by anything *except* single quotes.  Sheesh.
    //
    // Also, the rules change depending on which style of quoting
    // is in force at the time.
    //
    static rcstring_accumulator buffer;
    buffer.clear();
    buffer.push_back(mode);
    for (const char *cp = c_str(); *cp; ++cp)
    {
        if (mode == '\'')
        {
            // within single quotes
            if (*cp == '\'')
            {
                //
                // You can't quote a single quote within
                // single quotes.  Need to change to
                // double quote mode.
                //
                buffer.push_back("'\"'", 3);
                mode = '"';
            }
            else
                buffer.push_back(*cp);
        }
        else
        {
            // within double quotes
            switch (*cp)
            {
            case '!':
                //
                // You can't quote an exclamation mark within double
                // quotes.  Need to change to single quote mode.
                //
                buffer.push_back("\"'!", 3);
                mode = '\'';
                break;

            case '\n':
            case '"':
            case '\\':
            case '`': // stop command substitutions
            case '$': // stop variable substitutions
                    buffer.push_back('\\');
                    // fall through...

            default:
                buffer.push_back(*cp);
                break;
            }
        }
    }
    buffer.push_back(mode);
    return buffer.mkstr();
}


// vim: set ts=8 sw=4 et :
