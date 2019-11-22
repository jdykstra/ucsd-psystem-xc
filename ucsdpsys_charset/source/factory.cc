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

#include <lib/config.h>
#include <libexplain/output.h>

#include <lib/fstrcmp.h>
#include <lib/sizeof.h>

#include <ucsdpsys_charset/source/apple.h>
#include <ucsdpsys_charset/source/psftools.h>
#include <ucsdpsys_charset/source/terak.h>
#include <ucsdpsys_charset/source/text.h>


struct table_t
{
    const char *format_name;
    bool (*candidate)(const rcstring &filename);
    source::pointer (*create)(const rcstring &filename);
};

static const table_t table[] =
{
    { "apple", &source_apple::candidate, &source_apple::create },
    { "apple.charset", &source_apple::candidate, &source_apple::create },
    { "psftools", &source_psftools::candidate, &source_psftools::create },
    { "terak", &source_terak::candidate, &source_terak::create },
    { "terak.charset", &source_terak::candidate, &source_terak::create },
    { "text", &source_text::candidate, &source_text::create },
};


source::pointer
source::factory(const rcstring &filename)
{
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (tp->candidate(filename))
            return tp->create(filename);
    }
    return source_apple::create(filename);
}


source::pointer
source::factory_by_name(const rcstring &format_name, const rcstring &filename)
{
    rcstring lc_format_name = format_name.downcase();
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (lc_format_name == tp->format_name)
            return tp->create(filename);
    }

    double best_weight = 0.6;
    const table_t *best = 0;
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        double w = fstrcmp(lc_format_name.c_str(), tp->format_name);
        if (w > best_weight)
        {
            best = tp;
            best_weight = w;
        }
    }
    if (best)
    {
        explain_output_error_and_die
        (
            "input format %s unknown, did you mean \"%s\" instead?",
            format_name.quote_c().c_str(),
            best->format_name
        );
    }
    explain_output_error_and_die
    (
        "input format %s unknown",
        format_name.quote_c().c_str()
    );
    // NOTREACHED
    return source::pointer();
}


// vim: set ts=8 sw=4 et :
