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

#ifndef LIB_SET_H
#define LIB_SET_H

#include <lib/rcstring.h>

/**
  * The set class is used to represent a set value.  Set are limited to
  * the range 0..(255*16-1), due to an implementation restriction in the
  * p-Machine, and the fact that there wasn't much space in 64KiB.
  *
  * 255*16-1 = 4079
  */
class set
{
public:
    /**
      * The destructor.
      */
    virtual ~set();

    /**
      * The default constructor.
      */
    set();

    /**
      * The contructor.
      * Create a set with exactly one member.
      *
      * @param bitnum
      *     The bit to set.
      */
    set(unsigned bitnum);

    /**
      * The contructor.
      * Create a set with a contiguous range of members.
      *
      * @param bitnum_lo
      *     The lowest bit to set.
      * @param bitnum_hi
      *     The highest bit to set.
      */
    set(unsigned bitnum_lo, unsigned bitnum_hi);

    /**
      * The copy constructor.
      *
      * @param arg
      *     The value to be copied.
      */
    set(const set &arg);

    /**
      * The assignment operator.
      *
      * @param arg
      *     The value to be copied.
      */
    set &operator=(const set &arg);

    /**
      * The add-and-assign operator is used to perform an in-situ set
      * union.
      *
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     a reference to this
      */
    set &operator+=(const set &rhs);

    /**
      * The addition operator is used to perform set union.
      *
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     a value which is the union of this set and the rhs set
      */
    set operator+(const set &rhs) const;

    /**
      * The subtract-and-assign operator is used to perform an in-situ
      * set difference.
      *
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     a reference to this
      */
    set &operator-=(const set &rhs);

    /**
      * The subtraction operator is used to perform set difference.
      *
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     a value which is the set difference of this set and the rhs set
      */
    set operator-(const set &rhs) const;

    /**
      * The multiply-and-assign operator is used to perform an in-situ
      * set intersection.
      *
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     a reference to this
      */
    set &operator*=(const set &rhs);

    /**
      * The multiplication operator is used to perform set intersection.
      *
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     a value which is the intersection of this set and the rhs set
      */
    set operator*(const set &rhs) const;

    /**
      * The equal-to operator is used to perform set comparisons.
      *
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     bool, the result of the comparison
      */
    bool operator==(const set &rhs) const;

    /**
      * The not-equal-to operator is used to perform set comparisons.
      *
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     bool, the result of the comparison
      */
    bool operator!=(const set &rhs) const;

    /**
      * The less-than operator is used to perform set comparisons.
      *
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     bool, the result of the comparison
      */
    bool operator<(const set &rhs) const;

    /**
      * The less-than-or-equal-to operator is used to perform set comparisons.
      *
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     bool, the result of the comparison
      */
    bool operator<=(const set &rhs) const;

    /**
      * The greater-than operator is used to perform set comparisons.
      *
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     bool, the result of the comparison
      */
    bool operator>(const set &rhs) const;

    /**
      * The greater-than-or-equal-to operator is used to perform set
      * comparisons.
      *
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     bool, the result of the comparison
      */
    bool operator>=(const set &rhs) const;

    /**
      * The size method is used to get the sizeof the set, in words.
      */
    size_t size() const;

    /**
      * The empty method is used to test whether or not a set is empty
      * (no bits set).
      */
    bool empty() const { return (size() == 0); }

    /**
      * The nth method is used to obtain the n'th word of the set.
      */
    unsigned nth(size_t n) const;

    unsigned operator[](size_t n) const { return nth(n); }

    /**
      * The clear method may be used to reset all of the bits in the set.
      */
    void clear();

    /**
      * The get_bit method is used to test whether or not the given set
      * member is present in the set.
      *
      * @param n
      *     The member to test for
      * @returns
      *     bool; true if in set, false if not in set
      */
    bool get_bit(size_t n) const;

    bool has_member(size_t n) const { return get_bit(n); }

    /**
      * The set_bit method is used to set the given bit in the set to
      * the given value.
      *
      * @param n
      *     The number of the bit to be set.
      * @param val
      *     The value of the bit to be set.
      */
    void set_bit(size_t n, bool val = true);

    enum
    {
        /**
          * The range_maximum indicates the largest acceptable bit
          * number in a set.  This a p-Machine limitation, a set may
          * contain at most 255 16-bit words.
          */
        range_maximum = ((255 * 16) - 1)
    };

    /**
      * The representation method is used (principally for debugging) to
      * build a text representation of this set's value.
      */
    rcstring representation() const;

private:
    /**
      * The length instance variable is used to remember how much of the
      * "value" array has been used.
      */
    size_t length;

    /**
      * The maximum instance variable is used to remember how large the
      * "value" array is.
      */
    size_t maximum;

    typedef unsigned short value_t;

    /**
      * The value instance variable is used to remember the base address
      * of the dynamically allocated array used to remember the settings
      * of all the bits in the set.
      */
    value_t *value;
};

#endif // LIB_SET_H
// vim: set ts=8 sw=4 et :
