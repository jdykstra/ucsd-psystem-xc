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

#include <lib/ac/assert.h>
#include <lib/ac/ctype.h>

#include <lib/debug.h>
#include <lib/long_integer.h>
#include <lib/rcstring/accumulator.h>


long_integer::~long_integer()
{
    delete [] word;
}


long_integer::long_integer() :
    nwords(0),
    nwords_max(0),
    word(0)
{
}


long_integer::long_integer(const long_integer &rhs) :
    nwords(0),
    nwords_max(0),
    word(0)
{
    grow_without_preserving(rhs.nwords);
    for (size_t j = 0; j < rhs.nwords; ++j)
        word[j] = rhs.word[j];
    minimize();
}


static unsigned
count_words(unsigned n)
{
    unsigned result = 2;
    for (;;)
    {
        n /= 10000;
        if (!n)
            return result;
        ++result;
    }
}


long_integer::long_integer(int n) :
    nwords(0),
    nwords_max(0),
    word(0)
{
    bool neg = false;
    if (n < 0)
    {
        n = -n;
        neg = true;
    }
    grow_without_preserving(count_words(n));
    for (size_t j = 0; j < nwords; ++j)
        word[j] = 0;
    size_t dignum = 0;
    while (n)
    {
        set_digit(dignum, n % 10);
        n /= 10;
        ++dignum;
    }
    set_negative(neg);
    minimize();
}


long_integer::long_integer(const rcstring &s) :
    nwords(0),
    nwords_max(0),
    word(0)
{
    *this = strtol(s.c_str(), 0, 0);
}


long_integer::long_integer(const char *s) :
    nwords(0),
    nwords_max(0),
    word(0)
{
    *this = strtol(s, 0, 0);
}


long_integer
long_integer::strtol(const char *start, const char **ep, int radix)
{
    assert(radix <= 36);
    assert(radix == 0 || radix >= 2);
    const char *cp = start;
    for (;;)
    {
        unsigned char c = *cp;
        if (!c)
            break;
        if (isspace(c))
        {
            ++cp;
            continue;
        }
        break;
    }
    bool neg = false;
    switch (*cp)
    {
    case '-':
        neg = true;
        // Fall through...

    case '+':
        ++cp;
        break;
    }
    switch (radix)
    {
    case 16:
        if (cp[0] == '0' && (cp[1] == 'x' || cp[1] == 'X'))
            cp += 2;
        break;

    case 0:
        radix = 10;
        if (cp[0] == '0')
        {
            radix = 8;
            if (cp[1] == 'x' || cp[1] == 'X')
            {
                cp += 2;
                radix = 16;
            }
        }
        break;
    }
    const char *digits_begin_here = cp;
    long_integer lradix(radix);
    long_integer result;
    for (;;)
    {
        unsigned char c = *cp;
        switch (c)
        {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                int n = c - '0';
                if (n >= radix)
                    break;
                result = result * lradix + long_integer(n);
                ++cp;
            }
            continue;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            {
                int n = c - 'A' + 10;
                if (n >= radix)
                    break;
                result = result * radix + n;
                ++cp;
            }
            continue;

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
            {
                int n = c - 'a' + 10;
                if (n >= radix)
                    break;
                result = result * radix + n;
                ++cp;
            }
            continue;

        default:
            break;
        }
        break;
    }
    if (cp == digits_begin_here)
    {
        if (ep)
            *ep = start;
        return long_integer();
    }
    if (neg)
        result.negate_in_situ();
    if (ep)
        *ep = cp;
    return result;
}


long_integer &
long_integer::operator=(const long_integer &rhs)
{
    if (this != &rhs)
    {
        grow_without_preserving(rhs.nwords);
        for (size_t j = 0; j < rhs.nwords; ++j)
            word[j] = rhs.word[j];
        minimize();
    }
    return *this;
}


void
long_integer::grow_without_preserving(size_t to_this_many_words)
{
    if (nwords_max < to_this_many_words)
    {
        delete [] word;
        size_t new_nwords_max = 16;
        while (new_nwords_max < to_this_many_words)
            new_nwords_max = new_nwords_max * 2 + 16;
        word = new data_t [new_nwords_max];
        nwords_max = new_nwords_max;
    }
    nwords = to_this_many_words;
}


void
long_integer::minimize(void)
{
    if (nwords == 0)
    {
        return;
    }
    if (nwords == 1)
    {
        nwords = 0;
        return;
    }
    size_t j = 1;
    for (; j < nwords; ++j)
    {
        if (word[j])
            break;
    }
    if (j == nwords)
    {
        nwords = 0;
        return;
    }
    if (j > 1)
    {
        size_t shift = j - 1;
        nwords -= shift;
        for (size_t k = 1; k < nwords; ++k)
            word[k] = word[shift + k];
    }
}


void
long_integer::adjust(size_t to_this_many_words)
{
    // These are the limits imposed by UCSD Pascal language.
    assert(to_this_many_words >= 2);
    assert(to_this_many_words <= 10);

    if (to_this_many_words == nwords)
    {
        DEBUG(1, "}");
        return;
    }
    if (to_this_many_words < nwords)
    {
        //
        //  +--------+--------+--------+--------+
        //  |  sign  |   hi   |        |   lo   |
        //  +--------+--------+--------+--------+
        //       v                 . '      . '
        //       v             . '      . '
        //       v         . '      . '
        //       v     . '      . '
        //  +--------+--------+
        //  |  sign  |   lo   |
        //  +--------+--------+
        //
        size_t shift = nwords - to_this_many_words;
        nwords = to_this_many_words;
        for (size_t k = 1; k < nwords; ++k)
            word[k] = word[shift + k];
    }
    else
    {
        //
        //  +--------+--------+
        //  |  sign  |   lo   |
        //  +--------+--------+
        //       v     ' .      ' .
        //       v         ' .      ' .
        //       v             ' .      ' .
        //       v                 ' .      ' .
        //  +--------+--------+--------+--------+
        //  |  sign  |    0   |    0   |   lo   |
        //  +--------+--------+--------+--------+
        //
        size_t shift = to_this_many_words - (nwords ? nwords : 1);
        if (to_this_many_words > nwords_max)
        {
            size_t new_nwords_max = nwords_max;
            while (new_nwords_max < to_this_many_words)
                new_nwords_max = new_nwords_max * 2 + 16;
            data_t *new_words = new data_t [new_nwords_max];
            new_words[0] = ((nwords == 0) ? 0 : word[0]);
            for (size_t k = 1; k < nwords; ++k)
                new_words[shift + k] = word[k];
            for (size_t k = 1; k <= shift; ++k)
                new_words[k] = 0;
            delete [] word;
            word = new_words;
            nwords_max = new_nwords_max;
        }
        else
        {
            for (size_t j = 1; j < nwords; ++j)
            {
                size_t jj = nwords - j;
                word[shift + jj] = word[jj];
            }
            for (size_t j = 1; j <= shift; ++j)
                word[j] = 0;
        }
        nwords = to_this_many_words;
    }
}


unsigned
long_integer::get_digit(unsigned n)
    const
{
    unsigned wnum = n / 4 + 1;
    if (wnum >= nwords)
        return 0;
    wnum = nwords - wnum;
    return ((word[wnum] >> (12 - (n & 3) * 4)) & 0x0F);
}


rcstring
long_integer::to_string(void)
    const
{
    rcstring_accumulator ac;
    if (is_negative())
        ac.push_back('-');
    size_t empty = ac.size();
    for (size_t k = 0; k < get_ndigits(); ++k)
    {
        unsigned dignum = get_ndigits() - 1 - k;
        unsigned n = get_digit(dignum);
        if (n != 0 || ac.size() > empty)
            ac.push_back('0' + n);
    }
    if (ac.size() <= empty)
        return "0";
    return ac.mkstr();
}


void
long_integer::set_digit(unsigned dignum, unsigned digval)
{
    assert(digval < 10);
    unsigned wnum = dignum / 4 + 1;
    assert(wnum < nwords); // call adjust() first
    wnum = nwords - wnum;
    unsigned shift = 12 - 4 * (dignum & 3);
    word[wnum] = (word[wnum] & ~(0x0F << shift)) | (digval << shift);
}


static size_t
max(size_t a, size_t b)
{
    return (a < b ? b : a);
}


void
long_integer::operator+=(const long_integer &rhs)
{
    if (is_negative())
    {
        if (rhs.is_negative())
        {
            // (-lhs) + (-rhs)
            unsigned_add_in_situ(rhs);
        }
        else
        {
            // (-lhs) + (+rhs)
            int cmp = unsigned_compare(rhs);
            if (cmp == 0)
            {
                nwords = 0;
                return;
            }
            else if (cmp > 0)
            {
                // (-big) + (+small)
                unsigned_subtract_in_situ(rhs);
            }
            else
            {
                // (-small) + (+big)
                long_integer result(rhs);
                result.unsigned_subtract_in_situ(*this);
                *this = result;
            }
        }
    }
    else
    {
        if (rhs.is_negative())
        {
            // (+lhs) + (-rhs)
            int cmp = unsigned_compare(rhs);
            if (cmp == 0)
            {
                nwords = 0;
            }
            else if (cmp > 0)
            {
                // (+big) + (-small)
                unsigned_subtract_in_situ(rhs);
            }
            else
            {
                // (+small) + (-big)
                long_integer result = rhs;
                result.unsigned_subtract_in_situ(*this);
                *this = result;
            }
        }
        else
        {
            // (+lhs) + (+rhs)
            unsigned_add_in_situ(rhs);
        }
    }
}


void
long_integer::operator-=(const long_integer &rhs)
{
    if (is_negative())
    {
        if (rhs.is_negative())
        {
            // (-lhs) - (-rhs)
            int cmp = unsigned_compare(rhs);
            if (cmp == 0)
            {
                nwords = 0;
            }
            else if (cmp > 0)
            {
                // (-big) - (-small)
                unsigned_subtract_in_situ(rhs);
            }
            else
            {
                // (-small) - (-big)
                long_integer result = rhs;
                result.negate_in_situ();
                result.unsigned_subtract_in_situ(*this);
                *this = result;
            }
        }
        else
        {
            // (-lhs) - (+rhs)
            unsigned_add_in_situ(rhs);
        }
    }
    else
    {
        if (rhs.is_negative())
        {
            // (+lhs) - (-rhs)
            unsigned_add_in_situ(rhs);
        }
        else
        {
            // (+lhs) - (+rhs)
            int cmp = unsigned_compare(rhs);
            if (cmp == 0)
            {
                nwords = 0;
            }
            else if (cmp > 0)
            {
                // (+big) - (+small)
                unsigned_subtract_in_situ(rhs);
            }
            else
            {
                // (+small) - (+big)
                long_integer result = rhs;
                result.unsigned_subtract_in_situ(*this);
                result.negate_in_situ();
                *this = result;
            }
        }
    }
}


void
long_integer::unsigned_add_in_situ(const long_integer &rhs)
{
    adjust(max(nwords, rhs.nwords) + 1);
    unsigned carry = 0;
    for (size_t dignum = 0; dignum < get_ndigits(); ++dignum)
    {
        unsigned sum = get_digit(dignum) + rhs.get_digit(dignum) + carry;
        set_digit(dignum, sum % 10);
        carry = sum / 10;
    }
    assert(carry == 0);
    minimize();
}


void
long_integer::unsigned_subtract_in_situ(const long_integer &rhs)
{
    assert(unsigned_compare(rhs) >= 0);
    int borrow = 0;
    for (size_t dignum = 0; dignum < get_ndigits(); ++dignum)
    {
        int diff = (int)get_digit(dignum) - (int)rhs.get_digit(dignum) - borrow;
        borrow = 0;
        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        assert(diff >= 0);
        assert(diff < 10);
        set_digit(dignum, diff);
    }
    assert(borrow == 0);
    minimize();
}


long_integer
long_integer::operator+(const long_integer &rhs)
    const
{
    long_integer result(*this);
    result += rhs;
    return result;
}


long_integer
long_integer::operator-(const long_integer &rhs)
    const
{
    long_integer result(*this);
    result -= rhs;
    return result;
}


rcstring
long_integer::representation(void)
    const
{
    rcstring_accumulator ac;
    ac.push_back('{');
    for (size_t j = 0; j < nwords; ++j)
    {
        if (ac.size() > 1)
            ac.push_back(',');
        ac.push_back(rcstring::printf(" %04X", word[j]));
    }
    ac.push_back(" }");
    return ac.mkstr();
}


void
long_integer::operator*=(const long_integer &rhs)
{
    *this = *this * rhs;
}


long_integer
long_integer::operator*(const long_integer &rhs)
    const
{
    long_integer result;
    result.grow_without_preserving(nwords + rhs.nwords);
    for (size_t j = 0; j < result.nwords; ++j)
        result.word[j] = 0;
    for (size_t j = 0; j < get_ndigits(); ++j)
    {
        unsigned d = get_digit(j);
        if (!d)
            continue;
        unsigned carry = 0;
        for (size_t k = 0; k < rhs.get_ndigits(); ++k)
        {
            unsigned prod = d * rhs.get_digit(k) + carry;
            prod += result.get_digit(j + k);
            result.set_digit(j + k, prod % 10);
            carry = prod / 10;
        }
        if (carry)
        {
            size_t k = rhs.get_ndigits();
            data_t prod = result.get_digit(j + k) + carry;
            result.set_digit(j + k, prod % 10);
        }
    }
    result.set_negative(is_negative() != rhs.is_negative());
    result.minimize();
    return result;
}


int
long_integer::compare(const long_integer &rhs)
    const
{
    if (is_negative())
    {
        if (rhs.is_negative())
            return -unsigned_compare(rhs);
        else
            return -1;
    }
    else
    {
        if (rhs.is_negative())
            return 1;
        else
            return unsigned_compare(rhs);
    }
}


int
long_integer::unsigned_compare(const long_integer &rhs)
    const
{
    size_t ndig = max(get_ndigits(), rhs.get_ndigits());
    for (unsigned j = 0; j < ndig; ++j)
    {
        unsigned dignum = ndig - 1 - j;
        int ldig = get_digit(dignum);
        int rdig = rhs.get_digit(dignum);
        if (ldig == rdig)
            continue;
        return (ldig - rdig);
    }
    return 0;
}


bool
long_integer::operator<(const long_integer &rhs)
    const
{
    return (compare(rhs) < 0);
}


bool
long_integer::operator<=(const long_integer &rhs)
    const
{
    return (compare(rhs) <= 0);
}


bool
long_integer::operator>(const long_integer &rhs)
    const
{
    return (compare(rhs) > 0);
}


bool
long_integer::operator>=(const long_integer &rhs)
    const
{
    int cmp = compare(rhs);
    DEBUG(1, "(%s >= %s) = %s", to_string().c_str(), rhs.to_string().c_str(),
        (cmp >= 0 ? "true" : "false"));
    return (cmp >= 0);
}


bool
long_integer::operator==(const long_integer &rhs)
    const
{
    return (compare(rhs) == 0);
}


bool
long_integer::operator!=(const long_integer &rhs)
    const
{
    return (compare(rhs) != 0);
}


long_integer
long_integer::operator-(void)
    const
{
    long_integer result(*this);
    result.negate_in_situ();
    return result;
}


void
long_integer::negate_in_situ()
{
    set_negative(!is_negative());
}


long_integer
long_integer::operator/(const long_integer &rhs)
    const
{
    div_t result = div(*this, rhs);
    return result.quot;
}


void
long_integer::operator/=(const long_integer &rhs)
{
    div_t result = div(*this, rhs);
    *this = result.quot;
}


long_integer
long_integer::operator%(const long_integer &rhs)
    const
{
    div_t result = div(*this, rhs);
    return result.rem;
}


void
long_integer::operator%=(const long_integer &rhs)
{
    div_t result = div(*this, rhs);
    *this = result.rem;
}


long_integer::div_t
long_integer::div(const long_integer &lhs, const long_integer &rhs)
{
    div_t result = udiv(lhs, rhs);
    if (lhs.is_negative() != rhs.is_negative())
        result.quot.negate_in_situ();
    return result;
}


bool
long_integer::is_zero(void)
    const
{
    if (nwords <= 1)
        return true;
    for (size_t j = 1; j < nwords; ++j)
        if (word[j] != 0)
            return false;
    return true;
}


long_integer::div_t
long_integer::udiv(const long_integer &lhs, const long_integer &rhs)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    div_t result;
    if (rhs.is_zero())
    {
        result.rem = lhs;
        DEBUG(1, "}");
        return result;
    }

    // We will use long division, base 10, just like western school
    // children learn in primary school.
    long_integer numerator(lhs);
    numerator.abs_in_situ();
    DEBUG(2, "numerator = %s", numerator.to_string().c_str());
    long_integer denominator(rhs);
    denominator.abs_in_situ();
    DEBUG(2, "denominator = %s", denominator.to_string().c_str());
    long_integer signif(1);
    while (numerator > denominator)
    {
        denominator = denominator.mul10();
        DEBUG(2, "denominator = %s", denominator.to_string().c_str());
        signif = signif.mul10();
        DEBUG(2, "signif = %s", signif.to_string().c_str());
    }
    while (!signif.is_zero() && !numerator.is_zero())
    {
        while (numerator >= denominator)
        {
            numerator -= denominator;
            DEBUG(2, "numerator = %s", numerator.to_string().c_str());
            result.quot += signif;
            DEBUG(2, "result.quot = %s", result.quot.to_string().c_str());
        }
        denominator = denominator.div10();
        DEBUG(2, "denominator = %s", denominator.to_string().c_str());
        signif = signif.div10();
        DEBUG(2, "signif = %s", signif.to_string().c_str());
    }
    result.rem = numerator;
    DEBUG(1, "}");
    return result;
}


long_integer
long_integer::mul10(void)
    const
{
    long_integer result;
    if (!nwords)
        return result;

    // FIXME: We could this this faster, using bass ackwards shifts, but
    // don't do that now.  Focus on reviewable correctness, get fancy
    // once we have a comprehensive test suite.
    result.adjust(nwords + 1);
    for (size_t j = 0; j < get_ndigits(); ++j)
        result.set_digit(j + 1, get_digit(j));
    result.minimize();
    return result;
}


long_integer
long_integer::div10(void)
    const
{
    long_integer result;
    if (!nwords)
        return result;

    // FIXME: We could this this faster, using bass ackwards shifts, but
    // don't do that now.  Focus on reviewable correctness, get fancy
    // once we have a comprehensive test suite.
    result.adjust(nwords);
    for (size_t j = 1; j < get_ndigits(); ++j)
        result.set_digit(j - 1, get_digit(j));
    result.set_negative(is_negative());
    result.minimize();
    DEBUG(1, "div10(%s) = %s", to_string().c_str(), result.to_string().c_str());
    return result;
}


long_integer
long_integer::abs(void)
    const
{
    long_integer result(*this);
    result.abs_in_situ();
    return result;
}


void
long_integer::set_negative(bool yesno)
{
    if (nwords > 0)
    {
        word[0] = (yesno ? 0xFFFFu : 0);
    }
}


long
long_integer::trunc(void)
    const
{
    if (nwords <= 1)
        return 0;

    // Calculate the number of digits that an unsigned long can hold.
    // This is how we convert just the digits of interest.
    int ndigits = 0;
    {
        unsigned long test = ~0uL;
        test >>= 1;
        while (test)
        {
            test /= 10;
            ++ndigits;
        }
    }

    long value = 0;
    for (int j = ndigits - 1; j >= 0; --j)
        value = value * 10 + get_digit(j);
    return (is_negative() ? -value : value);
}



long_integer
long_integer::pascal_lex_lint_constant(const rcstring &text)
{
    if (text[0] == '$')
        return strtol(text.c_str() + 1, 0, 16);
    if (text[0] == '%')
        return strtol(text.c_str() + 1, 0, 2);

    const char *ep = 0;
    long_integer n = strtol(text.c_str(), &ep, 10);
    if (*ep != '#')
        return n;
    assert(n >= 2);
    assert(n <= 26);
    return strtol(ep + 1, 0, n.trunc());
}


// vim: set ts=8 sw=4 et :
