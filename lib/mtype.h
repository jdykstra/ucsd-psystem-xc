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

#ifndef LIB_MTYPE_H
#define LIB_MTYPE_H

#include <lib/byte_sex.h>
#include <lib/rcstring.h>

enum mtype_t
{
    mtype_undefined,
    mtype_pcode_be,
    mtype_pcode_le,

    mtype_pdp11, // PDP-11, Terak, LSI-11
    mtype_8080, // original IBM PC
    mtype_z80, // TRS-80s, Osbourne
    mtype_ga440,
    mtype_6502, // KIM-1, Apple ][
    mtype_6800, // Sage II (?)
    mtype_ti9900, // TMS9900, TI-99/4
};

/**
  * The mtype_name function is used to decode a mtype code into
  * a human readable string.
  *
  * @param value
  *     The value to be decoded.
  * @returns
  *     a human readable string, or numeric equivalent if unknown.
  */
rcstring mtype_name(mtype_t value);

/**
  * The mtype_pcode_from_byte_sex function is used to select one of
  * mtype_pcode_le or mtype_pcode_be based on the byte sex requested.
  *
  * @param bs
  *     Thye byte sex of interest.
  * @returns
  *     one of mtype_pcode_le or mtype_pcode_be as appropriate.
  */
mtype_t mtype_pcode_from_byte_sex(byte_sex_t bs);

/**
  * The byte_sex_from_mtype function may be used to derive the byte sex
  * from the machine type.
  *
  * @param mtype
  *     The machine type
  * @return
  *     the byte sex corresponding to the machine type
  */
byte_sex_t byte_sex_from_mtype(mtype_t mtype);

/**
  * The mtype_from_name function may be used to translate an mtype name
  * into the equivalent mtype enum tag.
  *
  * @param name
  *     The name to be stranslated
  * @returns
  *     The corresponding mtype enum tag, or mtype_undefined if the name
  *     is unknown.
  */
mtype_t mtype_from_name(const rcstring &name);

/**
  * The mtype_from_name_fuzzy function may be used to translate an mtype
  * name into the equivalent mtype enum tag.  It is used to improve
  * error messages, allowing them to guess possible typographical errors
  * on the part of the user.
  *
  * @param name
  *     The name to be stranslated
  * @returns
  *     The corresponding mtype enum tag, or mtype_undefined if the name
  *     is unknown.
  */
mtype_t mtype_from_name_fuzzy(const rcstring &name);

/**
  * The mtype_from_host function is used to map a --host="host" name to
  * an mtype (an --arch="arch").
  *
  * @param name
  *     The host system name, e.g. "apple" or "terak"
  * @returns
  *     The machine type (the architecture), e.g. mtype_6502 or mtype_pdp11.
  */
mtype_t mtype_from_host(const rcstring &name);

#endif // LIB_MTYPE_H
// vim: set ts=8 sw=4 et :
