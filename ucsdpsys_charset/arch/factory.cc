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

#include <lib/ac/assert.h>
#include <lib/config.h>
#include <libexplain/output.h>

#include <ucsdpsys_charset/arch/normal.h>
#include <ucsdpsys_charset/arch/terak.h>


arch::pointer
arch::factory_by_arch(const rcstring &name)
{
    mtype_t mtype = mtype_from_name(name);
    if (mtype == mtype_undefined)
    {
        // try fuzzy
        mtype = mtype_from_name_fuzzy(name);
        if (mtype == mtype_undefined)
        {
            explain_output_error_and_die
            (
                "architecture %s unknown",
                name.quote_c().c_str()
            );
        }
        explain_output_error_and_die
        (
            "architecture %s unknown, did you mean %s instead?",
            name.quote_c().c_str(),
            mtype_name(mtype).quote_c().c_str()
        );
    }
    return factory_by_mtype(mtype);
}


arch::pointer
arch::factory_by_host(const rcstring &name)
{
    mtype_t mtype = mtype_from_host(name);
    if (mtype == mtype_undefined)
    {
        explain_output_error_and_die("host %s unknown", name.quote_c().c_str());
    }
    return factory_by_mtype(mtype);
}


arch::pointer
arch::factory_by_mtype(mtype_t mtype)
{
    switch (mtype)
    {
    case mtype_pdp11:
        return arch_terak::create(mtype);

    case mtype_undefined:
        assert(!"should be impossible");

    case mtype_pcode_be:
    case mtype_pcode_le:
    case mtype_8080:
    case mtype_z80:
    case mtype_ga440:
    case mtype_6502:
    case mtype_6800:
    case mtype_ti9900:
    default:
        return arch_normal::create(mtype);
    }
}


// vim: set ts=8 sw=4 et :
