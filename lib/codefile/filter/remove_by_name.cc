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

#include <lib/codefile/filter/remove_by_name.h>


codefile_filter_remove_by_name::~codefile_filter_remove_by_name()
{
}


codefile_filter_remove_by_name::codefile_filter_remove_by_name(
    const codefile::pointer &a_deeper,
    const rcstring &segname
) :
    codefile_filter(a_deeper)
{
    rcstring uc_name = segname.upcase();
    for (unsigned sn = 0; ; ++sn)
    {
        segment::pointer sp = codefile_filter::get_segment(sn);
        if (!sp)
            break;
        if (sp->get_name() != uc_name)
            segs.push_back(sp);
    }
}


codefile::pointer
codefile_filter_remove_by_name::create(const codefile::pointer &a_deeper,
    const rcstring &segname)
{
    if (!a_deeper->get_segment_by_name(segname))
        return a_deeper;
    return pointer(new codefile_filter_remove_by_name(a_deeper, segname));
}


segment::pointer
codefile_filter_remove_by_name::get_segment(unsigned idx)
    const
{
    if (idx >= segs.size())
        return segment::pointer();
    return segs[idx];
}


// vim: set ts=8 sw=4 et :
