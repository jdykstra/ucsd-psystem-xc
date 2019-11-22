//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2012 Peter Miller
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

#include <lib/symtab.h>


bool
symtab_t::valid()
    const
{
    if (hash_table == 0)
        return false;
    if (hash_modulus == 0)
        return false;
    // the hash_modulus is required to be a power of two
    if ((hash_modulus & (hash_modulus - 1)) != 0)
        return false;
    if (hash_mask + 1 != hash_modulus)
        return false;
    if (hash_load >= hash_modulus)
        return false;
    rcstring::hash_t count = 0;
    for (rcstring::hash_t j = 0; j < hash_modulus; ++j)
    {
        for (row_t *rp = hash_table[j]; rp; rp = rp->overflow)
        {
            if (rp->data == 0)
                return false;
            if (!rp->key.valid())
                return false;
            ++count;
        }
    }
    return (count == hash_load);
}


// vim: set ts=8 sw=4 et :
