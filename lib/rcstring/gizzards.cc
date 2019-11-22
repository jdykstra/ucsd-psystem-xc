//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <lib/ac/assert.h>
#include <lib/ac/string.h>
#include <lib/rcstring/gizzards.h>

//
// maximum conversion width for numbers
//
#define MAX_WIDTH 509

static rcstring_gizzards **hash_table;
static rcstring_gizzards::hash_t hash_modulus;
static rcstring_gizzards::hash_t hash_mask;
static rcstring_gizzards::hash_t hash_load;

#define MAX_HASH_LEN 20


//
// NAME
//      hash_generate - hash string to number
//
// SYNOPSIS
//      rcstring_gizzards::hash_t hash_generate(char *s, size_t n);
//
// DESCRIPTION
//      The hash_generate function is used to make a number from a string.
//
// RETURNS
//      rcstring_gizzards::hash_t - the magic number
//
// CAVEAT
//      Only the last MAX_HASH_LEN characters are used.  It is important
//      that rcstring_gizzards::hash_t be unsigned (int or long).
//

static rcstring_gizzards::hash_t
hash_generate(const char *s, size_t n)
{
    if (n > MAX_HASH_LEN)
    {
        s += n - MAX_HASH_LEN;
        n = MAX_HASH_LEN;
    }
    rcstring_gizzards::hash_t retval = 0;
    while (n > 0)
    {
        retval = (retval + (retval << 1)) ^ *s++;
        --n;
    }
    return retval;
}


static void
rcstring_gizzards_initialize(void)
{
    hash_modulus = 1 << 8;      // MUST be a power of 2
    hash_mask = hash_modulus - 1;
    hash_load = 0;
    hash_table = new rcstring_gizzards * [hash_modulus];
    for (rcstring_gizzards::hash_t j = 0; j < hash_modulus; ++j)
        hash_table[j] = 0;
}


//
// NAME
//      split - reduce table loading
//
// SYNOPSIS
//      void split(void);
//
// DESCRIPTION
//      The split function is used to reduce the load factor on the hash table.
//
// RETURNS
//      void
//
// CAVEAT
//      A load factor of about 80% is suggested.
//

static void
split(void)
{
    //
    // double the modulus
    //
    // This is subtle.  If we only increase the modulus by one, the
    // load always hovers around 80%, so we have to do a split for
    // every insert.  I.e. the malloc burden os O(n) for the lifetime of
    // the program.  BUT if we double the modulus, the length of time
    // until the next split also doubles, making the probablity of a
    // split halve, and sigma(2**-n)=1, so the malloc burden becomes O(1)
    // for the lifetime of the program.
    //
    rcstring_gizzards::hash_t new_hash_modulus = hash_modulus * 2;
    rcstring_gizzards **new_hash_table =
        new rcstring_gizzards * [new_hash_modulus];
    rcstring_gizzards::hash_t new_hash_mask = new_hash_modulus - 1;

    //
    // now redistribute the list elements
    //
    for (rcstring_gizzards::hash_t idx = 0; idx < hash_modulus; ++idx)
    {
        new_hash_table[idx] = 0;
        new_hash_table[idx + hash_modulus] = 0;
        rcstring_gizzards *p = hash_table[idx];
        while (p)
        {
            rcstring_gizzards *p2 = p;
            p = p->rcstring_gizzards_next;
            assert((p2->rcstring_gizzards_hash & hash_mask) == idx);
            rcstring_gizzards::hash_t new_idx =
                p2->rcstring_gizzards_hash & new_hash_mask;
            p2->rcstring_gizzards_next = new_hash_table[new_idx];
            new_hash_table[new_idx] = p2;
        }
    }
    delete [] hash_table;
    hash_table = new_hash_table;
    hash_modulus = new_hash_modulus;
    hash_mask = new_hash_mask;
}


rcstring_gizzards *
rcstring_gizzards_from_c(const char *s)
{
    return rcstring_gizzards_n_from_c(s, (s ? strlen(s) : 0));
}


rcstring_gizzards *
rcstring_gizzards_n_from_c(const unsigned char *s, size_t length)
{
    return rcstring_gizzards_n_from_c((const char *)s, length);
}


rcstring_gizzards *
rcstring_gizzards_n_from_c(const char *s, size_t length)
{
    if (!hash_table)
        rcstring_gizzards_initialize();
    if (!s)
        length = 0;
    rcstring_gizzards::hash_t hash = hash_generate(s, length);
    rcstring_gizzards::hash_t idx = hash & hash_mask;
    for
    (
        rcstring_gizzards *p = hash_table[idx];
        p;
        p = p->rcstring_gizzards_next
    )
    {
        if
        (
            p->rcstring_gizzards_hash == hash
        &&
            p->rcstring_gizzards_length == length
        &&
            !memcmp(p->rcstring_gizzards_text, s, length)
        )
        {
            p->rcstring_gizzards_references++;
            return p;
        }
    }

    //
    // Is there an elegant way to do this in C++?
    //
    // What we are doing is allocating enough space for the gizzards
    // structure and the, and tacking the text onto the end.  This saves
    // us a pointer dereference every time we access the text, which can
    // be a significant savings in some cases.
    //
    // In C I used malloc() but calling operator new directly feels
    // kludgy.  Of course using an array new char [] for this would
    // stink.
    //
    rcstring_gizzards *np =
        (rcstring_gizzards *)operator new(sizeof(rcstring_gizzards) + length);

    np->rcstring_gizzards_hash = hash;
    np->rcstring_gizzards_length = length;
    np->rcstring_gizzards_references = 1;
    np->rcstring_gizzards_next = hash_table[idx];
    hash_table[idx] = np;
    memcpy(np->rcstring_gizzards_text, s, length);
    np->rcstring_gizzards_text[length] = 0;

    hash_load++;
    if (hash_load * 10 > hash_modulus * 8)
        split();
    return np;
}


rcstring_gizzards *
rcstring_gizzards_copy(rcstring_gizzards *s)
{
    s->rcstring_gizzards_references++;
    return s;
}


void
rcstring_gizzards_free(rcstring_gizzards *s)
{
    if (!s)
        return;
    if (s->rcstring_gizzards_references > 1)
    {
        s->rcstring_gizzards_references--;
        return;
    }

    //
    // find the hash bucket it was in,
    // and remove it
    //
    rcstring_gizzards::hash_t idx = s->rcstring_gizzards_hash & hash_mask;
    for
    (
        rcstring_gizzards **spp = &hash_table[idx];
        *spp;
        spp = &(*spp)->rcstring_gizzards_next
    )
    {
        if (*spp == s)
        {
            *spp = s->rcstring_gizzards_next;
            // See above comment about elegance.
            operator delete((void *)s);
            --hash_load;
            return;
        }
    }

    //
    // Should never reach here!
    // This is an attempt to free a nonexistant string.
    //
    assert(0);
}


bool
rcstring_gizzards::validate()
    const
{
    if (rcstring_gizzards_references == 0)
        return 0;
    rcstring_gizzards::hash_t idx = rcstring_gizzards_hash & hash_mask;
    for
    (
        rcstring_gizzards **spp = &hash_table[idx];
        *spp;
        spp = &(*spp)->rcstring_gizzards_next
    )
    {
        if (*spp == this)
            return true;
    }
    return false;
}


// vim: set ts=8 sw=4 et :
