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

#include <lib/ac/string.h>
#include <lib/ac/fnmatch.h>

#include <lib/gmatch.h>
#include <lib/rcstring.h>


int
gmatch(const char *formal, const char *actual)
{
#ifdef HAVE_FNMATCH
    switch (fnmatch(formal, actual, 0))
    {
    case 0:
        return 1;

    case FNM_NOMATCH:
        return 0;

    default:
        return -1;
    }
#else
    const char      *cp;
    int             result;

    while (*formal)
    {
        switch (*formal)
        {
        default:
            if (*actual++ != *formal++)
            {
                result = 0;
                goto ret;
            }
            break;

        case '?':
            if (!*actual++)
            {
                result = 0;
                goto ret;
            }
            ++formal;
            break;

        case '*':
            for (;;)
            {
                ++formal;
                switch (*formal)
                {
                case 0:
                    result = 1;
                    goto ret;

                case '*':
                    continue;

                case '?':
                    if (!*actual++)
                    {
                        result = 0;
                        goto ret;
                    }
                    continue;

                default:
                    break;
                }
                break;
            }
            cp = actual + strlen(actual);
            for (;;)
            {
                result = gmatch(formal, cp);
                if (result)
                {
                    result = 1;
                    break;
                }
                --cp;
                if (cp < actual)
                {
                    result = 0;
                    break;
                }
            }
            goto ret;

        case '[':
            ++formal;
            if (*formal == '^')
            {
                ++formal;
                for (;;)
                {
                    if (!*formal)
                    {
                        no_close:
                        result = -1;
                        goto ret;
                    }

                    //
                    // note: this allows leading close
                    // square bracket elegantly
                    //
                    if
                    (
                        formal[1] == '-'
                    &&
                        formal[2]
                    &&
                        formal[2] != ']'
                    &&
                        formal[3]
                    )
                    {
                        char            c1;
                        char            c2;

                        c1 = formal[0];
                        c2 = formal[2];
                        formal += 3;
                        if
                        (
                            c1 <= c2
                        ?
                            (c1 <= *actual && *actual <= c2)
                        :
                            (c2 <= *actual && *actual <= c1)
                        )
                        {
                            result = 0;
                            goto ret;
                        }
                    }
                    else if (*actual == *formal++)
                    {
                        result = 0;
                        goto ret;
                    }
                    if (*formal == ']')
                        break;
                }
                ++formal;
            }
            else
            {
                for (;;)
                {
                    if (!*formal)
                        goto no_close;

                    //
                    // note: this allows leading close
                    // square bracket elegantly
                    //
                    if
                    (
                        formal[1] == '-'
                    &&
                        formal[2]
                    &&
                        formal[2] != ']'
                    &&
                        formal[3]
                    )
                    {
                        char            c1;
                        char            c2;

                        c1 = formal[0];
                        c2 = formal[2];
                        formal += 3;
                        if
                        (
                            c1 <= c2
                        ?
                            (c1 <= *actual && *actual <= c2)
                        :
                            (c2 <= *actual && *actual <= c1)
                        )
                            break;
                    }
                    else if (*actual == *formal++)
                        break;
                    if (*formal == ']')
                    {
                        result = 0;
                        goto ret;
                    }
                }
                for (;;)
                {
                    if (!*formal)
                        goto no_close;
                    if (*formal++ == ']')
                        break;
                }
            }
            ++actual;
            break;
        }
    }
    result = (*actual == 0);

    //
    // here for all exits
    //
    ret:
    return result;
#endif
}


int
gmatch(const rcstring &needle, const rcstring &haystack)
{
    return gmatch(needle.c_str(), haystack.c_str());
}


// vim: set ts=8 sw=4 et :
