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

#include <lib/ac/stdio.h>
#include <lib/ac/string.h>
#include <libexplain/output.h>

#include <lib/debug.h>
#include <lib/seginfo.h>

#include <ucsdpsys_link/link_info_entry.h>
#include <ucsdpsys_link/segrec.h>


segrec::~segrec()
{
    delete [] in_data;
    delete [] out_data;
}


segrec::segrec(
    const codefile::pointer &a_srcfile,
    const segment::pointer &a_sp
) :
    srcfile(a_srcfile),
    sp(a_sp),
    in_data(0),
    out_data(0),
    out_data_len(0),
    nextspot(0)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(srcfile);
    assert(sp);
}


segrec::pointer
segrec::create(const codefile::pointer &a_srcfile, const segment::pointer &a_sp)
{
    return pointer(new segrec(a_srcfile, a_sp));
}


int
segrec::get_codeaddr(void)
    const
{
    return sp->get_code_address();
}


int
segrec::get_codeleng(void)
    const
{
    return sp->get_code_length();
}


int
segrec::get_size_in_blocks(void)
    const
{
    return ((get_codeleng() + 511) >> 9);
}


segkind_t
segrec::get_segkind(void)
    const
{
    return sp->get_kind();
}


rcstring
segrec::get_src_filename(void)
    const
{
    return srcfile->get_filename();
}


rcstring
segrec::get_src_segname(void)
    const
{
    return sp->get_name();
}


void
segrec::symtab_insert(const link_info_entry::pointer &ep)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring key = ep->get_symbol_table_key();
    symbol_table_t::iterator it = symtab.find(key);
    if (it != symtab.end())
    {
        assert(ep->origin);
        explain_output_error
        (
            "codefile %s, segment %s: why is %s defined twice?",
            ep->origin->get_src_filename().quote_c().c_str(),
            ep->origin->get_src_segname().quote_c().c_str(),
            key.c_str()
        );
        it->second = ep;
    }
    else
    {
        symtab.insert(symbol_table_t::value_type(key, ep));
    }
}


link_info_entry::pointer
segrec::symtab_lookup(const rcstring &name, link_info_type_t oktype)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring key = link_info_entry::get_symbol_table_key(name, oktype);
    symbol_table_t::iterator it = symtab.find(key);
    if (it == symtab.end())
        return link_info_entry::pointer();
    return it->second;
}


unsigned short
segrec::fetch_word(const unsigned char *buf)
    const
{
    return srcfile->get_word(buf);
}


const unsigned char *
segrec::get_data(void)
    const
{
    if (!in_data)
    {
        unsigned nbytes = sp->get_code_length();
        unsigned nblocks = (nbytes + 511) >> 9;
        nblocks += sp->get_linkinfo_nblocks();
        in_data = new unsigned char [nblocks << 9];
        sp->read_block(0, in_data, nblocks);
    }
    return in_data;
}


int
segrec::get_length(void)
    const
{
    return sp->get_code_length();
}


int
segrec::get_link_info_block(void)
    const
{
    int nblocks = (get_codeleng() + 511) >> 9;
    return get_codeaddr() + nblocks;
}


int
segrec::get_seginfo_packed(void)
    const
{
    int segnum = sp->get_segment_number();
    mtype_t mtype = sp->get_mtype();
    segversion_t version = sp->get_version();
    seginfo si(segnum, mtype, version);
    return si.pack();
}


int
segrec::get_segment_number(void)
    const
{
    return sp->get_segment_number();
}


int
segrec::get_textstart(void)
    const
{
    return sp->get_text_address();
}


void
segrec::set_srcfile(const codefile::pointer &a_srcfile, int segnum)
{
    assert(get_codeleng() == 0);
    srcfile = a_srcfile;
    sp = srcfile->get_segment_by_number(segnum);
    assert(sp);
}


mtype_t
segrec::get_mtype(void)
    const
{
    return sp->get_mtype();
}


void
segrec::set_mtype(mtype_t value)
{
    if (sp->get_mtype() < value)
    {
        segversion_t version = sp->get_version();
        if (version < segversion_3_0)
            sp->set_version(segversion_3_0);
        sp->set_mtype(value);
    }
}


void
segrec::read_and_split(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(out_data_len > 0);
    assert((out_data_len & 1) == 0);
    int nblocks = (out_data_len + 511) >> 9;
    size_t nbytes = nblocks << 9;
    out_data = new unsigned char [nbytes];
    memset(out_data, 0, nbytes);

    nextspot = 0;
    unsigned codeleng = sp->get_code_length();
    assert((codeleng & 1) == 0);
    assert(codeleng <= out_data_len);
    memcpy(out_data, get_data(), codeleng);

    // move the procedure dictionary to the end of the segment
    int pddelta = out_data_len - codeleng;
    DEBUG(2, "pddelta = %d", pddelta);
    assert(pddelta >= 0);
    int nprocs = out_data[codeleng - 1];
    int pdleng = nprocs * 2 + 2;
    nextspot = codeleng - pdleng;
    if (pddelta > 0)
    {
        unsigned char *src = out_data + codeleng - pdleng;
        unsigned char *dst = out_data + out_data_len - pdleng;
        memmove(dst, src, pdleng);
        memset(src, 0, dst - src);

        // repair jump table
        for (int j = 1; j <= nprocs; ++j)
        {
            unsigned char *p = out_data + out_data_len - 2 * j - 2;
            int joff = sp->get_word(p);
            joff += pddelta;
            DEBUG(2, "proc %d, joff %d", j, joff);
            sp->put_word(p, joff);
        }
    }
}


void
segrec::read_source_segment(const workrec_list_t &procs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(sp);
    unsigned orgleng = sp->get_code_length();

    //
    // Figure out how much bigger the segment will become.
    //
    int addleng = 0;
    int addprocs = 0;
    for
    (
        workrec_list_t::const_iterator it = procs.begin();
        it != procs.end();
        ++it
    )
    {
        workrec::pointer wp = *it;
        addleng += wp->defsym->place.length;
        if (wp->newproc == 0)
            ++addprocs;
    }

    //
    // Allocate space that size, and copy in the origin segment,
    // splitting the procedure dictionary from the code.
    //
    out_data_len = orgleng + addleng + 2 * addprocs;
    assert(out_data_len < (1u << 15));
    read_and_split();
    assert(out_data);

    //
    // Run down the work list, figuring out where each additional
    // procedure is to be placed into the new segment image.
    //
    int last = out_data[out_data_len - 1];
    for
    (
        workrec_list_t::const_iterator it = procs.begin();
        it != procs.end();
        ++it
    )
    {
        workrec::pointer wp = *it;
        wp->defsym->place.destbase = nextspot;
        nextspot += wp->defsym->place.length;

        if (wp->newproc == 0)
        {
            ++last;
            if (last > 255)
            {
                explain_output_error_and_die
                (
                    "codefile %s, segment %s: procedure number overflow",
                    wp->defsym->origin->get_src_filename().quote_c().c_str(),
                    wp->defsym->origin->get_src_segname().quote_c().c_str()
                );
                last = 1;
            }
            wp->newproc = last;
        }

        // FIXME: why don't we memcpy them at this time?
    }

    // repair the number of procedures
    out_data[out_data_len - 1] = last;

    // repair the segment number
    out_data[out_data_len - 2] = sp->get_segment_number();
}


void
segrec::write_to_codefile(const codefile::pointer &cfp)
{
    segkind_t sk = sp->get_kind();
    switch (sk)
    {
    case HOSTSEG:
        sk = LINKED;
        break;

    case UNLINKED_INTRINS:
        sk = LINKED_INTRINS;
        break;

    case LINKED:
    case SEGPROC:
    case UNITSEG:
    case SEPRTSEG:
    case LINKED_INTRINS:
    case DATASEG:
        break;
    }
    segment::pointer nsp =
        cfp->new_segment
        (
            sp->get_name(),
            get_segment_number(),
            sk,
            sp->get_mtype(),
            sp->get_version()
        );
    nsp->get_text_address(sp->get_interface_text());
    nsp->get_code_address(out_data_len);
    int out_data_nblocks = (out_data_len + 511) >> 9;
    nsp->write_block(0, out_data, out_data_nblocks);
    // FIXME: xfer linkage
    nsp->write_link_information(1);
}


void
segrec::fix_up_references(workrec_list_t &works, bool fix_all_refs,
    const workrec_list_t &procs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for (workrec_list_t::iterator it = works.begin(); it != works.end(); ++it)
    {
        workrec::pointer work = *it;
        int val = 0;
        switch (work->refsym->get_litype())
        {
        case CONSTDEF:
        case EOFMARK:
        case EXTFUNC:
        case EXTPROC:
        case GLOBDEF:
        case PUBLDEF:
        case SEPFUNC:
        case SEPPROC:
            continue;

        case UNITREF:
            val = work->defsegnum;
            break;

        case GLOBREF:
            val = work->defsym->get_icoffset() +
                work->defproc->defsym->place.destbase;
            break;

        case PUBLREF:
        case PRIVREF:
            if (work->refsym->get_litype() == PRIVREF)
                val = work->newoffset;
            else
                val = work->defsym->get_baseoffset();
            if (work->refsym->get_format() == opformat_word)
            {
                // mark stack size for pub/priv fixup
                const int msdelta = 12;
                val = (val - 1) * 2 + msdelta;
            }
            else if (val < 0 || val >= 65536)
            {
                explain_output_error_and_die
                (
                    "codefile %s, segment %s: address overflow (%d)",
                    work->defsym->origin->get_src_filename().quote_c().c_str(),
                    work->defsym->origin->get_src_segname().quote_c().c_str(),
                    val
                );
            }
            break;

        case CONSTREF:
            val = work->defsym->get_constval();
            break;

        case SEPPREF:
        case SEPFREF:
            val = work->defproc->newproc;
            break;
        }
        const link_info::references_t &rr = work->refsym->get_references();
        for
        (
            link_info::references_t::const_iterator it3 = rr.begin();
            it3 != rr.end();
            ++it3
        )
        {
            int ref = *it3;
            bool skipit = !fix_all_refs;
            if (skipit)
            {
                // If fix_all_refs is true, then all pointers in the ref
                // lists are used, otherwise the reference pointers are
                // checked to see if they occur in the procedures to be
                // linked.
                for
                (
                    workrec_list_t::const_iterator it2 = procs.begin();
                    it2 != procs.end();
                    ++it2
                )
                {
                    workrec::pointer wp = *it2;
                    if (wp->defseg == work->refseg)
                    {
                        int src_base = wp->defsym->place.srcbase;
                        int src_end = src_base + wp->defsym->place.length;
                        if (src_base <= ref && ref < src_end)
                        {
                            ref = ref - wp->defsym->place.srcbase
                                + wp->defsym->place.destbase;
                            skipit = false;
                            break;
                        }
                    }
                }
            }
            if (!skipit)
            {
                switch (work->refsym->get_format())
                {
                case opformat_word:
                    {
                        short offset = sp->get_word(out_data + ref);
                        sp->put_word(out_data + ref, val + offset);
                    }
                    break;

                case opformat_byte:
                    out_data[ref] = val;
                    break;

                case opformat_big:
                    out_data[ref] = 0x80 | (val >> 8);
                    out_data[ref + 1] = val;
                    break;
                }
            }
        }
    }
}


void
segrec::copy_in_procedures(const workrec_list_t &procs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for
    (
        workrec_list_t::const_iterator it = procs.begin();
        it != procs.end();
        ++it
    )
    {
        workrec::pointer wp = *it;
        assert(wp);
        DEBUG(2, "%s", wp->representation().c_str());
#if 0
        if (verbose)
        {
            const char *caption = "function";
            if (wp->defsym->get_litype() == SEPPROC)
                caption = "procedure";
            printf
            (
                "   Copying %s %s\n",
                caption,
                wp->defsym->get_name().quote_c().c_str()
            );
        }
#endif
        sp->set_mtype(wp->defseg->get_mtype());

        const unsigned char *sepbase = wp->defseg->get_data();

        DEBUG(2, "destbase = %d", wp->defsym->place.destbase);
        unsigned char *dst = out_data + wp->defsym->place.destbase;
        DEBUG(2, "srcbase = %d", wp->defsym->place.srcbase);
        const unsigned char *src = sepbase + wp->defsym->place.srcbase;
        DEBUG(2, "length = %d", wp->defsym->place.length);
        memcpy(dst, src, wp->defsym->place.length);

        //
        // Now we update the procedure number in the procedure's attributes,
        // but if it is an assembly procedure, leave it zero.
        //
        unsigned char *jtab = dst + wp->defsym->place.length - 2;
        DEBUG(2, "old proc num = %d", *jtab);
        if (*jtab != 0)
        {
            *jtab = wp->newproc;
            DEBUG(2, "new proc num = %d", *jtab);
        }
        unsigned char *pdp = out_data + out_data_len - 2 * wp->newproc - 2;
        assert(pdp > jtab);
        sp->put_word(pdp, pdp - jtab);
    }
}


void
segrec::write_map(FILE *map, const workrec_list_t &procs,
    const workrec_list_t &local, const workrec_list_t &other) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(sp);
    fprintf
    (
        map,
        "\nSegment %d, %s:\n",
        sp->get_segment_number(),
        get_src_segname().quote_c().c_str()
    );

    if (!procs.empty())
    {
        fprintf(map, "   Separate procedures:\n");
        for
        (
            workrec_list_t::const_iterator it = procs.begin();
            it != procs.end();
            ++it
        )
        {
            workrec::pointer wp = *it;
            fprintf
            (
                map,
                "      %-10s",
                wp->defsym->get_name().quote_c().c_str()
            );
            if (wp->defsym->get_litype() == SEPPROC)
                fprintf(map, " proc");
            else
                fprintf(map, " func");
            fprintf(map, " %3d, ", wp->newproc);
            fprintf(map, "base = %5d, ", wp->defsym->place.destbase);
            fprintf(map, "leng = %4d\n", wp->defsym->place.length);
        }
    }

    if (!local.empty())
    {
        fprintf(map, "   Local segment references:\n");
        for
        (
            workrec_list_t::const_iterator it = local.begin();
            it != local.end();
            ++it
        )
        {
            workrec::pointer wp = *it;
            fprintf
            (
                map,
                "      %-11s",
                (wp->defsym->get_name().quote_c() + ",").c_str()
            );
            switch (wp->defsym->get_litype())
            {
            case UNITREF:
                fprintf(map, " unit segn = %d", wp->defsegnum);
                break;

            case PRIVREF:
                fprintf(map, " privatelc = %d", wp->newoffset);
                break;

            case GLOBDEF:
                fprintf
                (
                    map,
                    " glob def in %s at %d",
                    wp->defproc->defsym->get_name().quote_c().c_str(),
                    wp->defsym->get_icoffset()
                );
                break;

            case PUBLDEF:
                fprintf(map, " public LC = %d", wp->defsym->get_baseoffset());
                break;

            case CONSTDEF:
                fprintf(map, " const val = %d", wp->defsym->get_constval());
                break;

            case CONSTREF:
            case EOFMARK:
            case EXTFUNC:
            case EXTPROC:
            case GLOBREF:
            case PUBLREF:
            case SEPFREF:
            case SEPFUNC:
            case SEPPREF:
            case SEPPROC:
            default:
                break;
            }
            fprintf(map, "\n");
        }
    }

    if (!other.empty())
    {
        fprintf(map, "   Separate procedure references:\n");
        for
        (
            workrec_list_t::const_iterator it = other.begin();
            it != other.end();
            ++it
        )
        {
            workrec::pointer wp = *it;
            fprintf
            (
                map,
                "      %-11s",
                (wp->defsym->get_name().quote_c() + ",").c_str()
            );
            switch (wp->defsym->get_litype())
            {
            case UNITREF:
                fprintf(map, " unit segn = %d", wp->defsegnum);
                break;

            case PRIVREF:
                fprintf(map, " privat LC = %d", wp->newoffset);
                break;

            case GLOBDEF:
                fprintf
                (
                    map,
                    " glob def in %s at %d",
                    wp->defproc->defsym->get_name().quote_c().c_str(),
                    wp->defsym->get_icoffset()
                );
                break;

            case PUBLDEF:
                fprintf(map, " public LC = %d", wp->defsym->get_baseoffset());
                break;

            case CONSTDEF:
                fprintf(map, " const val = %d", wp->defsym->get_constval());
                break;

            case CONSTREF:
            case EOFMARK:
            case EXTFUNC:
            case EXTPROC:
            case GLOBREF:
            case PUBLREF:
            case SEPFREF:
            case SEPFUNC:
            case SEPPREF:
            case SEPPROC:
            default:
                break;
            }
            fprintf(map, "\n");
        }
    }
}


void
segrec::update_proc1_varsize(int nextbaselc)
{
    // Update the size of procedure one's global variables.
    // FIXME: This assumes nparams==4 (i.e. that it's a program)
    unsigned char *jtab = out_data + out_data_len - 4;
    jtab -= sp->get_word(jtab);
    assert(jtab - 8 >= out_data);
    sp->put_word(jtab - 8, nextbaselc * 2 - 6);
}


// vim: set ts=8 sw=4 et :
