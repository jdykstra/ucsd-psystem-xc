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

#include <lib/ac/vector>

#include <lib/codefile.h>


static bool
compare(const segment::pointer &s1, const segment::pointer &s2)
{
    int n1 = s1->get_segment_number();
    int n2 = s2->get_segment_number();
    if (n1 == n2)
        return (s1->get_name() < s2->get_name());
    return (n1 < n2);
}


void
codefile::copy_from(const codefile::pointer &source)
{
    // The caller must guarantee this.
    assert(get_byte_sex() == source->get_byte_sex());

    // Transfer the copyright notice.
    set_copyright_notice(source->get_copyright_notice());

    // Grab a list of all segments.
    typedef std::vector<segment::pointer> segs_t;
    segs_t segs;
    for (unsigned sn = 0; ; ++sn)
    {
        segment::pointer sp = source->get_segment(sn);
        if (!sp)
            break;
        segs.push_back(sp);
    }

    // Sort the segment list by segment number.
    std::sort(segs.begin(), segs.end(), compare);

    // Copy each of the segments over.
    for (segs_t::const_iterator it = segs.begin(); it != segs.end(); ++it)
    {
        segment::pointer sp = *it;
        sp->copy_to(this);
    }
}


// vim: set ts=8 sw=4 et :
