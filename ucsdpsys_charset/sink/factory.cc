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
#include <lib/sizeof.h>
#include <libexplain/output.h>

#include <lib/fstrcmp.h>

#include <ucsdpsys_charset/sink/apple.h>
#include <ucsdpsys_charset/sink/include.h>
#include <ucsdpsys_charset/sink/psftools.h>
#include <ucsdpsys_charset/sink/terak.h>
#include <ucsdpsys_charset/sink/text.h>


struct table_t
{
    const char *format_name;
    bool (*candidate)(const rcstring &filename);
    sink::pointer (*create)(const rcstring &filename);
};

static const table_t table[] =
{
    { "apple", &sink_apple::candidate, &sink_apple::create },
    { "apple.charset", &sink_apple::candidate, &sink_apple::create },
    { "include", &sink_include::candidate, &sink_include::create },
    { "psftools", &sink_psftools::candidate, &sink_psftools::create },
    { "terak", &sink_terak::candidate, &sink_terak::create },
    { "terak.charset", &sink_terak::candidate, &sink_terak::create },
    { "text", &sink_text::candidate, &sink_text::create },
};


sink::pointer
sink::factory_by_name(const rcstring &format_name, const rcstring &filename)
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
            "output format %s unknown, did you mean \"%s\" instead?",
            format_name.quote_c().c_str(),
            best->format_name
        );
    }
    explain_output_error_and_die
    (
        "output format %s unknown",
        format_name.quote_c().c_str()
    );
    // NOTREACHED
    return sink::pointer();
}


sink::pointer
sink::factory(const rcstring &filename)
{
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (tp->candidate(filename))
            return tp->create(filename);
    }
    return sink_apple::create(filename);
}


// vim: set ts=8 sw=4 et :
