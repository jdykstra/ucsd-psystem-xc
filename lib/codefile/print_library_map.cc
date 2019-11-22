//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/codefile.h>
#include <lib/columnar.h>
#include <lib/output.h>
#include <lib/rcstring/list.h>
#include <lib/segment.h>


void
codefile::print_library_map(output::pointer &os)
{
    library_map_columns columns(os);

    columns.comments->fprintf
    (
        "Library map for %s\n",
        get_filename().quote_c().c_str()
    );
    columns.eoln();
    columns.eoln();

    //
    // Print the copyright notice
    //
    {
        rcstring notice = get_copyright_notice();
        if (!notice.empty())
        {
            columns.comments->fputs(notice);
            columns.eoln();
            columns.eoln();
        }
    }

    //
    // Print the column headings.
    //
    columns.headings();

    //
    // Print the map for each segment.
    //
    for (size_t sn = 0; ; ++sn)
    {
        segment::pointer sp = get_segment(sn);
        if (!sp)
            break;
        sp->print_library_map(columns);
    }
}


// vim: set ts=8 sw=4 et :
