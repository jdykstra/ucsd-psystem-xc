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

#ifndef LIB_BIT_ADDRESS_H
#define LIB_BIT_ADDRESS_H

#include <lib/rcstring.h>

/**
  * The bit_address class is used to represent a combination of word
  * address and bit number.
  */
class bit_address
{
public:
    /**
      * The destructor.
      */
    virtual ~bit_address();

    /**
      * The default constructor.
      */
    bit_address();

    /**
      * The constructor.
      */
    bit_address(unsigned word_addr, unsigned bitnum);

    /**
      * The constructor.
      */
    bit_address(unsigned word_addr);

    /**
      * The copy constructor.
      */
    bit_address(const bit_address &);

    /**
      * The assignment operator.
      */
    bit_address &operator=(const bit_address &);

    /**
      * The valid method is used to check that this bit_address is
      * value.  This is used for debugging.
      */
    bool valid() const;

    /**
      * The get_word_addr method is used to obtain the word offset of
      * the bit address.
      */
    unsigned get_word_addr() const { return word_addr; }

    /**
      * The have_space_for method is used to determine whether or not
      * there is space for the given number of bits without advancing to
      * the next 16-bit word boundary.
      */
    bool have_space_for(unsigned bit_width) const;

    /**
      * The advance_to_word_boundary method is used to move this
      * bit_address to the next bitnum==0 boundary.  Nothing happens if
      * we are already at such a point.
      */
    void advance_to_word_boundary();

    /**
      * The advance_by_bits method is used to move this bit addressed
      * pointer forward by the given number of bits.
      *
      * @param bit_width
      *     The number of bits to advance.
      */
    void advance_by_bits(unsigned bit_width);

    /**
      * The advance_by_words method is used to move this bit addressed
      * pointer forward by the given number of words.  It will be
      * rounded up to the nearest word boundary first, if necessary.
      *
      * @param word_width
      *     The number of words to advance.
      */
    void advance_by_words(unsigned word_width);

    /**
      * The empty method is used to determine whether or not this bit
      * address is zero or not.
      */
    bool empty() const { return (word_addr == 0 && bitnum == 0); }

    /**
      * The get_size_in_words method is used to obtain the number of
      * whole words from the origin (0, 0, 0) to the present location,
      * rounded up.
      */
    unsigned get_size_in_words() const;

    bool on_word_boundary() const { return (bitnum == 0); }

    unsigned get_bit_number() const { return bitnum; }

    bool operator==(const bit_address &rhs) const;
    bool operator!=(const bit_address &rhs) const { return !(*this == rhs); }
    bool operator<(const bit_address &rhs) const;
    bool operator<=(const bit_address &rhs) const { return !(*this > rhs); }
    bool operator>(const bit_address &rhs) const;
    bool operator>=(const bit_address &rhs) const { return !(*this < rhs); }

    /**
      * The representation nethod i sused to obtain a human-readable
      * string equvalent of a bit_address.
      */
    rcstring representation(void) const;

private:
    /**
      * The word_addr instance variable is used to remember the 16-bit
      * word offset (within the local or global scope) of the value.
      * This is twice the byte address.  It is remembered this way
      * because it most closely resembles the p-code opcodes generated
      * from it.
      */
    unsigned word_addr;

    /**
      * The bitnum instance variable is used to remember the bit offset
      * (from 0 to 15) within the word addressed.
      */
    unsigned bitnum;
};

#endif // LIB_BIT_ADDRESS_H
// vim: set ts=8 sw=4 et :
