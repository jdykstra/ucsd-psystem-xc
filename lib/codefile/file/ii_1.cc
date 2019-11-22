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

#include <lib/codefile/file/ii_1.h>
#include <lib/debug.h>
#include <lib/disassembler/pcode/ii_1.h>
#include <lib/interval.h>
#include <lib/pascal_id.h>
#include <lib/seginfo.h>
#include <lib/segment/concrete.h>
#include <lib/sizeof.h>


codefile_file_ii_1::~codefile_file_ii_1()
{
}


codefile_file_ii_1::codefile_file_ii_1(const rcstring &a_filename) :
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
    // Figure out the byte sex of the file.
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
            "the %s file does not look like a UCSD p-System II.1 codefile, "
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
                DEBUG(2, "segname[%d] failed zero test", j);
                goto yuck;
            }
            unsigned short codeaddr = get_word(buffer.diskinfo[j][0]);
            if (codeaddr != 0)
            {
                DEBUG(2, "codeaddr[%d] failed zero test", j);
                goto yuck;
            }
            unsigned short codeleng = get_word(buffer.diskinfo[j][1]);
            if (codeleng != 0)
            {
                DEBUG(2, "codeleng[%d] failed zero test", j);
                goto yuck;
            }
            unsigned short segkind = get_word(buffer.segkind[j]);
            if (segkind != 0)
            {
                DEBUG(2, "segkind[%d] failed zero test", j);
                goto yuck;
            }
            unsigned short textaddr = get_word(buffer.textaddr[j]);
            if (textaddr != 0)
            {
                DEBUG(2, "textaddr[%d] failed zero test", j);
                goto yuck;
            }
            unsigned short seginfo = get_word(buffer.seginfo[j]);
            if (seginfo != 0)
            {
                DEBUG(2, "seginfo[%d] failed zero test", j);
                goto yuck;
            }
            continue;
        }
        rcstring name = rcstring(buffer.segname[j], 8).trim();

        unsigned short codeaddr = get_word(buffer.diskinfo[j][0]);
        if (codeaddr >= get_size_in_blocks())
        {
            DEBUG(1, "codeaddr[%d]=%d failed sanity test", j, codeaddr);
            goto yuck;
        }
        unsigned short codeleng = get_word(buffer.diskinfo[j][1]); // bytes
        unsigned codeleng_blocks = (codeleng + 511) >> 9;
        if (codeaddr && codeaddr + codeleng_blocks > get_size_in_blocks())
        {
            DEBUG(1, "codeleng[%d] failed size sanity test", j);
            goto yuck;
        }

        unsigned short n = get_word(buffer.segkind[j]);
        if (n >= 256)
        {
            DEBUG(1, "segkind[%d] failed sanity test", j);
            goto yuck;
        }
        segkind_t segkind = (segkind_t)n;

        if (codeleng == 0)
        {
            switch (segkind)
            {
            case UNITSEG:
            case DATASEG:
                break;

            case LINKED:
            case HOSTSEG:
            case SEGPROC:
            case SEPRTSEG:
            case UNLINKED_INTRINS:
            case LINKED_INTRINS:
            default:
                DEBUG(1, "codeleng[%d] failed non-zero test", j);
                goto yuck;
            }
        }

        unsigned short textaddr = get_word(buffer.textaddr[j]);
        if (textaddr != 0)
        {
            if (textaddr >= get_size_in_blocks())
            {
                DEBUG(1, "textaddr[%d] failed sanity test", j);
                goto yuck;
            }
        }
        unsigned short seginfo_packed = get_word(buffer.seginfo[j]);
        seginfo seginfo_unpacked(seginfo_packed);
        unsigned segnum = seginfo_unpacked.get_segnum();
        DEBUG(2, "segnum = %u", segnum);
        if (segnum >= 64)
        {
            DEBUG(2, "segnum failed sanity test");
            goto yuck;
        }
        mtype_t mtype = seginfo_unpacked.get_mtype();
        DEBUG(2, "mtype = %s", mtype_name(mtype).c_str());
        if (segnum != 0 && mtype == mtype_undefined)
        {
            DEBUG(2, "mtype failed sanity test");
            goto yuck;
        }
        segversion_t segversion = seginfo_unpacked.get_version();
        DEBUG(2, "segversion = %s", segversion_name(segversion).c_str());

        // FIXME: extract the intrinsic segment info, too.

        unsigned link_info_size_in_blocks = 0;
        if (codeaddr)
        {
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

            //
            // Figure out if there is link info after this segment,
            // and if so, how many blocks.
            //
            unsigned short code_end = codeaddr + codeleng_blocks;
            unsigned maxad = get_size_in_blocks();
            for (unsigned k = 0; k < 16; ++k)
            {
                if (j == k)
                    continue;
                unsigned short next_textaddr = get_word(buffer.textaddr[k]);
                if (next_textaddr >= code_end && next_textaddr < maxad)
                    maxad = next_textaddr;
                unsigned short next_codeaddr = get_word(buffer.diskinfo[k][0]);
                if (next_codeaddr >= code_end && next_codeaddr < maxad)
                    maxad = next_codeaddr;
            }
            if (maxad > code_end)
                link_info_size_in_blocks = maxad - code_end;
        }
        if (textaddr)
        {
            // We are going to enforce that the text blocks for a segment must
            // immediately preceed the segment's code blocks.  The file can
            // actually cope with pretty much anything, but the tools can't.
            if (textaddr >= codeaddr)
            {
                DEBUG(2, "textaddr (%d) should be before codeaddr (%d)",
                    textaddr, codeaddr);
                goto yuck;
            }
            interval seg_text_int(textaddr, codeaddr);
            if (seg_text_int * unused_blocks != seg_text_int)
            {
                DEBUG(2, "text block range is broken (text=%s vs unused=%s)",
                    seg_text_int.representation().c_str(),
                    unused_blocks.representation().c_str()
                );
                goto yuck;
            }
            unused_blocks -= seg_text_int;
        }
        segments.push_back
        (
            segment_concrete::create
            (
                this,
                codeleng,
                codeaddr,
                name,
                segkind,
                textaddr,
                link_info_size_in_blocks,
                segnum,
                mtype,
                segversion
            )
        );
    }

    //
    // Extract the copyright notice.
    //
    unsigned char n = buffer.notice[0];
    if (n < 80)
        set_copyright_notice(rcstring(buffer.notice + 1, n).trim());
}


bool
codefile_file_ii_1::candidate(const void *vblock0ptr)
{
    const block0 &buffer = *(block0 *)vblock0ptr;

    return
        (
            looks_like_valid_segment_table(buffer, big_endian)
        ||
            looks_like_valid_segment_table(buffer, little_endian)
        );
}


codefile_file_ii_1::pointer
codefile_file_ii_1::create(const rcstring &a_filename)
{
    return pointer(new codefile_file_ii_1(a_filename));
}


codefile_file_ii_1::codefile_file_ii_1(
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


codefile_file_ii_1::pointer
codefile_file_ii_1::create(const rcstring &a_filename, byte_sex_t a_byte_sex)
{
    return pointer(new codefile_file_ii_1(a_filename, a_byte_sex));
}


codefile::pointer
codefile_file_ii_1::new_file_factory(const rcstring &outfilename)
    const
{
    return create(outfilename, get_byte_sex());
}


void
codefile_file_ii_1::block0::clear(void)
{
    assert(sizeof(*this) == 512);
    memset(this, 0, sizeof(*this));
    memset(segname, ' ', sizeof(segname));
}


bool
codefile_file_ii_1::looks_like_valid_segment_table(const block0 &buffer,
    byte_sex_t byte_sex)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    // The copyright notice must fit in the available space.
    if ((unsigned char)buffer.notice[0] >= sizeof(buffer.notice))
    {
        DEBUG(2, "copyright notice has silly length");
        return false;
    }

    //
    // The filler must be zero, because the II.1 compiler sets it to
    // zero, and so do we.
    // This eliminates the codefile shapes used by other p-machine releases.
    //
    for
    (
        const unsigned char *fp = buffer.filler;
        fp < ENDOF(buffer.filler);
        ++fp
    )
    {
        if (*fp != 0)
        {
            DEBUG(2, "filler not zero");
            return false;
        }
    }

    //
    // Now check the segment dictionary proper.
    //
    const unsigned maxblock = 1 + 16 * (1 << (16 - 9));
    for (unsigned j = 0; j < 16; ++j)
    {
        DEBUG(2, "j = %u", j);
        if (buffer.segname[j][0] == ' ')
            continue;
        if (!is_valid_segment_name(buffer.segname[j]))
        {
            DEBUG(2, "segment name broken");
            return false;
        }
        rcstring name = rcstring(buffer.segname[j], 8).trim();

        unsigned short codeaddr =
            byte_sex_get_word(byte_sex, buffer.diskinfo[j][0]);
        if (codeaddr >= maxblock)
        {
            DEBUG(1, "codeaddr[%d]=%d failed sanity test", j, codeaddr);
            return false;
        }
        unsigned short codeleng =
            byte_sex_get_word(byte_sex, buffer.diskinfo[j][1]);
        unsigned codeleng_blocks = (codeleng + 511) >> 9;
        if (codeaddr && codeaddr + codeleng_blocks > maxblock)
        {
            DEBUG(1, "codeleng[%d] failed size sanity test", j);
            return false;
        }

        unsigned short n = byte_sex_get_word(byte_sex, buffer.segkind[j]);
        if (n >= 256)
        {
            DEBUG(1, "segkind[%d] failed endian test", j);
            return false;
        }
        segkind_t segkind = (segkind_t)n;

        if (codeleng == 0)
        {
            switch (segkind)
            {
            case UNITSEG:
            case DATASEG:
                break;

            case LINKED:
            case HOSTSEG:
            case SEGPROC:
            case SEPRTSEG:
            case UNLINKED_INTRINS:
            case LINKED_INTRINS:
            default:
                DEBUG(1, "codeleng[%d] failed non-zero test", j);
                return false;
            }
        }

        unsigned short textaddr =
            byte_sex_get_word(byte_sex, buffer.textaddr[j]);
        if (textaddr > 0 && textaddr >= maxblock)
        {
            DEBUG(1, "textaddr[%d] failed sanity test", j);
            return false;
        }
        if (textaddr > 0 && codeaddr > 0 && textaddr >= codeaddr)
        {
            DEBUG(2, "textaddr %d >= codeaddr %d", textaddr, codeaddr);
            return false;
        }
        unsigned short seginfo_packed =
            byte_sex_get_word(byte_sex, buffer.seginfo[j]);
        seginfo seginfo_unpacked(seginfo_packed);
        if (seginfo_unpacked.get_segnum() >= 64)
        {
            DEBUG(2, "seginfo failed sanity test");
            return false;
        }
    }

    //
    // couldn't find anything obviously wrong
    //
    return true;
}


segment::pointer
codefile_file_ii_1::new_segment(const rcstring &name, int segnum,
    segkind_t segkind, mtype_t mtype, segversion_t version)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "segnum = %d", segnum);
    DEBUG(2, "segkind = %s", segkind_name(segkind).c_str());
    DEBUG(2, "mtype = %s", mtype_name(mtype).c_str());
    DEBUG(2, "version = %s", segversion_name(version).c_str());
    assert(segnum >= 0);
    assert(segnum < 64);

    unsigned codeleng = 0;
    unsigned codeaddr = 0;
    unsigned textaddr = 0;
    unsigned linkinfo_nblocks = 0;

    segment::pointer sp =
        segment_concrete::create
        (
            this,
            codeleng,
            codeaddr,
            pascal_id(name),
            segkind,
            textaddr,
            linkinfo_nblocks,
            segnum,
            mtype,
            version
        );

    //
    // Insert the new segment into the segment dictionary.
    //
    segments.push_back(sp);
    DEBUG(2, "return %p", sp.get());
    DEBUG(1, "}");
    return sp;
}


void
codefile_file_ii_1::write_segment_dictionary(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    block0 block;
    assert(sizeof(block) == sizeof_block);
    block.clear();

    if (segments.size() > 16)
    {
        explain_output_error_and_die
        (
            "the %s file is unable to cope with %ld segments, "
                "the II.1 format can only store 16 segments each",
            get_filename().quote_c().c_str(),
            (long)segments.size()
        );
    }

    //
    // Place the meta-data for each segment into the segment dictionary.
    //
    for
    (
        segments_t::const_iterator it = segments.begin();
        it != segments.end();
        ++it
    )
    {
        segment::pointer sp = *it;
        assert(sp);

        unsigned j = sp->get_segment_number();
        if (j >= 16 || block.segname[j][0] != ' ')
        {
            j &= 15;
            while (block.segname[j][0] != ' ')
                j = (j + 1) & 15;
        }
        DEBUG(2, "j = %d", j);

        unsigned codeaddr = sp->get_code_address();
        DEBUG(2, "codeaddr = %d", codeaddr);
        put_word(block.diskinfo[j][0], codeaddr);

        unsigned codeleng = sp->get_code_length();
        DEBUG(2, "codeleng = %d", codeleng);
        put_word(block.diskinfo[j][1], codeleng);

        // name
        rcstring temp = sp->get_name().substring(0, 8).upcase();
        DEBUG(2, "name = %s", temp.quote_c().c_str());
        memcpy(block.segname[j], temp.c_str(), temp.size());

        // The block with the INTERFACE text in it.
        unsigned textaddr = sp->get_text_address();
        DEBUG(2, "textaddr = %d", textaddr);
        put_word(block.textaddr[j], textaddr);

        segkind_t kind = sp->get_kind();
        DEBUG(2, "segkind = %s", segkind_name(kind).c_str());
        put_word(block.segkind[j], kind);

        DEBUG(2, "segnum = %d", sp->get_segment_number());
        DEBUG(2, "mtype = %s", mtype_name(sp->get_mtype()).c_str());
        DEBUG(2, "version = %s", segversion_name(sp->get_version()).c_str());
        seginfo seginfo_unpacked
        (
            sp->get_segment_number(),
            sp->get_mtype(),
            sp->get_version()
        );
        // Segment zero is a special case (backwards compatibility for
        // an ancient boot loader?)
        unsigned short seginfo_packed =
            (sp->get_segment_number() ? seginfo_unpacked.pack() : 0);
        DEBUG(2, "seginfo = 0x%04X", seginfo_packed);
        put_word(block.seginfo[j], seginfo_packed);
    }

    // FIXME: Plus the intrinsic segment bitmap.

    //
    // Insert the copyright notice.
    //
    assert(sizeof(block.notice) <= 256);
    rcstring temp = get_copyright_notice();
    size_t temp_len = temp.size();
    if (temp_len > sizeof(block.notice) - 1)
        temp_len = sizeof(block.notice) - 1;
    block.notice[0] = temp_len;
    memcpy(block.notice + 1, temp.c_str(), temp_len);

    //
    // Actually write the data to the file.
    //
    write_block(0, &block, 1);
    DEBUG(1, "}");
}


segment::pointer
codefile_file_ii_1::get_segment(unsigned number)
    const
{
    if (number >= segments.size())
        return segment::pointer();
    return segments[number];
}


void
codefile_file_ii_1::create_dataseg(const rcstring &name, int segnum, int nwords)
{
    assert(nwords > 0);
    unsigned codeleng = nwords * 2;
    unsigned codeaddr = 0;
    segkind_t segkind = DATASEG;
    unsigned textaddr = 0;
    unsigned linkinfo_nblocks = 0;
    assert(segnum >= 0);
    assert(segnum < 64);
    mtype_t mtype = mtype_pcode_le; // FIXME
    segversion_t version = segversion_3_0; // FIXME
    segments.push_back
    (
        segment_concrete::create
        (
            this,
            codeleng,
            codeaddr,
            pascal_id(name),
            segkind,
            textaddr,
            linkinfo_nblocks,
            segnum,
            mtype,
            version
        )
    );
}


unsigned
codefile_file_ii_1::maximum_number_of_segments(void)
    const
{
    return 64;
}


disassembler::pointer
codefile_file_ii_1::native_disassembler_factory(mtype_t mt, dislabel &where,
    discolumns &report, int seg_num, int proc_num, int proc_base,
    code_source_map *listing) const
{
    //
    // It is tempting to simply say get_byte_sex(), but that ignores the
    // (admittedly daft) possibility that there is a little-endian code
    // segment in a big-endian codefile, or vice versa.
    //
    byte_sex_t bs =
        (mt == mtype_undefined ? get_byte_sex() : byte_sex_from_mtype(mt));

    return
        disassembler_pcode_ii_1::create
        (
            bs,
            where,
            report,
            seg_num,
            proc_num,
            proc_base,
            listing
        );
}


// vim: set ts=8 sw=4 et :
