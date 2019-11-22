//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#ifndef LIB_LONG_INTEGER_H
#define LIB_LONG_INTEGER_H

#include <lib/rcstring.h>

/**
  * The long_integer class is used to represent a long integer in the same way
  * that the UCSD p-machine represented them.
  *
  * The p-System stored integer[n] as (((n + 3) div 4) + 1) words.
  *
  * You could be forgiven for thinking they used radix-65536 arithmentic and
  * a sign word.  The sign word is right, the rest is uglier.  OK, how about
  * radix-10000?  Sorry, no.  Well, all that leaves is BCD, right?  No.  Well,
  * almost no: they used bass-ackwards BCD.
  *
  *       word 0     word 1                  word n
  *     +--------+-------------+---------+-------------+
  *     |        |  four most  |         |  four least |
  *     |  sign  | significant |   ...   | significant |
  *     |        |   digits    |         |   digits    |
  *     +--------+-------------+---------+-------------+
  *
  * The sign word is actually two bytes.  The one with the lowest address
  * (NOT the host byte sex least significant byte) contains 0xFF for negative
  * numbers, and 0 for positive.  The other byte is always zero (I wonder why
  * they disn't put the length in there?)
  *
  * The digits words, in host byte sex order, are laid out as follows:
  *
  *      F  E  D  C  B  A  9  8  7  6  5  4  3  2  1  0
  *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  *     |  digit 0  |  digit 1  |  digit 2  |  digit 3  |
  *     +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  *      right hand digits              left hand digits     <= bass ackwards
  *
  * The first sad thing about this representation was that it was unable
  * to take advantage of the BCD arithmetic opcodes that most of the early
  * microprocessors had.
  *
  * The second sad thing is that you can't just overlay a packed array[n] of
  * 0..9 onto the storage, and have [0] be the unit digit, [1] the tens digit,
  * and so-on.  This would have permitted a Pascal implementation initially,
  * followed by an optimized assembly version later.
  *
  * The third sad thing is that adjusting the number of words isn't a
  * case of simply truncating words, or simply appending zero words; you
  * actually have to suffle words around in both cases.
  *
  * The fourth sad thing is that relational comparisons (<, <=, >, * >=) can't
  * be applied word-by-word, because the bit significance is bass ackwards.
  * The decimal digits must be extracted and compared individually.  This did
  * not occur to me until I implemented the comparisons.
  *
  * Of course, these ugly implementation issues may be unique to the 6502
  * implementation, and other implementations could be less ugly.  But don't
  * hold your breath.
  */
class long_integer
{
public:
    /**
      * The destructor.
      */
    virtual ~long_integer();

    /**
      * The default constructor.
      */
    long_integer();

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    long_integer(const long_integer &rhs);

    /**
      * A constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    long_integer(int rhs);

    /**
      * A constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    long_integer(const rcstring &rhs);

    /**
      * A constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    long_integer(const char *rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    long_integer &operator=(const long_integer &rhs);

    /**
      * The strtol class method converts the initial part of the string
      * in nptr to a long integer value according to the given base,
      * which must be between 2 and 36 inclusive, or be the special
      * value 0.
      *
      * @param nptr
      *     The string to be evaluated.  The string may begin with
      *     an arbitrary amount of white space (as determined by
      *     isspace(3)) followed by a single optional '+' or '-' sign.
      *
      *     The remainder of the string is converted to a long int value
      *     in the obvious manner, stopping at the first character which
      *     is not a valid digit in the given base.
      *
      * @param endptr
      *     If endptr is not NULL, strtol stores the address of the
      *     first invalid character in *endptr.  If there were no
      *     digits at all, strtol stores the original value of nptr
      *     in *endptr (and returns 0).  In particular, if *nptr is not
      *     '\\0' but **endptr is '\\0' on return, the entire string is
      *     valid.
      *
      * @param radix
      *     The base of the number.  In bases above 10, the letter 'A'
      *     in either upper or lower case represents 10, 'B' represents
      *     11, and so forth, with 'Z' representing 35.  If base is zero
      *     or 16, the string may then include a "0x" prefix, and the
      *     number will be read in base 16; otherwise, a zero base is
      *     taken as 10 (decimal) unless the next character is '0', in
      *     which case it is taken as 8 (octal).
      *
      * @returns
      *     the result of the conversion, or zero if the conversion fails.
      */
    static long_integer strtol(const char *nptr, const char **endptr,
        int radix);

    /**
      * The adjust method is used to make the long integer larger or
      * smaller.
      *
      * @param to_this_many_words
      *     The desired size of the long integer.
      *     assert(to_this_many_words >= 2);
      */
    void adjust(size_t to_this_many_words);

    /**
      * The to_string method may be used to obtain a string
      * representation of the number, in decimal.
      */
    rcstring to_string(void) const;

    /**
      * The representation method may be used to obtain a string
      * representation of the number, in a format useful for debugging.
      */
    rcstring representation(void) const;

    /**
      * The add-in-place operator may be used to add two long integers
      * together.
      *
      * @param rhs
      *     The right hand size of the addition.
      */
    void operator+=(const long_integer &rhs);

    /**
      * The addition operator may be used to add two long integers
      * together.
      *
      * @param rhs
      *     The right hand size of the addition.
      */
    long_integer operator+(const long_integer &rhs) const;

    /**
      * The subtract-in-place operator may be used to add two long
      * integers together.
      *
      * @param rhs
      *     The right hand size of the subtraction.
      */
    void operator-=(const long_integer &rhs);

    /**
      * The subtraction operator may be used to add two long integers
      * together.
      *
      * @param rhs
      *     The right hand size of the subtraction.
      */
    long_integer operator-(const long_integer &rhs) const;

    /**
      * The multiply-in-place operator may be used to multiply two long
      * integers together.
      *
      * @param rhs
      *     The right hand size of the multiply.
      */
    void operator*=(const long_integer &rhs);

    /**
      * The multiply operator may be used to multiply two long integers
      * together.
      *
      * @param rhs
      *     The right hand size of the multiply.
      */
    long_integer operator*(const long_integer &rhs) const;

    /**
      * The divide-in-place operator may be used to divide two long
      * integers together.
      *
      * @param rhs
      *     The right hand size of the division.
      */
    void operator/=(const long_integer &rhs);

    /**
      * The divide operator may be used to divide two long integers
      * together.
      *
      * @param rhs
      *     The right hand size of the division.
      */
    long_integer operator/(const long_integer &rhs) const;

    /**
      * The modulo-in-place operator may be used to modulo two long
      * integers together.
      *
      * @param rhs
      *     The right hand size of the modulo.
      */
    void operator%=(const long_integer &rhs);

    /**
      * The modulo operator may be used to modulo two long integers
      * together.
      *
      * @param rhs
      *     The right hand size of the modulo.
      */
    long_integer operator%(const long_integer &rhs) const;

    struct div_t;

    /**
      * The div class method computes the value numerator/denominator
      * and returns the quotient and remainder in a div_t structure.
      * The quotient is rounded towards zero.  For positive numerator
      * and denominator, the result satisfies <i>quot * denominator +
      * rem = numerator</i>.
      *
      * @param numerator
      *     The left hand side of the division.
      * @param denominator
      *     The right hand side of the division.
      */
    static div_t div(const long_integer &numerator,
        const long_integer &denominator);

    /**
      * The is_negative method may be used to rest whether or not the
      * long integer value is negative.  It may report zero as negative
      * in some cases; if this bothers you, use the < operator, which
      * does not have this behaviour.
      */
    bool is_negative(void) const { return (nwords > 0 && word[0]); }

    /**
      * The is_zero method may be used to determine whether or not a
      * long integer value is zero.
      */
    bool is_zero(void) const;

#if 0
    /**
      * The bool operator is sued to cast a long integer value to bool.
      * A long integer value is considered to be false if it is zero,
      * and true for all other values.
      */
    operator bool(void) const { return !is_zero(); }
#endif

    /**
      * The less-than operator is used to compare this (left hand) long
      * integer value with the right hand long integer value.
      *
      * @param rhs
      *     The right hand side of the comparison
      * @return
      *     bool; true if this is less than the right hand side, false
      *     otherwise.
      */
    bool operator<(const long_integer &rhs) const;

    /**
      * The less-than-or-equal operator is used to compare this (left
      * hand) long integer value with the right hand long integer value.
      *
      * @param rhs
      *     The right hand side of the comparison
      * @return
      *     bool; true if this is less than or equal to the right hand
      *     side, false otherwise.
      */
    bool operator<=(const long_integer &rhs) const;

    /**
      * The greater-than operator is used to compare this (left hand)
      * long integer value with the right hand long integer value.
      *
      * @param rhs
      *     The right hand side of the comparison
      * @return
      *     bool; true if this is greater than the right hand side,
      *     false otherwise.
      */
    bool operator>(const long_integer &rhs) const;

    /**
      * The greater-than-or-equal operator is used to compare this (left
      * hand) long integer value with the right hand long integer value.
      *
      * @param rhs
      *     The right hand side of the comparison
      * @return
      *     bool; true if this is greater than or equal to the right
      *     hand side, false otherwise.
      */
    bool operator>=(const long_integer &rhs) const;

    /**
      * The equality operator is used to compare this (left hand) long
      * integer value with the right hand long integer value.
      *
      * @param rhs
      *     The right hand side of the comparison
      * @return
      *     bool; true if this is equal to the right hand side, false
      *     otherwise.
      */
    bool operator==(const long_integer &rhs) const;

    /**
      * The inequality operator is used to compare this (left hand) long
      * integer value with the right hand long integer value.
      *
      * @param rhs
      *     The right hand side of the comparison
      * @return
      *     bool; true if this is not equal to the right hand side, false
      *     otherwise.
      */
    bool operator!=(const long_integer &rhs) const;

    /**
      * The negate operator may be used to create a new long integer
      * value with the opposite sign as this long integer value.
      */
    long_integer operator-(void) const;

    /**
      * The abs method may be used to obtain the absolute value of this
      * long integer.
      */
    long_integer abs(void) const;

    /**
      * The trunc method is used to turn the value into an integer value.
      *
      * @note
      *     If the return value will not fit into 32 bits, the result
      *     is undefined gibberish.  It most certainly is NOT the least
      *     significant 32 bits.
      */
    long trunc(void) const;

    typedef unsigned short data_t;

    /**
      * The get_data method is used to obtain a pointer to the data
      * representing the long integer's value.
      *
      * @note
      *     Provided solely for code generation, shall not be use in any
      *     other context.
      */
    const data_t *get_data(void) const { return word; }

    /**
      * The get_data_size method is used to obtain the number of words
      * of data are in the representation of the long integer's value.
      *
      * @note
      *     Provided solely for code generation, shall not be use in any
      *     other context.
      */
    size_t get_data_size(void) const { return nwords; }

    /**
      * The pascal_lex_lint_constant class method is used to convert a
      * number recognised by the lexter into a long_integer value.
      * It has different radix indicators than the #strtol class method.
      *
      * @param text
      *     The text to be converted.
      */
    static long_integer pascal_lex_lint_constant(const rcstring &text);

private:
    /**
      * The nwords instance variable is used to remember how many words
      * have been used in the #word array.
      */
    size_t nwords;

    /**
      * The nwords_max instance variable is used to remember how many
      * words have been allocated in the #word array.
      *
      * assert(nwords <= nwords_max);
      */
    size_t nwords_max;

    /**
      * The word instance variable is used to remember the base address
      * of an array of unsigned 16-bit words.
      *
      * assert(!nwords_max == !word);
      */
    data_t *word;

    /**
      * The grow_without_preserving method is used to increse #nwords
      * (if necessary) to the given size.
      *
      * @param to_this_many_words
      *     The minimum number of words required.
      *
      * @note
      *     The words have not need initialized to anything; assume they
      *     contain random garbage.
      */
    void grow_without_preserving(size_t to_this_many_words);

    /**
      * The minimize method is called to reduce the value to the minimum
      * number of words, without changing the value.
      */
    void minimize(void);

    /**
      * The set_digit method is used to set the given digit to the given value.
      *
      * @param dignum
      *     The number of the digit.  The units digit is 0, the tens
      *     digit is 1, the hundreds digit is 2, etc.
      * @param digval
      *     The value of the digit.
      * @note
      *     This will throw an assert if dignum is off the high end of
      *     the value.  Thou shalt call #adjust first.
      */
    void set_digit(unsigned dignum, unsigned digval);

    /**
      * The get_digit method is used to obtain a single digit of the
      * long integer value.
      *
      * @param dignum
      *     The number of the digit.  The units digit is 0, the tens
      *     digit is 1, the hundreds digit is 2, etc.
      * @returns
      *     The digit requested, or 0 if the digit requested is of the
      *     high end of the value.
      */
    unsigned get_digit(unsigned dignum) const;

    /**
      * The get_ndigits method i sused to obtain the (maximum) number of
      * digits stored in this long integer instance.
      */
    size_t
    get_ndigits(void)
        const
    {
        return ((nwords <= 1) ? 0 : ((nwords - 1) * 4));
    }

    /**
      * The compare method is used to compare this long integer with a
      * second long integer.  The sign word is significant.
      *
      * @param rhs
      *     The right hand size of the comparison.
      * @returns
      *     integer; < 0 if this < rhs, == 0 if this == rhs, > 0 if this > rhs
      */
    int compare(const long_integer &rhs) const;

    /**
      * The unsigned_compare method is used to compare this long integer with a
      * second long integer.  The sign word is ignored.
      *
      * @param rhs
      *     The right hand size of the comparison.
      * @returns
      *     integer; < 0 if this < rhs, == 0 if this == rhs, > 0 if this > rhs
      */
    int unsigned_compare(const long_integer &rhs) const;

    /**
      * The negate_in_situ method is used to negate a long integer value
      * in place.
      */
    void negate_in_situ(void);

    /**
      * The unsigned_add_in_situ method is used to add two long integers
      * together, treating both as if ther were positive, ignoring the
      * sign words of both, and leaving this sign word unchanged.
      *
      * @param rhs
      *     The right hand side of the addition.
      */
    void unsigned_add_in_situ(const long_integer &rhs);

    /**
      * The unsigned_subtract_in_situ method is used to subtract another
      * long integers from this long integer, treating both as if they
      * were positive, ignoring the sign words of both, and leaving this
      * sign word unchanged.
      *
      * @param rhs
      *     The right hand side of the addition.
      */
    void unsigned_subtract_in_situ(const long_integer &rhs);

    /**
      * The udiv class method computes the value numerator/denominator
      * and returns the quotient and remainder in a div_t structure.
      * It ignores the sign of both the numerator and the denominator,
      * treating both as unsigned (positive).  The quotient is rounded
      * towards zero.  The result satisfies <i>quot * denominator + rem
      * = numerator</i>.
      *
      * @param numerator
      *     The left hand side of the division.
      * @param denominator
      *     The right hand side of the division.
      */
    static div_t udiv(const long_integer &numerator,
        const long_integer &denominator);

    /**
      * The abs_in_situ method is used to take the absolute value of a
      * long integer, in place.
      */
    void abs_in_situ(void) { if (nwords > 0) word[0] = 0; }

    /**
      * The mul10 method is used to create a new long integer that is
      * equal to this long integer multiplied by 10.
      */
    long_integer mul10(void) const;

    /**
      * The div10 method is used to create a new long integer that is
      * equal to this long integer divided by 10, truncating towards
      * zero.
      */
    long_integer div10(void) const;

    /**
      * The set_negative method may be used to set the sign of this long
      * integer.
      *
      * @param yesno
      *     true if this long integer is to be set to negative, false if
      *     this long integer is to be set to be positive.
      */
    void set_negative(bool yesno = true);
};

struct long_integer::div_t
{
    long_integer quot;
    long_integer rem;
};

#endif // LIB_LONG_INTEGER_H
// vim: set ts=8 sw=4 et :
