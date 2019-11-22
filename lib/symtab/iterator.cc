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

#include <lib/ac/assert.h>

#include <lib/symtab/iterator.h>
#include <lib/symtab.h>


symtab_iterator_t::~symtab_iterator_t()
{
    stp = 0;
    idx = 0;
    rp = 0;
}


symtab_iterator_t::symtab_iterator_t() :
    stp(0),
    idx(0),
    rp(0)
{
}


symtab_iterator_t::symtab_iterator_t(const symtab_t *a_stp, bool begin) :
    stp(a_stp),
    idx(0),
    rp(0)
{
    if (stp)
    {
        if (begin)
        {
            //
            // The begin marker has to find the first non-empty bucket.
            // It could end up being the same as the end marker.
            //
            next();
        }
        else
        {
            //
            // The end marker has the index beyond the end of the symbol table
            //
            idx = stp->hash_modulus;
        }
    }
    assert(valid());
}


symtab_iterator_t::symtab_iterator_t(const symtab_iterator_t &arg) :
    stp(arg.stp),
    idx(arg.idx),
    rp(arg.rp)
{
    assert(valid());
}


symtab_iterator_t &
symtab_iterator_t::operator=(const symtab_iterator_t &arg)
{
    if (this != &arg)
    {
        stp = arg.stp;
        idx = arg.idx;
        rp = arg.rp;
    }
    assert(valid());
    return *this;
}


void *
symtab_iterator_t::get()
    const
{
    assert(valid());
    assert(rp != 0 && stp != 0);
    if (!rp)
        return 0;
    return ((symtab_t::row_t *)rp)->data;
}


void
symtab_iterator_t::next()
{
    if (stp)
    {
        if (rp)
            rp = ((symtab_t::row_t *)rp)->overflow;
        if (!rp)
        {
            while (idx < stp->hash_modulus)
            {
                rp = stp->hash_table[idx++];
                if (rp)
                    break;
            }
        }
    }
    assert(valid());
}


bool
symtab_iterator_t::equal(const symtab_iterator_t &rhs)
    const
{
    assert(valid());
    return (rp == rhs.rp);
}


bool
symtab_iterator_t::valid()
    const
{
    if (!stp)
        return (idx == 0 && rp == 0);
    return (idx <= stp->hash_modulus);
}


// vim: set ts=8 sw=4 et :
