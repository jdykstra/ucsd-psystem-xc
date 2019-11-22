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

#include <lib/ac/stdio.h>
#include <libexplain/output.h>

#include <lib/fstrcmp.h>
#include <lib/sizeof.h>

#include <ucsdpsys_errors/arch/adaptable/6502.h>
#include <ucsdpsys_errors/arch/adaptable/6800.h>
#include <ucsdpsys_errors/arch/adaptable/8080.h>
#include <ucsdpsys_errors/arch/adaptable/ga440.h>
#include <ucsdpsys_errors/arch/adaptable/pdp11.h>
#include <ucsdpsys_errors/arch/adaptable/ti9900.h>
#include <ucsdpsys_errors/arch/adaptable/z80.h>


struct table_t
{
    mtype_t mtype;
    arch::pointer (*create)(void);
};

static const table_t table[] =
{
    { mtype_6502, &arch_adaptable_6502::create },
    { mtype_6800, &arch_adaptable_6800::create },
    { mtype_8080, &arch_adaptable_8080::create },
    { mtype_ga440, &arch_adaptable_ga440::create },
    { mtype_pdp11, &arch_adaptable_pdp11::create },
    { mtype_ti9900, &arch_adaptable_ti9900::create },
    { mtype_z80, &arch_adaptable_z80::create },
};


arch::pointer
arch::factory_by_mtype(mtype_t mtype)
{
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (tp->mtype == mtype)
            return tp->create();
    }
    explain_output_error_and_die
    (
        "the %s architecture is known, but it is not yet supported by the "
            "assembler",
        mtype_name(mtype).c_str()
    );
}


arch::pointer
arch::factory_by_arch(const rcstring &arch_name)
{
    mtype_t m = mtype_from_name(arch_name);
    if (m == mtype_undefined)
    {
        m = mtype_from_name_fuzzy(arch_name);
        if (m != mtype_undefined)
        {
            explain_output_error_and_die
            (
                "architecture %s unknown, did you mean %s instead?",
                arch_name.quote_c().c_str(),
                mtype_name(m).downcase().quote_c().c_str()
            );
        }
        explain_output_error_and_die
        (
            "architecture %s unknown",
            arch_name.quote_c().c_str()
        );
    }
    return factory_by_mtype(m);
}


arch::pointer
arch::factory_by_host(const rcstring &host_name)
{
    mtype_t m = mtype_from_host(host_name);
    if (m == mtype_undefined)
    {
        explain_output_error_and_die
        (
            "host %s unknown",
            host_name.quote_c().c_str()
        );
    }
    return factory_by_mtype(m);
}


void
arch::list(void)
{
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        printf("%s\n", mtype_name(tp->mtype).c_str());
    }
}


// vim: set ts=8 sw=4 et :
