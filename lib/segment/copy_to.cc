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

#include <lib/codefile.h>
#include <lib/segment.h>


void
segment::copy_to(codefile *destination)
    const
{
    segment::pointer sp =
        destination->new_segment
        (
            get_name(),
            get_segment_number(),
            get_kind(),
            get_mtype(),
            get_version()
        );

    // Copy the interface text.
    // (must be first)
    rcstring text = get_interface_text();
    if (!text.empty())
        sp->get_text_address(text);

    // allocate code address and code length, and space in the codefile
    sp->get_code_address(get_code_length());

    //
    // Now we can copy the data to the codefile.
    // (relative to codeaddr, no need to mention it here).
    //
    unsigned codeleng_blocks = (get_code_length() + 511) >> 9;
    for (unsigned n = 0; n < codeleng_blocks; ++n)
    {
        char data[codefile::sizeof_block];
        read_block(n, data, 1);
        sp->write_block(n, data, 1);
    }

    // Copy the relocation table
    // (must be last)
    const link_info_list &lil = get_link_info();
    if (!lil.empty())
    {
        for (size_t j = 0; j < lil.size(); ++j)
            sp->add_link_information(lil.get(j));
        sp->write_link_information(1);
    }
}


// vim: set ts=8 sw=4 et :
