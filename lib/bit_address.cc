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

#include <lib/bit_address.h>


bit_address::~bit_address()
{
}


bit_address::bit_address() :
    word_addr(0),
    bitnum(0)
{
    assert(valid());
}


bit_address::bit_address(unsigned a_word_addr) :
    word_addr(a_word_addr),
    bitnum(0)
{
    assert(valid());
}


bit_address::bit_address(unsigned a_word_addr, unsigned a_bitnum) :
    word_addr(a_word_addr),
    bitnum(a_bitnum)
{
    assert(valid());
}


bit_address::bit_address(const bit_address &arg) :
    word_addr(arg.word_addr),
    bitnum(arg.bitnum)
{
    assert(valid());
}


bit_address &
bit_address::operator=(const bit_address &arg)
{
    if (this != &arg)
    {
        word_addr = arg.word_addr;
        bitnum = arg.bitnum;
        assert(valid());
    }
    return *this;
}


bool
bit_address::valid()
    const
{
    if (word_addr >= (1u << 16))
        return false;
    return (bitnum <= 16);
}


bool
bit_address::have_space_for(unsigned nbits)
    const
{
    assert(nbits > 0);
    assert(!(nbits & 15) || nbits <= 16);
    if (bitnum == 0)
        return true;
    if (nbits > 16)
        return false;
    return (bitnum + nbits <= 16);
}


void
bit_address::advance_to_word_boundary()
{
    if (bitnum == 0)
        return;
    ++word_addr;
    bitnum = 0;
}


void
bit_address::advance_by_bits(unsigned nbits)
{
    assert(valid());
    assert(nbits > 0);
    assert(!(nbits & 15) || nbits <= 16);
    if (!have_space_for(nbits))
        advance_to_word_boundary();
    bitnum += nbits;
    if (bitnum >= 16)
    {
        word_addr += bitnum >> 4;
        bitnum &= 15;
        assert(bitnum == 0);
    }
    assert(valid());
}


void
bit_address::advance_by_words(unsigned nwords)
{
    assert(valid());
    advance_to_word_boundary();
    word_addr += nwords;
    assert(valid());
}


unsigned
bit_address::get_size_in_words()
    const
{
    assert(valid());
    return (word_addr + (bitnum != 0));
};


bool
bit_address::operator==(const bit_address &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    return (word_addr == rhs.word_addr && bitnum == rhs.bitnum);
}


bool
bit_address::operator<(const bit_address &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    if (word_addr == rhs.word_addr)
        return (bitnum < rhs.bitnum);
    return (word_addr < rhs.word_addr);
}


bool
bit_address::operator>(const bit_address &rhs)
    const
{
    assert(valid());
    assert(rhs.valid());
    if (word_addr == rhs.word_addr)
        return (bitnum > rhs.bitnum);
    return (word_addr > rhs.word_addr);
}


rcstring
bit_address::representation(void)
    const
{
    if (bitnum)
    {
        return rcstring::printf("%u.%u", word_addr, bitnum);
    }
    return rcstring::printf("%u", word_addr);
}


// vim: set ts=8 sw=4 et :
