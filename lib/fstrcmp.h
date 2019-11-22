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

#ifndef LIB_FSTRCMP_H
#define LIB_FSTRCMP_H

/**
  * The fstrcmp function is used to make fuzzy comparisons between
  * two strings.  It does this using the minimum edit algorithm, and
  * then divining the average length of the strings by the number of
  * characters they have in common.
  *
  * @param s1
  *     A string to compare.
  * @param s2
  *     A string to compare.
  * @returns
  *     double; 0.0 if the strings are completely different, 1.0 if the
  *     strings are identical, and a number between the two if they have
  *     anything at all in common.
  */
double fstrcmp(const char *s1, const char *s2);

#endif // LIB_FSTRCMP_H
// vim: set ts=8 sw=4 et :
