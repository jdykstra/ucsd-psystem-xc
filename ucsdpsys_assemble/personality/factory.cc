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

#include <lib/sizeof.h>
#include <lib/symtab.h>

#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/personality/6502.h>
#include <ucsdpsys_assemble/personality/pcode.h>


personality::pointer
personality::factory_by_host(const rcstring &host_name, assembler *context)
{
    mtype_t mtype = mtype_from_host(host_name);
    if (mtype == mtype_undefined)
    {
        grammar_error("host %s unknown", host_name.quote_c().c_str());
        return personality::pointer();
    }
    return factory_by_mtype(mtype, context);
}


personality::pointer
personality::factory_by_arch(const rcstring &arch_name, assembler *context)
{
    mtype_t mtype = mtype_from_name(arch_name);
    if (mtype == mtype_undefined)
    {
        mtype = mtype_from_name_fuzzy(arch_name);
        if (mtype != mtype_undefined)
        {
            grammar_error
            (
                "architecture %s unknown, did you mean %s instead?",
                arch_name.quote_c().c_str(),
                mtype_name(mtype).quote_c().c_str()
            );
            return personality::pointer();
        }
        grammar_error("architecture %s unknown", arch_name.quote_c().c_str());
        return personality::pointer();
    }
    return factory_by_mtype(mtype, context);
}


personality::pointer
personality::factory_by_mtype(mtype_t mtype, assembler *context)
{
    struct table_t
    {
        mtype_t mtype;
        personality::pointer (*create)(assembler *context);
    };

    static const table_t table[] =
    {
        { mtype_6502, &personality_6502::create_6502 },
        { mtype_pcode_be, &personality_pcode::create_be },
        { mtype_pcode_le, &personality_pcode::create_le },
    };

    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (mtype == tp->mtype)
            return tp->create(context);
    }

    //
    // not implemented yet
    //
    grammar_error
    (
        "architecture %s is known, but is not yet supported",
        mtype_name(mtype).quote_c().c_str()
    );
    return personality::pointer();
}


// vim: set ts=8 sw=4 et :
