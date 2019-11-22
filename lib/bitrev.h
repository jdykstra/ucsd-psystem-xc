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

#ifndef LIB_BITREV_H
#define LIB_BITREV_H

#include <lib/ac/stdint.h>

uint8_t bitrev8(uint8_t value);
uint16_t bitrev16(uint16_t value);
uint32_t bitrev32(uint32_t value);

#endif // LIB_BITREV_H
// vim: set ts=8 sw=4 et :
