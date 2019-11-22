//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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
#include <unistd.h>

#include <lib/codefile.h>
#include <lib/debug.h>
#include <lib/interval.h>
#include <lib/segment.h>


codefile::~codefile()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


codefile::codefile()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


segment::pointer
codefile::new_segment(const rcstring &name, bool syscomp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);

    //
    // Obtain the next available segment number.
    //
    int segnum = next_free_segment_number(syscomp);
    if (segnum < 0)
    {
        explain_output_error_and_die
        (
            "%s: too many segments",
            get_filename().c_str()
        );
    }

    //
    // Build the new segment details.  The segments does not yet have a
    // size or a position in the codefile_file.
    //
    segkind_t segkind = LINKED;
    mtype_t mtype = mtype_pcode_from_byte_sex(get_byte_sex());
    segversion_t version = segversion_3_0;
    if (syscomp && segnum == 0)
    {
        // this is a system segment
        mtype = mtype_undefined;
        version = segversion_pre_ii_1;
    }

    return new_segment(name, segnum, segkind, mtype, version);
}


segment::pointer
codefile::new_segment(const rcstring &name, int segnum, segkind_t segkind)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(segnum > 0);
    mtype_t mtype = mtype_pcode_from_byte_sex(get_byte_sex());
    segversion_t version = segversion_3_0;
    return new_segment(name, segnum, segkind, mtype, version);
}


int
codefile::next_free_segment_number(bool syscomp, bool is_a_unit)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    interval available(0, maximum_number_of_segments());
    if (syscomp)
    {
        if (is_a_unit)
            available -= interval(0);
    }
    else
    {
        available -= interval(0);
        available -= interval(2, 7);
        available -= interval(30, 32);
        available -= interval(58, 64);
        if (is_a_unit)
            available -= interval(1);
    }
    for (unsigned sn = 0; ; ++sn)
    {
        segment::pointer sp = get_segment(sn);
        if (!sp)
            break;
        unsigned n = sp->get_segment_number();
        assert(n < 64);
        available -= interval(n);
    }
    if (available.empty())
        return -1;
    return available.get_lowest();
}


segment::pointer
codefile::get_segment_by_name(const rcstring &segname)
    const
{
    rcstring uc_name = segname.upcase();
    for (unsigned sn = 0; ; ++sn)
    {
        segment::pointer sp = get_segment(sn);
        if (!sp || uc_name == sp->get_name())
            return sp;
    }
}


segment::pointer
codefile::get_segment_by_name(const rcstring &segname, segkind_t segkind)
    const
{
    rcstring uc_name = segname.upcase();
    for (unsigned sn = 0; ; ++sn)
    {
        segment::pointer sp = get_segment(sn);
        if (!sp)
            break;
        if (segkind == sp->get_kind() && uc_name == sp->get_name())
            return sp;
    }
    return segment::pointer();
}


segment::pointer
codefile::get_segment_by_number(unsigned segnum)
    const
{
    for (unsigned sn = 0; ; ++sn)
    {
        segment::pointer sp = get_segment(sn);
        if (!sp)
            break;
        if (segnum == sp->get_segment_number())
            return sp;
    }
    return segment::pointer();
}


segment::pointer
codefile::find_segment_by_unit_name(const rcstring &name)
    const
{
    for (unsigned sn = 0; ; ++sn)
    {
        segment::pointer sp = get_segment(sn);
        if (!sp)
            break;
        if (sp->get_name() == name && sp->get_kind() == UNITSEG)
            return sp;
    }
    return segment::pointer();
}


// vim: set ts=8 sw=4 et :
