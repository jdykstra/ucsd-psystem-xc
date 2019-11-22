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

#include <lib/ac/ctype.h>
#include <lib/ac/string.h>
#include <libexplain/output.h>

#include <lib/codefile.h>
#include <lib/debug.h>
#include <lib/pascal_id.h>
#include <lib/rcstring/list.h>
#include <lib/segment/concrete.h>


segment_concrete::~segment_concrete()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


segment_concrete::segment_concrete(
    codefile *a_parent,
    unsigned a_codeleng,
    unsigned a_codeaddr,
    const rcstring &a_name,
    segkind_t a_segkind,
    unsigned a_textstart,
    unsigned a_linkinfo_nblocks,
    unsigned a_segnum,
    mtype_t a_mtype,
    segversion_t a_version
) :
    parent(a_parent),
    codeleng(a_codeleng),
    codeaddr(a_codeaddr),
    name(a_name),
    segkind(a_segkind),
    textstart(a_textstart),
    linkinfo_nblocks(a_linkinfo_nblocks),
    segnum(a_segnum),
    mtype(a_mtype),
    version(a_version)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "codeleng = %d", codeleng);
    DEBUG(2, "codeaddr = %d", codeaddr);
    DEBUG(2, "name = %s", name.quote_c().c_str());
    DEBUG(2, "segkind = %s", segkind_name(segkind).c_str());
    DEBUG(2, "textstart = %d", textstart);
    assert(textstart == 0 || textstart < codeaddr);

    // Read all of the link information
    // FIXME: do this on demand
    if (codeaddr)
        read_link_information();
}


segment_concrete::pointer
segment_concrete::create(codefile *a_parent, unsigned a_codeleng,
    unsigned a_codeaddr, const rcstring &a_name, segkind_t a_segkind,
    unsigned a_textstart, unsigned a_linkinfo_nblocks, unsigned a_segnum,
    mtype_t a_mtype,
    segversion_t a_version)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return
        pointer
        (
            new segment_concrete
            (
                a_parent,
                a_codeleng,
                a_codeaddr,
                a_name,
                a_segkind,
                a_textstart,
                a_linkinfo_nblocks,
                a_segnum,
                a_mtype,
                a_version
            )
        );
}


void
segment_concrete::read_link_information(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "segkind = %s", segkind_name(segkind).c_str());
    switch (segkind)
    {
    case DATASEG:
    case LINKED:
    case LINKED_INTRINS:
        if (linkinfo_nblocks != 0)
        {
            explain_output_warning
            (
                "codefile %s, segment %s, %s: should have no link info, "
                    "but it has %d block%s",
                parent->get_filename().quote_c().c_str(),
                get_name().quote_c().c_str(),
                segkind_name(segkind).c_str(),
                linkinfo_nblocks,
                (linkinfo_nblocks == 1 ? "" : "s")
            );
        }
        return;

    case HOSTSEG:
    case SEGPROC:
    case SEPRTSEG:
    case UNITSEG:
    case UNLINKED_INTRINS:
        if (linkinfo_nblocks == 0)
        {
            explain_output_warning
            (
                "codefile %s, segment %s, %s: should have link info, "
                    "but it does not",
                parent->get_filename().quote_c().c_str(),
                get_name().quote_c().c_str(),
                segkind_name(segkind).c_str()
            );
            return;
        }
        break;
    }

    unsigned recsleft = 0;
    unsigned nextblk = codeaddr + (codeleng + 511) / 512;
    unsigned char block[512];
    for (;;)
    {
        if (recsleft == 0)
        {
            parent->read_block(nextblk, block);
            recsleft = 32;
            ++nextblk;
        }
        const unsigned char *data = block + (32 - recsleft) * 16;
        --recsleft;

        // format of link info records
        //
        //    +---------------+---------------+
        //  0 |                               |
        //    +--                           --+
        //  2 |                               |
        //    +--           name            --+
        //  4 |                               |
        //    +--                           --+
        //  6 |                               |
        //    +---------------+---------------+
        //  8 |        link info type         |
        //    +---------------+---------------+
        // 10 |             word5             |
        //    +---------------+---------------+
        // 12 |             word6             |
        //    +---------------+---------------+
        // 14 |             word7             |
        //    +---------------+---------------+
        //
        // But: if link info type is GLOBREF, PUBLREF, CONSTREF, SEPFREF,
        // SEPPREF, UNITREF or PRIVREF then the next ((word6+7)&~7)
        // words are reference offsets.
        //
        rcstring liname = rcstring((const char *)data, 8).trim();
        link_info_type_t word4 = (link_info_type_t)get_word(data + 8);
        DEBUG(2, "%s, litype=%d", liname.quote_c().c_str(), word4);
        int word5 = get_word(data + 10);
        int word6 = get_word(data + 12);
        int word7 = get_word(data + 14);

        link_info::pointer lip =
            link_info::create(liname, word4, word5, word6, word7);
        switch (word4)
        {
        case EOFMARK:
            // // word5 is "nextlc"
            // assert(word5 > 0);
            break;

        case GLOBDEF:
        case PUBLDEF:
        case CONSTDEF:
        case EXTPROC:
        case EXTFUNC:
        case SEPPROC:
        case SEPFUNC:
            break;

        case GLOBREF:
        case PUBLREF:
        case CONSTREF:
        case SEPFREF:
        case SEPPREF:
        case UNITREF:
        case PRIVREF:
            {
                unsigned nrefs = word6;
                unsigned nrefs8 = (word6 + 7) & ~7;
                unsigned nentries = nrefs8 >> 3;
                for (unsigned j = 0; j < nentries; ++j)
                {
                    if (recsleft == 0)
                    {
                        parent->read_block(nextblk, block);
                        recsleft = 32;
                        ++nextblk;
                    }
                    data = block + (32 - recsleft) * 16;
                    --recsleft;

                    for (unsigned k = 0; k < 8 && j * 8 + k < nrefs; ++k)
                        lip->add_reference(get_word(data + 2 * k));
                }
            }
        }

        link_infos.push_back(lip);

        if (word4 == EOFMARK)
            break;
    }
}


void
segment_concrete::write_link_information(int nextlc)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(nextlc > 0);
    assert(nextlc < 32768);
    switch (segkind)
    {
    case DATASEG:
    case LINKED:
    case LINKED_INTRINS:
        DEBUG(1, "not writing link info");
        return;

    case HOSTSEG:
    case SEGPROC:
    case SEPRTSEG:
    case UNITSEG:
    case UNLINKED_INTRINS:
        break;
    }

    DEBUG(1, "write_link_info");
    link_info_list temp(link_infos);

    if (!temp.ends_with_eofmark())
    {
        // terminate the list
        DEBUG(2, "adding EOFMARK");
        temp.push_back(link_info::create("", EOFMARK, nextlc, 0, 0));
    }

    size_t nbytes = temp.serialize_size();
    DEBUG(2, "nbytes = %d", (int)nbytes);
    size_t nblocks = (nbytes + 511) >> 9;
    assert(nblocks > 0);
    size_t nbytes2 = nblocks << 9;
    unsigned char *data = new unsigned char [nbytes2];

    temp.serialize(data, parent);
    memset(data + nbytes, 0, nbytes2 - nbytes);

    unsigned link_info_block = codeaddr + ((codeleng + 511) >> 9);
    DEBUG(2, "link_info_block = %u", link_info_block);
    unsigned where = parent->allocate(nblocks);
    assert(where == link_info_block);
    parent->write_block(link_info_block, data, nblocks);
    delete [] data;
    DEBUG(2, "write_link_info %d blocks written", (int)nblocks);
}


byte_sex_t
segment_concrete::get_byte_sex(void)
    const
{
    return parent->get_byte_sex();
}


void
segment_concrete::segment_not_linked(const link_info::pointer &lip)
{
    switch (segkind)
    {
    case LINKED:
    case HOSTSEG:
        segkind = HOSTSEG;
        break;

    case SEGPROC:
        // In theory, can't happen.
        assert(!"grammar was supposed to prevent this");
        break;

    case UNITSEG:
        // Non-intrinsic unit.
        //
        // Non-intrinsic units have the same segkind when they have
        // external procedures and when they don't.  I wonder why?  It
        // seems very inconsistent.
        //
        // Also, linked or unlinked, they always have link_info, even if
        // it is only EOFMARK in the linked case.
        //
        break;

    case SEPRTSEG:
        // This is what the assembler produces,
        // unlinked by definition.
        break;

    case UNLINKED_INTRINS:
    case LINKED_INTRINS:
        // Intrinsic unit.
        segkind = UNLINKED_INTRINS;
        break;

    case DATASEG:
        break;
    }
    if (lip)
        add_link_information(lip);
}


void
segment_concrete::add_link_information(const link_info::pointer &lip)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    //
    // Remember the link info, we will write to the block(s) after the
    // segment_concrete code block(s).
    //
    assert(lip);

    //
    // It may be possible to collapse two reference lists into one.
    //
    if (lip->has_references())
    {
        if (link_infos.extendable(lip))
            return;
    }

    // It's a new one
    link_infos.push_back(lip);
}


unsigned
segment_concrete::get_code_address(unsigned code_length)
{
    assert(code_length > 0);
    assert((code_length & 1) == 0);
    assert(code_length < (1uL << 16));

    codeleng = code_length;
    unsigned nblocks = (codeleng + 511) >> 9;
    codeaddr = parent->allocate(nblocks);
    return codeaddr;
}


static const char DLE = 0x10;


rcstring
segment_concrete::get_interface_text(void)
    const
{
    assert(textstart < codeaddr);
    return get_interface_text(textstart, codeaddr - textstart);
}


static rcstring
convert_cr2nl(const unsigned char *data, size_t data_size)
{
    rcstring_accumulator buffer;
    int prev_white_space = 0;
    const unsigned char *cp = data;
    const unsigned char *end = data + data_size;
    while (cp < end)
    {
        unsigned char c = *cp++;
        switch (c)
        {
        case '\0':
            cp = data + (((cp - data) + 1023) & ~1023);
            break;

        case DLE:
            prev_white_space += *cp - ' ';
            ++cp;
            break;

        case ' ':
        case '\b':
        case '\f':
        case '\t':
        case '\v':
            ++prev_white_space;
            break;

        case '\r':
        case '\n':
            if (!buffer.empty())
                buffer.push_back('\n');
            prev_white_space = 0;
            break;

        default:
            while (prev_white_space > 0)
            {
                --prev_white_space;
                buffer.push_back(' ');
            }
            buffer.push_back(c);
            break;
        }
    }
    if (!buffer.empty() && buffer.back() != '\n')
        buffer.push_back('\n');
    return buffer.mkstr();
}


static size_t
look_for_implementation(const rcstring &s)
{
    const char *cp = s.c_str();
    const char *end = cp + s.size();
    while (cp < end)
    {
        size_t start_of_token = cp - s.c_str();
        unsigned char c = *cp++;
        switch (c)
        {
        case '(':
            if (*cp != '*')
                break;
            // Comment
            for (;;)
            {
                if (cp >= end)
                    break;
                c = *cp++;
                if (c == '*')
                {
                    if (cp < end && *cp == ')')
                    {
                        ++cp;
                        break;
                    }
                }
            }
            break;

        case '{':
            // Comment
            for (;;)
            {
                if (cp >= end)
                    break;
                c = *cp++;
                if (*cp == '}')
                    break;
            }
            break;

        case '\'':
            // String constant.
            for (;;)
            {
                c = *cp++;
                if (c == '\'')
                {
                    if (cp < end && *cp == '\'')
                        ++cp;
                    else
                        break;
                }
            }
            break;

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            {
                // identifier
                rcstring_accumulator ac;
                for (;;)
                {
                    ac.push_back(c);
                    if (cp >= end)
                        break;
                    c = *cp++;
                    switch (c)
                    {
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                    case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                    case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                    case 'y': case 'z':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                    case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                    case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                    case 'Y': case 'Z':
                    case '_':
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                        continue;

                    default:
                        --cp;
                        break;
                    }
                    break;
                }
                rcstring name = ac.mkstr();
                if (pascal_id(name) == pascal_id("IMPLEMENTATION"))
                    return start_of_token;
            }
            break;

        default:
            break;
        }
    }
    return 0;
}


rcstring
segment_concrete::get_interface_text(unsigned block_num, int size_in_blocks)
    const
{
    if (block_num == 0)
        return "";

    //
    // Read in text blocks until we see "IMPLEMENTATION".
    //
    size_t size_in_bytes = size_in_blocks * 512;
    unsigned char *data = new unsigned char [size_in_bytes];
    parent->read_block(block_num, data, size_in_blocks);

    rcstring text = convert_cr2nl(data, size_in_bytes);
    delete [] data;
    data = 0;

    size_t n = look_for_implementation(text);
    if (!n)
    {
        // looks broken to me
        return text;
    }

    while (n > 0 && isspace((unsigned char)text[n - 1]))
        --n;
    return text.substring(0, n);
}


//
// Some early versions of the UCSD native compiler had a nasty bug.
// This bug caused them to be unable to cope with text blocks that had
// zero bytes of NUL padding on the end.
//
#define COPE_WITH_COMPILER_BUG


static void
convert_nl2cr(const rcstring &s, unsigned char *&data, size_t &data_size)
{
    if (s.empty())
    {
        data = 0;
        data_size = 0;
        return;
    }

    //
    // Split the string into a list, one string per line.  Encode the DLE
    // whitespace compression, and change the line termionation, as we go.
    //
    const char *cp = s.c_str();
    const char *end = cp + s.size();
    rcstring_accumulator line_buf;
    rcstring_list lines;
    while (cp < end)
    {
        line_buf.clear();
        bool start_of_line = true;
        int white_space = 0;
        for (;;)
        {
            unsigned char c = *cp++;
            switch (c)
            {
            case '\0':
                if (!line_buf.empty())
                {
                    line_buf.push_back('\r');
                    lines.push_back(line_buf.mkstr());
                }
                break;

            case ' ':
            case '\b':
            case '\f':
            case '\t':
            case '\v':
                ++white_space;
                continue;

            case '\r':
            case '\n':
                line_buf.push_back('\r');
                lines.push_back(line_buf.mkstr());
                break;

            default:
                if (start_of_line)
                {
                    switch (white_space)
                    {
                    case 0:
                        break;

                    case 2:
                        line_buf.push_back(' ');
                        // fall through...

                    case 1:
                        line_buf.push_back(' ');
                        break;

                    default:
                        {
                            int max = '~' - ' ' + 1;
                            if (white_space > max)
                                white_space = max;
                            line_buf.push_back(DLE);
                            char len = ' ' + white_space;
                            line_buf.push_back(len);
                        }
                        break;
                    }
                }
                else
                {
                    // We could insert DLE here, too, excpt that isn't
                    // what the UCSD system does.  It only uses DLE at
                    // the start of a line, despite the fact that file
                    // I/O doesn't care where DLE occurs within a line.
                    while (white_space > 0)
                    {
                        line_buf.push_back(' ');
                        --white_space;
                    }
                }
                line_buf.push_back(c);
                white_space = 0;
                start_of_line = false;
                continue;
            }
            break;
        }
    }

    // The ten space characters following the IMPLEMENTATIOn keyword are
    // mandated by the UCSD codefile spec.
    lines.push_back("IMPLEMENTATION          \r");

    //
    // Now we have each line separated and encoded, see how many bytes
    // that will be.
    //
    size_t pos = 0;
    for (size_t j = 0; j < lines.size(); ++j)
    {
        size_t upper_limit = (pos + 1024) & ~1023;
        rcstring line = lines[j];
#ifdef COPE_WITH_COMPILER_BUG
        if (pos + line.size() >= upper_limit)
#else
        if (pos + line.size() > upper_limit)
#endif
            pos = upper_limit;
        pos += line.size();
    }

    //
    // Allocate the space
    //
    // (Yes, there are times we allocated 512 bytes more than we use, it
    // makes the code easier to understand.  This storage is transient.
    // It is not a memory leak.)
    //
    data_size = (pos + 511) & ~511;
    size_t nblocks = ((pos + 1023) & ~1023) >> 9;
    data = new unsigned char [nblocks << 9];

    //
    // Build the data to be written to the codefile.
    //
    pos = 0;
    for (size_t j = 0; j < lines.size(); ++j)
    {
        size_t upper_limit = (pos + 1024) & ~1023;
        rcstring line = lines[j];
#ifdef COPE_WITH_COMPILER_BUG
        if (pos + line.size() >= upper_limit)
#else
        if (pos + line.size() > upper_limit)
#endif
        {
            memset(data + pos, 0, upper_limit - pos);
            pos = upper_limit;
        }
        memcpy(data + pos, line.c_str(), line.size());
        pos += line.size();
    }
    if (pos < data_size)
        memset(data + pos, 0, data_size - pos);
}


unsigned
segment_concrete::get_text_address(const rcstring &text)
{
    if (text.empty())
        return 0;
    assert(textstart == 0);

    // Build the text to be saved.  (The ten spaces are required, as
    // spaces, see UCSD codefile documentation.)
    unsigned char *data = 0;
    size_t data_size = 0;
    convert_nl2cr(text, data, data_size);
    assert(data);
    assert(data_size);
    assert((data_size & 511) == 0);
    size_t nblocks = data_size >> 9;

    // Allocate space in the codefile, and
    // write the data blocks to the codefile.
    textstart = parent->allocate(nblocks);
    parent->write_block(textstart, data, nblocks);
    delete [] data;

    // return the block address of the interface text
    return textstart;
}


void
segment_concrete::read_block(unsigned blknum, void *data, unsigned nblocks)
    const
{
    assert(codeaddr);
    parent->read_block(codeaddr + blknum, data, nblocks);
}


void
segment_concrete::write_block(unsigned blknum, const void *data,
    unsigned nblocks) const
{
    assert(codeaddr);
    parent->write_block(codeaddr + blknum, data, nblocks);
}


segkind_t
segment_concrete::get_kind(void)
    const
{
    return segkind;
}


void
segment_concrete::set_kind(segkind_t value)
{
    segkind = value;
}


mtype_t
segment_concrete::get_mtype(void)
    const
{
    return mtype;
}


void
segment_concrete::set_mtype(mtype_t value)
{
    mtype = value;
}


segversion_t
segment_concrete::get_version(void)
    const
{
    return version;
}


void
segment_concrete::set_version(segversion_t value)
{
    version = value;
}


unsigned
segment_concrete::get_codeleng(void)
    const
{
    return codeleng;
}


unsigned
segment_concrete::get_code_address(void)
    const
{
    return codeaddr;
}


unsigned
segment_concrete::get_segment_number(void)
    const
{
    return segnum;
}


unsigned
segment_concrete::get_text_address(void)
    const
{
    return textstart;
}


unsigned
segment_concrete::get_text_start(void)
    const
{
    return textstart;
}


unsigned
segment_concrete::get_code_length(void)
    const
{
    return codeleng;
}


const link_info_list &
segment_concrete::get_link_info(void)
    const
{
    return link_infos;
}


unsigned
segment_concrete::get_linkinfo_nblocks(void)
    const
{
    if (!link_infos.empty())
    {
        link_info_list temp(link_infos);
        if (!temp.ends_with_eofmark())
            temp.push_back(link_info::create("", EOFMARK, 0, 0, 0));
        size_t nbytes = temp.serialize_size();
        return ((nbytes + 511) >> 9);
    }
    return linkinfo_nblocks;
}


// vim: set ts=8 sw=4 et :
