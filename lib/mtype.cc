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

#include <lib/fstrcmp.h>
#include <lib/mtype.h>
#include <lib/sizeof.h>


rcstring
mtype_name(mtype_t x)
{
    switch (x)
    {
    case mtype_undefined:
        return "Undefined";

    case mtype_pcode_be:
        return "P-Code BE";

    case mtype_pcode_le:
        return "P-Code LE";

    case mtype_pdp11:
        return "PDP11";

    case mtype_8080:
        return "8080";

    case mtype_z80:
        return "Z80";

    case mtype_ga440:
        return "GA440";

    case mtype_6502:
        return "6502";

    case mtype_6800:
        return "6800";

    case mtype_ti9900:
        return "TI9900";
    }
    return rcstring::printf("%d", x);
}


mtype_t
mtype_pcode_from_byte_sex(byte_sex_t bs)
{
    return (bs == little_endian ? mtype_pcode_le : mtype_pcode_be);
}


byte_sex_t
byte_sex_from_mtype(mtype_t mtype)
{
    switch (mtype)
    {
    case mtype_undefined:
        break;

    case mtype_pcode_be:
        return big_endian;

    case mtype_pcode_le:
        return little_endian;

    case mtype_pdp11:
        return little_endian;

    case mtype_8080:
        return little_endian;

    case mtype_z80:
        return little_endian;

    case mtype_ga440:
        return big_endian;

    case mtype_6502:
        return little_endian;

    case mtype_6800:
        return big_endian;

    case mtype_ti9900:
        return big_endian;
    }

    //
    // There is no way to know.  However, the p-System was first
    // developed on little endian machines (first PDP-11, then Z80) so
    // this is probably the least problematic guess.
    //
    return little_endian;
}


struct table_t
{
    const char *name;
    mtype_t mtype;
};

//
// The values herein are used with command line arguments documented to
// be the same.
//
// Note: all of the names in the mtype_name must appear here, too.
//
static const table_t table[] =
{
    { "6502", mtype_6502 }, // preferred
    { "65c02", mtype_6502 },
    { "65c02s", mtype_6502 },
    { "65c02s-wdc", mtype_6502 },
    { "klebsch", mtype_6502 },

    { "6800", mtype_6800 }, // preferred
    { "6809", mtype_6800 },

    { "8080", mtype_8080 }, // preferred
    { "8085", mtype_8080 },
    { "apple", mtype_6502 },

    { "ga440", mtype_ga440 }, // preferred
    { "ga-16", mtype_ga440 },
    { "ga-16/440", mtype_ga440 },
    { "ga-440", mtype_ga440 },

    { "pcode-le", mtype_pcode_le }, // preferred
    { "pcode le", mtype_pcode_le },
    { "pcode", mtype_pcode_le }, // a useful default
    { "p-code-le", mtype_pcode_le },
    { "p-code le", mtype_pcode_le },
    { "p-code", mtype_pcode_le }, // a useful default

    { "pcode-be", mtype_pcode_be }, // preferred
    { "pcode be", mtype_pcode_be },
    { "p-code-be", mtype_pcode_be },
    { "p-code be", mtype_pcode_be },

    { "pdp11", mtype_pdp11 }, // preferred
    { "pdp-11", mtype_pdp11 },
    { "lsi-11", mtype_pdp11 },
    { "terak", mtype_pdp11 },

    { "ti9900", mtype_ti9900 }, // preferred
    { "ti-99-4", mtype_ti9900 },
    { "ti-99/4", mtype_ti9900 },
    { "ti-990", mtype_ti9900 },
    { "ti-9900", mtype_ti9900 },
    { "tms9900", mtype_ti9900 },

    { "z80", mtype_z80 }, // preferred
    { "z-80", mtype_z80 },
    { "zilog-z80", mtype_z80 },
};


mtype_t
mtype_from_name(const rcstring &name)
{
    rcstring lc_name = name.downcase();
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (lc_name == rcstring(tp->name))
            return tp->mtype;
    }
    return mtype_undefined;
}


mtype_t
mtype_from_name_fuzzy(const rcstring &name)
{
    rcstring lc_name = name.downcase();
    double best_weight = 0.6;
    const table_t *best_tp = 0;
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        double weight = fstrcmp(lc_name.c_str(), tp->name);
        if (weight > best_weight)
        {
            best_tp = tp;
            best_weight = weight;
        }
    }
    if (best_tp)
        return best_tp->mtype;
    return mtype_undefined;
}


mtype_t
mtype_from_host(const rcstring &host)
{
    // ignore case
    rcstring lc_host = host.downcase();

    if
    (
        lc_host.starts_with("pdp")
    ||
        lc_host.starts_with("terak")
    ||
        lc_host.starts_with("lsi")
    )
        return mtype_pdp11;

    if
    (
        lc_host == "z80"
    ||
        lc_host.starts_with("trs")
    ||
        lc_host.starts_with("osbourne")
    ||
        lc_host.gmatch("*north*star*")
    ||
        lc_host.gmatch("*horizon*") // north star
    )
        return mtype_z80;

    if
    (
        lc_host == "klebsch"
    ||
        lc_host.starts_with("kim")
    ||
        lc_host.starts_with("apple")
    ||
        lc_host.gmatch("65*02*")
    )
        return mtype_6502;

    if (lc_host.starts_with("sage"))
        return mtype_6800;

    if (lc_host.gmatch("ti*99*") || lc_host.gmatch("tms*99*"))
        return mtype_ti9900;

    if (lc_host.gmatch("*ibm*") || lc_host.gmatch("*pc*"))
        return mtype_8080;

    //
    // No idea.
    //
    // See if they gave an architecture name, rather than a host system
    // name.  There is quite a bit of overlap.
    //
    return mtype_from_name(host);
}


// vim: set ts=8 sw=4 et :
