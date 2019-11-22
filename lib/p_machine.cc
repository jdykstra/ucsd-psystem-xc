//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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

#include <lib/ac/ctype.h>
#include <libexplain/output.h>

#include <lib/p_machine.h>
#include <lib/sizeof.h>


static p_machine_t p_machine = p_machine_ii_1;
static bool already_set = false;
static bool set_explicitly = false;


void
p_machine_set(p_machine_t value)
{
    if (already_set)
    {
        if (set_explicitly)
            explain_output_error_and_die("p-machine release already set");
        else
        {
            explain_output_error_and_die
            (
                "attempted to set the p-machine release too late in the process"
            );
        }
    }
    already_set = true;
    p_machine = value;
    set_explicitly = true;
}


bool
p_machine_set_explicitly(void)
{
    return set_explicitly;
}


p_machine_t
p_machine_get(void)
{
    already_set = true;
    return p_machine;
}


struct table_t
{
    const char *name;
    p_machine_t value;
};

static const table_t table[] =
{
    { "ii.1", p_machine_ii_1 },
    { "ii.0", p_machine_ii_0 },
    { "i.5", p_machine_i_5 },
    { "i.3", p_machine_i_3 },

    // these are for people who are roman-numeral-challenged
    { "2.1", p_machine_ii_1 },
    { "2.0", p_machine_ii_0 },
    { "1.5", p_machine_i_5 },
    { "1.3", p_machine_i_3 },
};


static inline char
safe_isalpha(char c)
{
    return isalpha((unsigned char)c);
}


void
p_machine_set(const rcstring &name)
{
    rcstring lc_name = name.downcase();

    //
    // The releases often had a trailing alpha, to indicate the bug fix
    // level (think of it as a third level version number).
    //
    if (!lc_name.empty() && safe_isalpha(lc_name.back()))
        lc_name = lc_name.substring(0, lc_name.size() - 1);

    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (lc_name == tp->name)
        {
            p_machine_set(tp->value);
            return;
        }
    }
    explain_output_error_and_die
    (
        "p-machine release %s unknown",
        name.quote_c().c_str()
    );
}


rcstring
p_machine_name(p_machine_t value)
{
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
        if (value == tp->value)
            return tp->name;
    return rcstring::printf("%d", value);
}


// vim: set ts=8 sw=4 et :
