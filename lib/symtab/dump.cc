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

#include <lib/ac/stdio.h>
#include <lib/symtab.h>


void
symtab_t::dump(const char *caption)
    const
{
    printf("symbol table %s = {\n", caption);
    for (rcstring::hash_t j = 0; j < hash_modulus; ++j)
    {
        for (row_t *p = hash_table[j]; p; p = p->overflow)
        {
            printf("{ key = \"%s\", data = %p },\n", p->key.c_str(), p->data);
        }
    }
    printf("}\n");
}


// vim: set ts=8 sw=4 et :
