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

#include <lib/segment/filter.h>


segment_filter::~segment_filter()
{
}


segment_filter::segment_filter(const segment::pointer &a_deeper) :
    deeper(a_deeper)
{
}


rcstring
segment_filter::get_name(void)
    const
{
    return deeper->get_name();
}


segkind_t
segment_filter::get_kind(void)
    const
{
    return deeper->get_kind();
}


void
segment_filter::set_kind(segkind_t value)
{
    deeper->set_kind(value);
}


mtype_t
segment_filter::get_mtype(void)
    const
{
    return deeper->get_mtype();
}


void
segment_filter::set_mtype(mtype_t value)
{
    deeper->set_mtype(value);
}


segversion_t
segment_filter::get_version(void)
    const
{
    return deeper->get_version();
}


void
segment_filter::set_version(segversion_t value)
{
    deeper->set_version(value);
}


byte_sex_t
segment_filter::get_byte_sex(void)
    const
{
    return deeper->get_byte_sex();
}


unsigned
segment_filter::get_segment_number(void)
    const
{
    return deeper->get_segment_number();
}


unsigned
segment_filter::get_text_address(void)
    const
{
    return deeper->get_text_address();
}


unsigned
segment_filter::get_text_address(const rcstring &text)
{
    return deeper->get_text_address(text);
}


rcstring
segment_filter::get_interface_text(void)
    const
{
    return deeper->get_interface_text();
}


rcstring
segment_filter::get_interface_text(unsigned block_num, int block_len)
    const
{
    return deeper->get_interface_text(block_num, block_len);
}


unsigned
segment_filter::get_code_address(void)
    const
{
    return deeper->get_code_address();
}


unsigned
segment_filter::get_code_address(unsigned code_length)
{
    return deeper->get_code_address(code_length);
}


unsigned
segment_filter::get_code_length(void)
    const
{
    return deeper->get_code_length();
}


void
segment_filter::write_block(unsigned blknum, const void *data, unsigned nblocks)
    const
{
    deeper->write_block(blknum, data, nblocks);
}


void
segment_filter::read_block(unsigned blknum, void *data, unsigned nblocks)
    const
{
    deeper->read_block(blknum, data, nblocks);
}


const link_info_list &
segment_filter::get_link_info(void)
    const
{
    return deeper->get_link_info();
}


void
segment_filter::segment_not_linked(const link_info::pointer &link_info_entry)
{
    deeper->segment_not_linked(link_info_entry);
}


void
segment_filter::add_link_information(const link_info::pointer &link_info_entry)
{
    deeper->add_link_information(link_info_entry);
}


void
segment_filter::write_link_information(int nextlc)
    const
{
    deeper->write_link_information(nextlc);
}


unsigned
segment_filter::get_linkinfo_nblocks(void)
    const
{
    return deeper->get_linkinfo_nblocks();
}


// vim: set ts=8 sw=4 et :
