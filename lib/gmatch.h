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

#ifndef LIB_GMATCH_H
#define LIB_GMATCH_H

class rcstring; // forward

/**
  * The  gmatch()  function checks whether the "candidate" argument matches the
  * "pattern" argument, which is a shell wildcard pattern.
  *
  * @param pattern
  *     The pattern to be matched.
  *     See glob(7) for a description of file name patterns.
  * @param candidate
  *     The string to be tested for matching against the pattern.
  * @returns
  *     int; 1 for a match, 0 for no match, -1 for invalid pattern.
  */
int gmatch(const char *pattern, const char *candidate);

/**
  * The  gmatch()  function checks whether the "candidate" argument matches the
  * "pattern" argument, which is a shell wildcard pattern.
  *
  * @param pattern
  *     The pattern to be matched.
  *     See glob(3) for a description of file name patterns.
  * @param candidate
  *     The string to be tested for matching against the pattern.
  * @returns
  *     int; 1 for a match, 0 for no match, -1 for invalid pattern.
  */
int gmatch(const rcstring &pattern, const rcstring &candidate);

#endif // LIB_GMATCH_H
// vim: set ts=8 sw=4 et :
