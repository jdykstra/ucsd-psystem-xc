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

#include <lib/segment.h>


void
segment::print_library_map(library_map_columns &columns)
    const
{
    columns.codeaddr->fprintf("%5d", get_code_address());
    columns.codeleng->fprintf("%5d", get_code_length());
    columns.name->fputs(get_name());
    columns.segkind->fputs(segkind_name(get_kind()));
    columns.segnum->fprintf("%3d", get_segment_number());
    columns.mtype->fputs(mtype_name(get_mtype()));
    columns.version->fputs(segversion_name(get_version()));
    columns.eoln();

    switch (get_kind())
    {
    case LINKED_INTRINS:
    case SEGPROC:
    case UNITSEG:
    case UNLINKED_INTRINS:
        {
            rcstring s =
                get_interface_text
                (
                    get_text_address(),
                    get_code_address() - get_text_address()
                );
            if (!s.empty())
            {
                columns.comments->fputs(s);
                columns.eoln();
            }
        }
        break;

    case DATASEG:
    case HOSTSEG:
    case LINKED:
    case SEPRTSEG:
        break;
    }

    get_link_info().print_library_map(columns);
}


// vim: set ts=8 sw=4 et :
