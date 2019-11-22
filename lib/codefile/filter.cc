//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#include <lib/codefile/filter.h>


codefile_filter::~codefile_filter()
{
}


codefile_filter::codefile_filter(const codefile::pointer &a_deeper) :
    deeper(a_deeper)
{
}


segment::pointer
codefile_filter::get_segment(unsigned number)
    const
{
    return deeper->get_segment(number);
}


void
codefile_filter::read_block(unsigned blkno, void *data, unsigned nblocks)
    const
{
    deeper->read_block(blkno, data, nblocks);
}


void
codefile_filter::write_block(unsigned blk, void const *data, unsigned nblks)
    const
{
    deeper->write_block(blk, data, nblks);
}


byte_sex_t
codefile_filter::get_byte_sex()
    const
{
    return deeper->get_byte_sex();
}


segment::pointer
codefile_filter::new_segment(rcstring const &name, int segnum,
    segkind_t kind, mtype_t mtype, segversion_t version)
{
    return deeper->new_segment(name, segnum, kind, mtype, version);
}


unsigned
codefile_filter::allocate(unsigned nblocks)
{
    return deeper->allocate(nblocks);
}


void
codefile_filter::write_segment_dictionary()
{
    deeper->write_segment_dictionary();
}


void
codefile_filter::set_copyright_notice(rcstring const &text)
{
    deeper->set_copyright_notice(text);
}


rcstring
codefile_filter::get_copyright_notice()
    const
{
    return deeper->get_copyright_notice();
}


rcstring
codefile_filter::get_filename()
    const
{
    return deeper->get_filename();
}


void
codefile_filter::create_dataseg(const rcstring &name, int segnum, int nwords)
{
    deeper->create_dataseg(name, segnum, nwords);
}


unsigned
codefile_filter::maximum_number_of_segments(void)
    const
{
    return deeper->maximum_number_of_segments();
}


codefile::pointer
codefile_filter::new_file_factory(const rcstring &outfilename)
    const
{
    return deeper->new_file_factory(outfilename);
}


codefile::disassembler_pointer
codefile_filter::native_disassembler_factory(mtype_t mtype, dislabel &where,
    discolumns &report, int seg_num, int proc_num, int proc_base,
    code_source_map *listing) const
{
    return
        deeper->native_disassembler_factory
        (
            mtype,
            where,
            report,
            seg_num,
            proc_num,
            proc_base,
            listing
        );
}


// vim: set ts=8 sw=4 et :
