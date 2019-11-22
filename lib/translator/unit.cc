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

#include <lib/debug.h>
#include <lib/interval.h>
#include <lib/pascal/lex.h>
#include <lib/symbol/unit.h>
#include <lib/translator.h>


void
translator::unit_intrinsic_code(int segnum)
{
    interval ok(7, 64);
    if (!ok.member(segnum))
    {
        int suggest = 7;
        pascal_lex_error
        (
            pascal_lex_location(),
            "intrinsic code %d: segment not acceptable, suggest %d instead",
            segnum,
            suggest
        );
        segnum = suggest;
    }
    unit_segnum_code = segnum;
    unit_segkind = LINKED_INTRINS;
}


void
translator::unit_intrinsic_data(int segnum)
{
    interval ok(7, 32);
    ok -= interval(unit_segnum_code);
    if (!ok.member(segnum))
    {
        int suggest = unit_segnum_code + 1;
        if (!ok.member(suggest))
        {
            for (int j = 3; ; ++j)
            {
                // 0 => +1 * 0
                // 1 => -1 * 0
                // 2 => +1 * 1
                // 3 => -1 * 1
                // 4 => +1 * 2
                // 5 => -1 * 2
                // etc
                int sign = ((j & 1) ? -1 : 1);
                int distance = j >> 1;
                suggest = unit_segnum_code + sign * distance;
                if (ok.member(suggest))
                    break;
            }
        }
        pascal_lex_error
        (
            pascal_lex_location(),
            "intrinsic data %d: segment not acceptable, suggest %d instead",
            segnum,
            suggest
        );
        segnum = suggest;
    }
    unit_segnum_data = segnum;
}


scope::pointer
translator::unit_scope(const rcstring &name)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // After this point, the user may not use the B, C, L, S or U
    // control comments.
    //
    no_more_codefile_options = true;

    if (warning_shadow_flag)
    {
        symbol::pointer osp = scopes.lookup(name);
        if (osp)
        {
            pascal_lex_warning
            (
                "unit name %s shadows the %s %s",
                name.quote_c().c_str(),
                osp->get_name().quote_c().c_str(),
                osp->get_kind()
            );
        }
    }

    //
    // Ugly hack infinity+1: the lex level height of a unit is
    // whatever it takes to be at lex level 0.
    //
    int llht = syscomp ? 1 : 0;

    scope::pointer scp =
        unit_scope_factory
        (
            name,
            unit_segnum_code,
            unit_segnum_data,
            llht,
            unit_segkind
        );
    scopes.push_back(scp);
    int lex_level = 0;
    symbol::pointer sp = symbol_unit::create(name, scp, lex_level);
    scp->attach_symbol(sp);
    scopes.define(sp, false);

    // Reset these, just in case they define several sub-units on the one
    // program.
    unit_segnum_code = -1;
    unit_segnum_data = -1;
    unit_segkind = UNITSEG;
    DEBUG(1, "}");
    return scp;
}


void
translator::unit_is_separate(void)
{
    unit_segkind = SEPRTSEG;
}


// vim: set ts=8 sw=4 et :
