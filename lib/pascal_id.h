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

#ifndef LIB_PASCAL_ID_H
#define LIB_PASCAL_ID_H

#include <lib/rcstring.h>

/**
  * The pascal_id funtion is used to translate a normal variabl ename
  * into the pascal variable name used in the symbol table.
  *
  * (a) variable name are case insensitive, so we always drop them to loer case,
  * (b) underscores are not significant
  * (c) only the first 8 characters are significant.
  *
  * @note
  *     The UCSD folks made up these hinky rules, not me!
  */
rcstring pascal_id(const rcstring &name);

#endif // LIB_PASCAL_ID_H
// vim: set ts=8 sw=4 et :
