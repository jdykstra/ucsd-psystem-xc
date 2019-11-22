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

#ifndef LIB_CARDINAL_H
#define LIB_CARDINAL_H

#include <lib/rcstring.h>

/**
  * The cardinal_style type is used to represent the various ways of
  * rendering a cardinal number for use in error messages.
  */
enum
{
    /**
      * All numbers, positive and negative, are rendered as English text.
      */
    cardinal_style_all,

    /**
      * Only zero to ten are rendered as English text, all others are
      * rendered as the conventional hindu-arabic digits.
      */
    cardinal_style_ten = 0x10,

    /**
      * This flag says to use teen hundred values, such as 1900 becomes
      * "ninteen hundred".
      */
    cardinal_style_teen_hundred = 0x2000,

    /**
      * Americans drop the "and" between hundreds and the following
      * words, this flags say to do that (one hundred five), rather then
      * the English way (one hundred and five).
      */
    cardinal_style_american = 0x4000,

    /**
      * This flag says to render 0 as "no", rather than "zero",
      * as in "no errors found".
      */
    cardinal_style_zero_is_no = 0x8000
};

/**
  * The cardinal function is used to turn a numeric value into an
  * English text string for a cardinal number ("one", "two", etc).
  */
rcstring cardinal(long n, int style = cardinal_style_all);

/**
  * The ordinal function is used to turn a numeric value into an
  * English text string for an ordinal number ("first", "second", etc(.
  */
rcstring ordinal(long n, int style = cardinal_style_all);

#endif // LIB_CARDINAL_H
// vim: set ts=8 sw=4 et :
