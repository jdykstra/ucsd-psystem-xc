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

#include <lib/ac/string.h>

#include <lib/fstrcmp.h>
#include <lib/symtab.h>


rcstring
symtab_t::query_fuzzy(const rcstring &key)
    const
{
    double best = 0.6;
    return query_fuzzy(key, best);
}


rcstring
symtab_t::query_fuzzy(const rcstring &key, double &best_weight)
    const
{
    rcstring best_name;
    for (rcstring::hash_t index = 0; index < hash_modulus; ++index)
    {
        for (row_t *p = hash_table[index]; p; p = p->overflow)
        {
            // ignore special symbols
            if (strchr(p->key.c_str(), ':'))
                continue;

            double weight = fstrcmp(key.c_str(), p->key.c_str());
            if (weight > best_weight)
                best_name = p->key;
        }
    }
    return best_name;
}


// vim: set ts=8 sw=4 et :
