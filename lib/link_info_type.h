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

#ifndef LIB_LINK_INFO_TYPE_H
#define LIB_LINK_INFO_TYPE_H

#include <lib/rcstring.h>

enum link_info_type_t
{
    /**
      * end-of-link-info marker
      * ext ref types, designates
      * fields to be updated by linker
      *
      * followed by:
      *     nextlc // word 5
      */
    EOFMARK,

    /**
      * unit ref
      * referencess to invisibly used units (archaic?)
      *
      * followed by:
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    UNITREF,

    /**
      * references to external global address
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    GLOBREF,

    /**
      * Referencess to BASE level variables in host
      *
      * followed by:
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    PUBLREF,

    /**
      * References to BASE variables, allocated by linker
      *
      * followed by:
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    PRIVREF,

    /**
      * References to host BASE level constant
      * defining types, gives linker values to fix refs
      *
      * followed by:
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    CONSTREF,

    /**
      * Global address location
      *
      * defining types, gives
      * linker values to fix refs
      *
      * followed by
      *     procnum   // word 5
      *     enter_ic  // word 6
      */
    GLOBDEF,

    /**
      * BASE variable location
      *
      * followed by:
      *     base_offset // word 5
      */
    PUBLDEF,

    /**
      * BASE constant definition
      * proc/func info,
      * assem to PASCAL, and
      * PASCAL to PASCAL interface
      *
      * followed by:
      *     value    // word 5
      */
    CONSTDEF,

    /**
      * EXTERNAL procedure to be linked into PASCAL
      *
      * this is a reference to an external procedure
      * to be linked into PASCAL
      *
      * followed by:
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    EXTPROC,

    /**
      * EXTERNAL function to be linked into PASCAL
      *
      * This is a reference to an external function
      * to be linked into PASCAL.
      *
      * followed by:
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    EXTFUNC,

    /**
      * Separate procedure definition record
      * this is a definition of a procedure
      *
      * followed by:
      *     the procedure number in This segment's proc dict // word 5
      *     the extected numer of words of parameters // word 6
      */
    SEPPROC,

    /**
      * Separate function definition record
      * This is a reference to an separate (external) function.
      *
      * followed by:
      *     the procedure number in This segment's proc dict // word 5
      *     the extected numer of words of parameters // word 6
      */
    SEPFUNC,

    /**
      * PASCAL reference to a separate procedure
      * This is a reference to a separate (assembler) procedure.
      *
      * followed by:
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    SEPPREF,

    /**
      * PASCAL reference to a separate function
      * PASCAL reference to a seprate (assembler) function.
      *
      * followed by:
      *     format    // word 5
      *     nrefs     // word 6
      *     nwords    // word 7
      */
    SEPFREF
};

rcstring link_info_type_name(link_info_type_t x);

#endif // LIB_LINK_INFO_TYPE_H
// vim: set ts=8 sw=4 et :
