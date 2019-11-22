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

#ifndef LIB_PASCAL_GRAMMAR_H
#define LIB_PASCAL_GRAMMAR_H

class rcstring; // forward
class translator; // forward

/**
  * The pascal_parse function is used to parse a pascal program.
  *
  * @param context
  *     The semantics source.
  * @param filename
  *     The name of the file to parse.
  */
void pascal_parse(translator *context, const rcstring &filename);

/**
  * The pascal_grammar_debug_set function is used to turn on or off
  * the printing of debug information by the parse, cheilfy tokens and
  * parsing actions, not semantics.
  *
  * @param flag
  *     true to turn debugging on, false for off.
  */
void pascal_grammar_debug_set(bool flag = true);

#endif // LIB_PASCAL_GRAMMAR_H
// vim: set ts=8 sw=4 et :
