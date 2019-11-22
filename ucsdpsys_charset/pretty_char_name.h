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

#ifndef UCSDPSYS_CHARSET_PRETTY_CHAR_NAME_H
#define UCSDPSYS_CHARSET_PRETTY_CHAR_NAME_H

#include <lib/rcstring.h>

rcstring pretty_char_name(int c);

/**
  * The unicode_char_name function may be used to obtain the unicode
  * name of a normal 0..255 character.  No other code pages are supported.
  */
rcstring unicode_char_name(int c);

#endif // UCSDPSYS_CHARSET_PRETTY_CHAR_NAME_H
// vim: set ts=8 sw=4 et :
