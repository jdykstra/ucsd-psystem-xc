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

#include <lib/segment/filter/renumber.h>


segment_filter_renumber::~segment_filter_renumber()
{
}


segment_filter_renumber::segment_filter_renumber(
    const segment::pointer &a_deeper,
    int a_segnum
) :
    segment_filter(a_deeper),
    segnum(a_segnum)
{
    assert(segnum >= 0);
    assert(segnum < 64);
}


segment_filter_renumber::pointer
segment_filter_renumber::create(const segment::pointer &a_deeper, int a_segnum)
{
    return pointer(new segment_filter_renumber(a_deeper, a_segnum));
}


unsigned
segment_filter_renumber::get_segment_number(void)
    const
{
    return segnum;
}


void
segment_filter_renumber::read_block(unsigned blknum, void *data,
    unsigned nblocks) const
{
    //
    // Read the data from the segment
    //
    segment_filter::read_block(blknum, data, nblocks);

    //
    // If the data contains the segment number (i.e. last block in segment)
    // alter the byte that contains the segment number.
    //
    unsigned codeleng = get_code_length();
    unsigned code_nblocks = (codeleng + 511) >> 9;
    unsigned block_with_segnum = code_nblocks - 1;
    if (blknum <= block_with_segnum && block_with_segnum < blknum + nblocks)
    {
        codeleng -= blknum << 9;
        *((unsigned char *)data + codeleng - 2) = segnum;
    }
}


// vim: set ts=8 sw=4 et :
