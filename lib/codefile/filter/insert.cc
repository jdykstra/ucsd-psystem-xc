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

#include <lib/codefile/filter/insert.h>
#include <lib/pascal_id.h>


codefile_filter_insert::~codefile_filter_insert()
{
}


codefile_filter_insert::codefile_filter_insert(
    const codefile::pointer &a_deeper,
    const codefile::pointer &a_deeper2,
    const segment::pointer &sp
) :
    codefile_filter(a_deeper),
    deeper2(a_deeper2)
{
    assert(sp);
    segs.push_back(sp);

    rcstring mname = sp->get_name();
    for (unsigned sn = 0; ; ++sn)
    {
        segment::pointer sp2 = codefile_filter::get_segment(sn);
        if (!sp2)
            break;
        if
        (
            sp2->get_name() != sp->get_name()
        &&
            sp2->get_segment_number() != sp->get_segment_number()
        )
            segs.push_back(sp2);
    }
}


codefile::pointer
codefile_filter_insert::create(const codefile::pointer &a_deeper,
    const codefile::pointer &a_deeper2, const rcstring &segname)
{
    segment::pointer sp = a_deeper2->get_segment_by_name(segname);
    if (!sp)
        return a_deeper;
    return pointer(new codefile_filter_insert(a_deeper, a_deeper2, sp));
}


codefile::pointer
codefile_filter_insert::create(const codefile::pointer &a_deeper,
    const codefile::pointer &a_deeper2, const segment::pointer &a_sp)
{
    return pointer(new codefile_filter_insert(a_deeper, a_deeper2, a_sp));
}


segment::pointer
codefile_filter_insert::get_segment(unsigned idx)
    const
{
    if (idx >= segs.size())
        return segment::pointer();
    return segs[idx];
}


// vim: set ts=8 sw=4 et :
