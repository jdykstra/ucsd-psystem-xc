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

#ifndef UCSDPSYS_OSMAKGEN_COLLECT_H
#define UCSDPSYS_OSMAKGEN_COLLECT_H

#include <lib/rcstring/list.h>

/**
  * The collect_output_of function is used to execute a command, and
  * collect the standard output, returning it as a list of strings, one
  * per line.
  *
  * @param command
  *     The command to be executed
  * @param output
  *     The standard output of the command, returning it as a list of
  *     strings, one per line.
  */
void collect_output_of(const rcstring &command, rcstring_list &output);

#endif // UCSDPSYS_OSMAKGEN_COLLECT_H
// vim: set ts=8 sw=4 et :
