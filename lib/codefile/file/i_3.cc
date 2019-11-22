//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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
#include <lib/ac/string.h>
#include <libexplain/output.h>

#include <lib/codefile/file/i_3.h>
#include <lib/debug.h>
#include <lib/disassembler/pcode/i_3.h>
#include <lib/interval.h>
#include <lib/pascal_id.h>
#include <lib/segment/concrete.h>
#include <lib/sizeof.h>


codefile_file_i_3::~codefile_file_i_3()
{
}


codefile_file_i_3::codefile_file_i_3(const rcstring &a_filename) :
    codefile_file(a_filename)
{
    //
    // Read in the first block of the file
    //
    block0 buffer;
    assert(sizeof(buffer) == 512);
    assert(sizeof(buffer) == sizeof_block);
    read_block(0, &buffer, 1);
    // debug_hexdump(2, &buffer, sizeof(buffer));

    //
    // Figure out the byte sex of the file.  The I.3 system was only
    // ever available for PDP-11, so the byte sex will always be
    // little-endian.  But we can be generous.
    //
    if (looks_like_valid_segment_table(buffer, little_endian))
        set_byte_sex(little_endian);
    else if (looks_like_valid_segment_table(buffer, big_endian))
        set_byte_sex(big_endian);
    else
    {
        DEBUG(1, "failed segment names test");

        yuck:
        explain_output_error_and_die
        (
            "the %s file does not look like a UCSD p-System I.3 codefile, "
            "unable to locate a valid segment dictionary in block 0",
            get_filename().quote_c().c_str()
        );
    }

    //
    // Parse the segment dictionary.
    //
    interval unused_blocks(1, get_size_in_blocks());
    for (unsigned j = 0; j < 16; ++j)
    {
        DEBUG(2, "j = %u", j);
        if (buffer.segname[j][0] == ' ')
        {
            if (!all_spaces(buffer.segname[j], sizeof(buffer.segname[j])))
            {
                DEBUG(1, "segname[%d] failed zero test", j);
                goto yuck;
            }
            unsigned short codeaddr = get_word(buffer.diskinfo[j][0]);
            if (codeaddr != 0)
            {
                DEBUG(1, "codeaddr[%d] failed zero test", j);
                goto yuck;
            }
            unsigned short codeleng = get_word(buffer.diskinfo[j][1]);
            if (codeleng)
            {
                DEBUG(1, "codeleng[%d] failed zero test", j);
                goto yuck;
            }
            continue;
        }
        rcstring name = rcstring(buffer.segname[j], 8).trim();

        unsigned short codeaddr = get_word(buffer.diskinfo[j][0]);
        if (codeaddr == 0 || codeaddr >= get_size_in_blocks())
        {
            DEBUG(1, "codeaddr[%d]=%d failed sanity test", j, codeaddr);
            goto yuck;
        }
        unsigned short codeleng = get_word(buffer.diskinfo[j][1]); // bytes
        unsigned codeleng_blocks = (codeleng + 511) >> 9;
        if (codeleng == 0 || codeaddr + codeleng_blocks > get_size_in_blocks())
        {
            DEBUG(1, "codeleng[%d] failed size sanity test", j);
            goto yuck;
        }

        //
        // make sure there is no overlap between segments
        //
        interval seg_code_int(codeaddr, codeaddr + codeleng_blocks);
        if (seg_code_int * unused_blocks != seg_code_int)
        {
            // error: overpapping segments, or goes beyond length of
            // file, or something dire.
            DEBUG
            (
                2,
                "code block range is broken (seg=%s vs unused=%s)",
                seg_code_int.representation().c_str(),
                unused_blocks.representation().c_str()
            );
            goto yuck;
        }
        unused_blocks -= seg_code_int;

        segments[j] =
            segment_concrete::create
            (
                this,
                codeleng,
                codeaddr,
                name,
                LINKED, // segkind
                0, // textstart
                0, // link info size in blocks,
                j, // segment number
                mtype_pcode_from_byte_sex(get_byte_sex()),
                segversion_pre_ii_1
            );
    }
}


bool
codefile_file_i_3::candidate(const void *vblock0ptr)
{
    const block0 &buffer = *(block0 *)vblock0ptr;

    return
        (
            looks_like_valid_segment_table(buffer, little_endian)
        ||
            looks_like_valid_segment_table(buffer, big_endian)
        );
}


codefile_file_i_3::pointer
codefile_file_i_3::create(const rcstring &a_filename)
{
    return pointer(new codefile_file_i_3(a_filename));
}


codefile_file_i_3::codefile_file_i_3(
    const rcstring &a_filename,
    byte_sex_t a_byte_sex
) :
    codefile_file(a_filename, a_byte_sex)
{
    //
    // Write an empty segment dictionary to the file,
    // it will be updated later.
    //
    block0 block;
    block.clear();
    write_block(0, &block, 1);
}


codefile_file_i_3::pointer
codefile_file_i_3::create(const rcstring &a_filename, byte_sex_t a_byte_sex)
{
    return pointer(new codefile_file_i_3(a_filename, a_byte_sex));
}


codefile::pointer
codefile_file_i_3::new_file_factory(const rcstring &outfilename)
    const
{
    return create(outfilename, get_byte_sex());
}


void
codefile_file_i_3::block0::clear(void)
{
    assert(sizeof(*this) == 512);
    memset(this, 0, sizeof(*this));
    memset(segname, ' ', sizeof(segname));
}


bool
codefile_file_i_3::looks_like_valid_segment_table(const block0 &buffer,
    byte_sex_t byte_sex)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);

    //
    // Now check the segment dictionary proper.
    //
    const unsigned maxblock = 1 + 16 * (1 << (16 - 9));
    for (unsigned j = 0; j < 16; ++j)
    {
        DEBUG(2, "j = %u", j);
        if (buffer.segname[j][0] == ' ')
        {
            if (!all_spaces(buffer.segname[j], sizeof(buffer.segname[j])))
            {
                DEBUG(2, "segname[%d] failed zero test", j);
                return false;
            }
            unsigned short codeaddr =
                byte_sex_get_word(byte_sex, buffer.diskinfo[j][0]);
            if (codeaddr != 0)
            {
                DEBUG(2, "codeaddr[%d] failed zero test", j);
                return false;
            }
            unsigned short codeleng =
                byte_sex_get_word(byte_sex, buffer.diskinfo[j][1]);
            if (codeleng)
            {
                DEBUG(2, "codeleng[%d] failed zero test", j);
                return false;
            }
            continue;
        }
        if (!is_valid_segment_name(buffer.segname[j]))
        {
            DEBUG(2, "segment name broken");
            return false;
        }
        rcstring name = rcstring(buffer.segname[j], 8).trim();

        unsigned short codeaddr =
            byte_sex_get_word(byte_sex, buffer.diskinfo[j][0]);
        if (!codeaddr || codeaddr >= maxblock)
        {
            DEBUG(2, "codeaddr[%d]=%d failed sanity test", j, codeaddr);
            return false;
        }
        unsigned short codeleng =
            byte_sex_get_word(byte_sex, buffer.diskinfo[j][1]);
        unsigned codeleng_blocks = (codeleng + 511) >> 9;
        if (!codeleng || codeaddr + codeleng_blocks > maxblock)
        {
            DEBUG(2, "codeleng[%d] failed size sanity test", j);
            return false;
        }
    }

    //
    // couldn't find anything obviously wrong
    //
    return true;
}


segment::pointer
codefile_file_i_3::new_segment(const rcstring &name, int segnum,
    segkind_t segkind, mtype_t mtype, segversion_t version)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "segnum = %d", segnum);
    assert(segnum >= 0);
    if (segnum < 0 || segnum >= 16)
    {
        explain_output_error_and_die
        (
            "the %s file may only contain segment numbers in the range 0..15, "
                "segment %d %s cannot be stored in an I.3 codefile",
            get_filename().quote_c().c_str(),
            segnum,
            name.quote_c().c_str()
        );
    }
    DEBUG(2, "segkind = %s", segkind_name(segkind).c_str());
    switch (segkind)
    {
    case LINKED:
        break;

    case HOSTSEG:
    case SEGPROC:
    case UNITSEG:
    case SEPRTSEG:
    case LINKED_INTRINS:
    case UNLINKED_INTRINS:
    case DATASEG:
    default:
        explain_output_error_and_die
        (
            "the %s file may only contain LINKED segments, "
                "segment %d %s cannot be stored in an I.3 codefile",
            get_filename().quote_c().c_str(),
            segnum,
            name.quote_c().c_str()
        );
        break;
    }
    DEBUG(2, "mtype = %s", mtype_name(mtype).c_str());
    DEBUG(2, "version = %s", segversion_name(version).c_str());

    unsigned codeleng = 0;
    unsigned codeaddr = 0;
    unsigned textstart = 0;
    unsigned linkinfo_nblocks = 0;

    segment::pointer sp =
        segment_concrete::create
        (
            this,
            codeleng,
            codeaddr,
            pascal_id(name),
            segkind,
            textstart,
            linkinfo_nblocks,
            segnum,
            mtype,
            version
        );

    //
    // Insert the new segment into the segment dictionary.
    //
    segments[segnum] = sp;
    DEBUG(2, "return %p", sp.get());
    DEBUG(1, "}");
    return sp;
}


void
codefile_file_i_3::write_segment_dictionary(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    block0 block;
    assert(sizeof(block) == sizeof_block);
    block.clear();

    //
    // Place the meta-data for each segment into the segment dictionary.
    //
    for (unsigned segnum = 0; segnum < SIZEOF(segments); ++segnum)
    {
        segment::pointer sp = segments[segnum];
        if (!sp)
            continue;
        DEBUG(2, "segnum = %d", segnum);
        assert(segnum == sp->get_segment_number());

        unsigned codeaddr = sp->get_code_address();
        DEBUG(2, "codeaddr = %d", codeaddr);
        put_word(block.diskinfo[segnum][0], codeaddr);

        unsigned codeleng = sp->get_code_length();
        DEBUG(2, "codeleng = %d", codeleng);
        put_word(block.diskinfo[segnum][1], codeleng);

        // name
        rcstring temp = sp->get_name().substring(0, 8).upcase();
        DEBUG(2, "name = %s", temp.quote_c().c_str());
        memcpy(block.segname[segnum], temp.c_str(), temp.size());
    }

    //
    // Actually write the data to the file.
    //
    write_block(0, &block, 1);
    DEBUG(1, "}");
}


segment::pointer
codefile_file_i_3::get_segment(unsigned n)
    const
{
    unsigned pos = 0;
    for (;;)
    {
        if (pos >= SIZEOF(segments))
            return segment::pointer();
        if (!segments[pos])
        {
            ++pos;
            continue;
        }
        if (n == 0)
            return segments[pos];
        ++pos;
        --n;
    }
}


void
codefile_file_i_3::create_dataseg(const rcstring &segname, int segnum, int)
{
    explain_output_error_and_die
    (
        "the %s file may only contain LINKED segments, "
            "segment %d %s cannot be stored in an I.3 codefile",
        get_filename().quote_c().c_str(),
        segnum,
        segname.quote_c().c_str()
    );
}


unsigned
codefile_file_i_3::maximum_number_of_segments(void)
    const
{
    return SIZEOF(segments);
}


disassembler::pointer
codefile_file_i_3::native_disassembler_factory(mtype_t, dislabel &where,
    discolumns &report, int seg_num, int proc_num, int proc_base,
    code_source_map *listing) const
{
    return
        disassembler_pcode_i_3::create
        (
            get_byte_sex(),
            where,
            report,
            seg_num,
            proc_num,
            proc_base,
            listing
        );
}


// vim: set ts=8 sw=4 et :
