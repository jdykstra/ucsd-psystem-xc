//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/pascal_id.h>
#include <lib/rcstring/accumulator.h>
#include <lib/translator.h>


rcstring
pascal_id(const rcstring &name)
{
    return translator::pascal_id(name);
}


rcstring
translator::pascal_id(const rcstring &name)
{
    if (name.empty())
        return "_empty";
    static rcstring_accumulator ac;
    ac.clear();
    const char *cp = name.c_str();
    assert(max_id_len >= 8);
    size_t maximum_size = max_id_len;
    while (ac.size() < maximum_size)
    {
        unsigned char c = *cp++;
        switch (c)
        {
        case '\0':
            return ac.mkstr();

        case '_':
            if (feature_underscore_significant)
                ac.push_back(c);
            break;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            c = tolower(c);
            ac.push_back(c);
            break;

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        default:
            ac.push_back(c);
            break;

        case ':':
            // This hack is used for implicit references, to build the
            // second variable of the "same" name.
            maximum_size = ac.size() + 1 + max_id_len;
            ac.push_back(c);
            break;
        }
    }
    return ac.mkstr();
}


// vim: set ts=8 sw=4 et :
