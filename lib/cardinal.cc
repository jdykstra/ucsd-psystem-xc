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

#include <lib/ac/assert.h>
#include <lib/ac/stdlib.h>

#include <lib/cardinal.h>


rcstring
cardinal(long n, int style)
{
    if (style & cardinal_style_ten)
    {
        if (n < 0 || n > 10)
            return rcstring::printf("%ld", n);
    }
    if (n < 0)
        return ("minus " + cardinal(-n, style));

    if (n == 0)
    {
        if (style & cardinal_style_zero_is_no)
            return "no";
    }
    if (n < 20)
    {
        static const char *const name[] =
        {
            "zero",
            "one",
            "two",
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine",
            "ten",
            "eleven",
            "twelve",
            "thirteen",
            "fourteen",
            "fifteen",
            "sixteen",
            "seventeen",
            "eighteen",
            "nineteen"
        };
        return name[n];
    }
    if (n < 100)
    {
        static const char *const name[] =
        {
            "zero (bug)",
            "ten (bug)",
            "twenty",
            "thirty",
            "forty",
            "fifty",
            "sixty",
            "seventy",
            "eighty",
            "ninety",
        };

        ldiv_t ans = ldiv(n, 10);
        assert(ans.quot >= 2);
        if (ans.rem == 0)
            return name[ans.quot];
        return (name[ans.quot] + (" " + cardinal(ans.rem, style)));
    }
    if ((style & cardinal_style_teen_hundred) ? (n < 2000) : (n < 1000))
    {
        ldiv_t ans = ldiv(n, 100);
        if (ans.rem == 0)
            return (cardinal(ans.quot, style) + " hundred");
        if (style & cardinal_style_american)
        {
            return
                (
                    cardinal(ans.quot, style)
                +
                    " hundred "
                +
                    cardinal(ans.rem, style)
                );
        }
        return
            (
                cardinal(ans.quot, style)
            +
                " hundred and "
            +
                cardinal(ans.rem, style)
            );
    }
    if (n < 1000000)
    {
        ldiv_t ans = ldiv(n, 1000);
        if (ans.rem == 0)
            return (cardinal(ans.quot, style) + " thousand");
        if (ans.rem >= 100 || (style & cardinal_style_american))
        {
            return
                (
                    cardinal(ans.quot, style)
                +
                    " thousand "
                +
                    cardinal(ans.rem, style)
                );
        }
        return
            (
                cardinal(ans.quot, style)
            +
                " thousand and "
            +
                cardinal(ans.rem, style)
            );
    }

    //
    // British English has billion == 10**12
    // (i.e. 1 million million == 1 billion)
    //
    if (!(style & cardinal_style_american) || n < 1000000000)
    {
        ldiv_t ans = ldiv(n, 1000000);
        if (ans.rem == 0)
            return (cardinal(ans.quot, style) + " million");
        if (ans.rem >= 100 || (style & cardinal_style_american))
        {
            return
                (
                    cardinal(ans.quot, style)
                +
                    " million "
                +
                    cardinal(ans.rem, style)
                );
        }
        return
            (
                cardinal(ans.quot, style)
            +
                " million and "
            +
                cardinal(ans.rem, style)
            );
    }

    //
    // Americal English has billion == 10**9
    // (i.e 1 thousand miller == 1 billion)
    //
    ldiv_t ans = ldiv(n, 1000000000);
    if (ans.rem == 0)
        return (cardinal(ans.quot, style) + " billion");
    if (ans.rem >= 100 || (style & cardinal_style_american))
    {
        return
            (
                cardinal(ans.quot, style)
            +
                " billion "
            +
                cardinal(ans.rem, style)
            );
    }
    return
        (
            cardinal(ans.quot, style)
        +
            " billion and "
        +
            cardinal(ans.rem, style)
        );
}


rcstring
ordinal(long n, int style)
{
    if (style & cardinal_style_ten)
    {
        if (n < 0 || n > 10)
        {
            static const char *const name[] =
            {
                "th",
                "st",
                "nd",
                "rd",
                "th",
                "th",
                "th",
                "th",
                "th",
                "th",
            };
            const char *suffix = name[labs(n) % 10];
            return rcstring::printf("%ld%s", n, suffix);
        }
    }
    if (n < 0)
        return ("minus " + ordinal(-n, style));

    if (n < 20)
    {
        static const char *const name[] =
        {
            "zeroth",
            "first",
            "second",
            "third",
            "fourth",
            "fifth",
            "sixth",
            "seventh",
            "eighth",
            "ninth",
            "tenth",
            "eleventh",
            "twelfth",
            "thirteenth",
            "fourteenth",
            "fifteenth",
            "sixteenth",
            "seventeenth",
            "eighteenth",
            "nineteenth"
        };
        return name[n];
    }
    if (n < 100)
    {

        ldiv_t ans = ldiv(n, 10);
        assert(ans.quot >= 2);
        if (ans.rem == 0)
        {
            static const char *const name[] =
            {
                "zero(bug)th",
                "ten(bug)th",
                "twentieth",
                "thirtieth",
                "fortieth",
                "fiftieth",
                "sixtieth",
                "seventieth",
                "eightieth",
                "ninetieth",
            };
            return name[ans.quot];
        }

        static const char *const name[] =
        {
            "zero(bug)",
            "ten(bug)",
            "twenty ",
            "thirty ",
            "forty ",
            "fifty ",
            "sixty ",
            "seventy ",
            "eighty ",
            "ninety ",
        };
        return (name[ans.quot] + ordinal(ans.rem, style));
    }
    if ((style & cardinal_style_teen_hundred) ? (n < 2000) : (n < 1000))
    {
        ldiv_t ans = ldiv(n, 100);
        if (ans.rem == 0)
            return (cardinal(ans.quot, style) + " hundredth");
        if (style & cardinal_style_american)
        {
            return
                (
                    cardinal(ans.quot, style)
                +
                    " hundred "
                +
                    ordinal(ans.rem, style)
                );
        }
        return
            (
                cardinal(ans.quot, style)
            +
                " hundred and "
            +
                ordinal(ans.rem, style)
            );
    }
    if (n < 1000000)
    {
        ldiv_t ans = ldiv(n, 1000);
        if (ans.rem == 0)
            return (cardinal(ans.quot, style) + " thousandth");
        if (ans.rem >= 100 || (style & cardinal_style_american))
        {
            return
                (
                    cardinal(ans.quot, style)
                +
                    " thousand "
                +
                    ordinal(ans.rem, style)
                );
        }
        return
            (
                cardinal(ans.quot, style)
            +
                " thousand and "
            +
                ordinal(ans.rem, style)
            );
    }

    //
    // British English has billion == 10**12
    // (i.e. 1 million million == 1 billion)
    //
    if (!(style & cardinal_style_american) || n < 1000000000)
    {
        ldiv_t ans = ldiv(n, 1000000);
        if (ans.rem == 0)
            return (cardinal(ans.quot, style) + " millionth");
        if (ans.rem >= 100 || (style & cardinal_style_american))
        {
            return
                (
                    cardinal(ans.quot, style)
                +
                    " million "
                +
                    ordinal(ans.rem, style)
                );
        }
        return
            (
                cardinal(ans.quot, style)
            +
                " million and "
            +
                ordinal(ans.rem, style)
            );
    }

    //
    // Americal English has billion == 10**9
    // (i.e 1 thousand miller == 1 billion)
    //
    ldiv_t ans = ldiv(n, 1000000000);
    if (ans.rem == 0)
        return (cardinal(ans.quot, style) + " billionth");
    if (ans.rem >= 100 || (style & cardinal_style_american))
    {
        return
            (
                cardinal(ans.quot, style)
            +
                " billion "
            +
                ordinal(ans.rem, style)
            );
    }
    return
        (
            cardinal(ans.quot, style)
        +
            " billion and "
        +
            ordinal(ans.rem, style)
        );
}


// vim: set ts=8 sw=4 et :
