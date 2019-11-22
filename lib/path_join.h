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

#ifndef LIB_PATH_JOIN_H
#define LIB_PATH_JOIN_H

#include <lib/rcstring.h>

/**
  * The path_join function is used to join a directory part and a
  * relative path together.  It takes care of redundant "." path
  * elements, to make the result nicer for humans to read.
  *
  * @param lhs
  *     The left hand side of the join.
  * @param rhs
  *     The right hand side of the join.
  * @returns
  *     a string with the two joined by a slash.
  */
rcstring path_join(const rcstring &lhs, const rcstring &rhs);

/**
  * The path_join function is used to join a directory part and a
  * relative path together.  It takes care of redundant "." path
  * elements, to make the result nicer for humans to read.
  *
  * @param f1
  *     The first string to join.
  * @param f2
  *     The second string to join.
  * @param f3
  *     The third string to join.
  * @returns
  *     a string with the two joined by a slash.
  */
rcstring path_join(const rcstring &f1, const rcstring &f2, const rcstring &f3);

/**
  * The path_join function is used to join a directory part and a
  * relative path together.  It takes care of redundant "." path
  * elements, to make the result nicer for humans to read.
  *
  * @param f1
  *     The first string to join.
  * @param f2
  *     The second string to join.
  * @param f3
  *     The third string to join.
  * @param f4
  *     The forth string to join.
  * @returns
  *     a string with the two joined by a slash.
  */
rcstring path_join(const rcstring &f1, const rcstring &f2, const rcstring &f3,
    const rcstring &f4);

#endif // LIB_PATH_JOIN_H
// vim: set ts=8 sw=4 et :
