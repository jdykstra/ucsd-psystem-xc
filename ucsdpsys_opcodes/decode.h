//
// UCSD p-System operating system
// Copyright (C) 2006, 2010, 2012 Peter Miller
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

#ifndef UCSDPSYS_OPCODES_DECODE_H
#define UCSDPSYS_OPCODES_DECODE_H

class rcstring; // forward

/**
  * The decode function is used to read a binary opcodes file and
  * generate a text equivalent - because text is much easier to edit.
  *
  * @param infile
  *     The binary file to be read.  The string "-" means the standard
  *     input is to be used.
  * @param outfile
  *     The text file to be written.  The string "-" means the standard
  *     output is to be used.
  */
void decode(const rcstring &infile, const rcstring &outfile);

void random_test_data(const rcstring &infile, const rcstring &outfile);

#endif // UCSDPSYS_OPCODES_DECODE_H
// vim: set ts=8 sw=4 et :
