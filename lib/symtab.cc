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


symtab_t::symtab_t(int suggested_size) :
    reap(0),
    hash_table(0),
    hash_modulus(0),
    hash_mask(0),
    hash_load(0)
{
    hash_modulus = 1 << 5; // MUST be a power of 2
    while ((long)hash_modulus < (long)suggested_size)
        hash_modulus <<= 1;
    hash_mask = hash_modulus - 1;
    hash_table = new row_t * [hash_modulus];
    for (rcstring::hash_t j = 0; j < hash_modulus; ++j)
        hash_table[j] = 0;
}


symtab_t::~symtab_t()
{
    clear();

    reap = 0;
    delete [] hash_table;
    hash_table = 0;
    hash_modulus = 0;
    hash_mask = 0;
}


// vim: set ts=8 sw=4 et :
