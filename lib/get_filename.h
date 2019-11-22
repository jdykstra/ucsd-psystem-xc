//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#ifndef LIB_GET_FILENAME_H
#define LIB_GET_FILENAME_H

#include <lib/ac/stdio.h>
#include <libexplain/filename.h>
#include <libexplain/getcwd.h>

#include <lib/rcstring.h>

/**
  * The filename_from_stream function is sued to obtain the name of the
  * file associtaed with the given stdio stream.  Commonly, this is
  * applied to stdin or stdout to get the name of the actual file.
  *
  * @param fp
  *     the stream to reverse engineer
  */
rcstring filename_from_stream(FILE *fp);

#endif // LIB_GET_FILENAME_H
// vim: set ts=8 sw=4 et :
