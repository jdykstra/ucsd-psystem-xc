//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
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

#include <lib/ac/list>
#include <lib/ac/map>
#include <lib/ac/stdio.h>
#include <lib/ac/string.h>
#include <lib/ac/sys/stat.h>
#include <lib/ac/vector>
#include <lib/ac/getopt.h>
#include <unistd.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>

#include <lib/byte_sex.h>
#include <lib/codefile/file.h>
#include <lib/debug.h>
#include <lib/output.h>
#include <lib/p_machine.h>
#include <lib/rcstring/list.h>
#include <lib/set.h>
#include <lib/sizeof.h>
#include <lib/version.h>

#include <ucsdpsys_link/link_info_entry.h>
#include <ucsdpsys_link/segrec.h>


static const int sysprog = 4;
static const int maxseg1 = 16;
static const int master_slot = 1;
static const int firstseg = 7;
static const int maxfile = 10;
static const int maxlc = 32767;
static const int maxic = 20000;
static const int maxproc = 160;
static const int msdelta = 12;

typedef set liset;


static byte_sex_t byte_sex;


static int
fetch_word(const unsigned char *cp, int offset)
{
    return byte_sex_get_word(byte_sex, cp + offset);
}


static codefile::pointer hostfile;
typedef std::vector<codefile::pointer> libfiles_t;
static libfiles_t libfiles;
typedef std::list<segrec::pointer> segrec_list_t;
static segrec_list_t seplist;
static set reflitypes;
static bool verbose;
static segrec::pointer hostsp;
static int nextbaselc;
static int nextseg;


#if 0
static void
store_big(int val, unsigned char *cp, int offset)
{
    assert(offset >= 0);
    assert(offset < (1 << 16));
    assert(val >= 0);
    assert(val < (1 << 15));
    cp[offset] = (val >> 8) | 128;
    cp[offset + 1] = val;
}
#endif


static codefile::pointer
unit_search(const rcstring &name, int &segnum)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "name = %s", name.quote_c().c_str());
    //
    // First we look in the host file.
    //
    assert(hostfile);
    {
        segment::pointer sp = hostfile->find_segment_by_unit_name(name);
        if (sp)
        {
            segnum = sp->get_segment_number();
            return hostfile;
        }
    }

    //
    // Then we look in the library files.
    //
    for (libfiles_t::iterator it = libfiles.begin(); it != libfiles.end(); ++it)
    {
        codefile::pointer fp = *it;
        segment::pointer sp = fp->find_segment_by_unit_name(name);
        if (sp)
        {
            segnum = sp->get_segment_number();
            return fp;
        }
    }

    segnum = -1;
    return codefile::pointer();
}


/**
  * The build_file_list function opens input codefiles and reads segtbls.
  * The order of these files determines how identifiers
  * will be searched for.
  */
static void
build_file_list(const rcstring_list &filenames)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(filenames.size() >= 1);
    hostfile = codefile_file::create(filenames[0]); // USERHOST,
    DEBUG(2, "hostfile = %p", hostfile.get());
    byte_sex = hostfile->get_byte_sex();

    for (size_t f = 1; f < filenames.size(); ++f)
    {
        rcstring fname = filenames[f];
        DEBUG(2, "fname = %s", fname.quote_c().c_str());
        if (fname == "*")
            fname = "system.library";
        codefile::pointer p = codefile_file::create(fname); // USERLIB
        DEBUG(2, "p = %p", p.get());

        if (p->get_byte_sex() != hostfile->get_byte_sex())
        {
            explain_output_error
            (
                "codefile %s: library file byte sex %s...",
                fname.quote_c().c_str(),
                byte_sex_name(p->get_byte_sex()).c_str()
            );
            explain_output_error_and_die
            (
                "codefile %s: ... mismatch with host file byte sex %s",
                hostfile->get_filename().quote_c().c_str(),
                byte_sex_name(hostfile->get_byte_sex()).c_str()
            );
        }

        libfiles.push_back(p);
    }
}


static segrec::pointer segdict[maxseg1];


static void
check_hostsp(const segrec::pointer &sp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    switch (sp->get_segkind())
    {
    case HOSTSEG:
    case UNLINKED_INTRINS:
    case SEGPROC:
    case SEPRTSEG:
    case DATASEG:
        break;

    case UNITSEG:
        if (sp->get_codeleng() == 0)
        {
            int segnum = 0;
            codefile::pointer fp = unit_search(sp->get_src_segname(), segnum);
            if (!fp)
            {
                explain_output_error_and_die
                (
                    "codefile %s: unit %s not found",
                    sp->get_src_filename().quote_c().c_str(),
                    sp->get_src_segname().quote_c().c_str()
                );
            }
            sp->set_srcfile(fp, segnum);
        }
        break;

    case LINKED:
    case LINKED_INTRINS:
        if (hostsp)
        {
            explain_output_error
            (
                "codefile %s, segment %s: duplicate host "
                    "segment ...",
                sp->get_src_filename().quote_c().c_str(),
                sp->get_src_segname().quote_c().c_str()
            );
            explain_output_error_and_die
            (
                "codefile %s, segment %s: ... here is the "
                    "first definition",
                hostsp->get_src_filename().quote_c().c_str(),
                hostsp->get_src_segname().quote_c().c_str()
            );
        }
        else
        {
            hostsp = sp;
            DEBUG(2, "hostsp = %p", hostsp.get());
        }
        break;
    }
}


/**
  * The build_seg_info function initializes the segdict table from the
  * host prototype segment table.  All legal states are checked, and
  * imported units found.  This leaves a list of all segs to finally
  * appear in the output codefile.
  */

static void
build_seg_info(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for (int n = 0; ; ++n)
    {
        segment::pointer sp1 = hostfile->get_segment(n);
        if (!sp1)
            break;
        if
        (
            (
                sp1->get_kind() != LINKED
            &&
                sp1->get_kind() != LINKED_INTRINS
            )
        ||
            sp1->get_code_length() != 0
        )
        {
            segrec::pointer sp = segrec::create(hostfile, sp1);
            size_t s = sp1->get_segment_number() % SIZEOF(segdict);
            while (segdict[s])
                s = (n + 1) % SIZEOF(segdict);
            segdict[s] = sp;

            check_hostsp(sp);
        }
    }

    //
    // Figure out the first empty slot in the output codefile.
    //
    for (nextseg = firstseg; nextseg < maxseg1; ++nextseg)
    {
        if (!segdict[nextseg])
            break;
    }
}


/**
  * The build_sep_list function searches through libraries and adds
  * onto a global list of separate segments that are to be searched
  * for procedures and globals.
  */

static void
build_sep_list(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for (libfiles_t::iterator it = libfiles.begin(); it != libfiles.end(); ++it)
    {
        codefile::pointer fp = *it;
        for (int n = 0; ; ++n)
        {
            segment::pointer sp1 = fp->get_segment(n);
            if (!sp1)
                break;
            if (sp1->get_kind() == SEPRTSEG)
            {
                segrec::pointer sp = segrec::create(fp, sp1);
                seplist.push_back(sp);

                check_hostsp(sp);
            }
        }
    }
}


/**
  * The phase1 function opens host and library files and reads in the
  * segment tables.  All fields are verified and the hostfile and
  * libfiles file list is built.  The prototype final segment table is
  * set up in segdict[*] from the host file, and the separate segment
  * list is set up for searching in later phases.
  */

static void
phase1(const rcstring_list &filenames)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    reflitypes = set(UNITREF) + set(GLOBREF) + set(PUBLREF) + set(PRIVREF) +
        set(CONSTREF) + set(SEPPREF) + set(SEPFREF);
    nextbaselc = 3;

    build_file_list(filenames);
    build_seg_info();
    build_sep_list();
    DEBUG(1, "}");
}


/**
  * The add_unit function is called to find or allocate a library unit
  * that is found in link info as an external reference.  This occurs in
  * library units which use other units.  If the unit can't be found or
  * no room, error is called.
  */

static void
add_unit(const rcstring &name)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    int segnum = -1;
    codefile::pointer fp = unit_search(name, segnum);
    if (fp && fp != hostfile)
    {
        segment::pointer sp1 = fp->get_segment_by_number(segnum);
        if (sp1)
        {
            if (nextseg >= maxseg1)
            {
                explain_output_error_and_die
                (
                    // FIXME: the output file's name, not the input!
                    "%s: no room in output segment dictionary",
                    fp->get_filename().c_str()
                );
            }

            segrec::pointer sp = segrec::create(fp, sp1);
            segdict[nextseg] = sp;
            ++nextseg;

            check_hostsp(sp);
        }
    }
}


static bool dumpseps;


/**
  * The validate function verifies link_info_entry format.  If a UNITREF
  * is found, it searched for and possibly allocated.  If the unit must
  * be added to segdict, it is placed after current position so it will
  * have its link info read as well.
  */

static void
validate(const link_info_entry::pointer &ep)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring caption = "codefile " + ep->origin->get_src_filename().quote_c()
        + ", segment " + ep->origin->get_src_segname().quote_c();
    ep->sanity_check(caption);

    switch (ep->get_litype())
    {
    case UNITREF:
        if (ep->has_references())
            add_unit(ep->get_name());
        break;

    case SEPPROC:
    case SEPFUNC:
        ep->place.destbase = 0;
        ep->place.srcbase = 0;
        ep->place.length = 0;
        break;

    case CONSTREF:
    case GLOBREF:
    case PRIVREF:
    case PUBLREF:
    case SEPFREF:
    case SEPPREF:
    case GLOBDEF:
    case PUBLDEF:
    case EXTPROC:
    case EXTFUNC:
    case CONSTDEF:
    case EOFMARK:
        break;
    }
}


/**
  * The read_link_info function reads in the link info for segment sp and
  * builds its symtab.  Some simple disk io routines do unblocking, and
  * all fields are again verified.  The only legal link_info_type_t are
  * in oktypes.
  */

static void
read_link_info(const segrec::pointer &sp, const set &oktypes)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    assert(sp);
    if (verbose)
    {
        printf
        (
            "codefile %s, segment %s: reading...\n",
            sp->get_src_filename().quote_c().c_str(),
            sp->get_src_segname().quote_c().c_str()
        );
    }

    const link_info_list &lil = sp->get_link_info();
    for (size_t j = 0; j < lil.size(); ++j)
    {
        link_info::pointer lip = lil.get(j);
        link_info_entry::pointer ep = link_info_entry::create(lip, sp.get());
        DEBUG(2, "ep = %s", ep->representation().c_str());

        if (ep->get_litype() == EOFMARK)
        {
            if (sp->get_segkind() == HOSTSEG)
                nextbaselc = ep->get_nextlc();
            break;
        }

        if (oktypes.has_member(ep->get_litype()))
            validate(ep);
        else
        {
            explain_output_error_and_die
            (
                "codefile %s, segment %s, symbol %s: invalid list info "
                    "type (%s)",
                sp->get_src_filename().quote_c().c_str(),
                sp->get_src_segname().quote_c().c_str(),
                ep->get_name().quote_c().c_str(),
                link_info_type_name(ep->get_litype()).c_str()
            );
        }

        if (dumpseps)
        {
            switch (ep->get_litype())
            {
            case EXTFUNC:
            case EXTPROC:
            case GLOBREF:
            case SEPFREF:
            case SEPPREF:
                dumpseps = false;
                break;

            case CONSTDEF:
            case CONSTREF:
            case EOFMARK:
            case GLOBDEF:
            case PRIVREF:
            case PUBLDEF:
            case PUBLREF:
            case SEPFUNC:
            case SEPPROC:
            case UNITREF:
                break;
            }
        }
        if (!reflitypes.has_member(ep->get_litype()) || ep->has_references())
            sp->symtab_insert(ep);
    }
    DEBUG(1, "}");
}


/**
  * Buildplaces reads code of separate segments from disk to generate the
  * placerec entries for use during #phase3.
  */

static void
build_places(const segrec::pointer &sp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(sp);
    int nbytes = sp->get_codeleng();
    const unsigned char *cp = sp->get_data();

    if (cp[nbytes - 2] != sp->get_segment_number())
    {
        explain_output_error_and_die
        (
            "codefile %s, segment %s: procedure dictionary has wrong segment "
                "number (has %d, expected %d)",
            sp->get_src_filename().quote_c().c_str(),
            sp->get_src_segname().quote_c().c_str(),
            cp[nbytes - 2],
            sp->get_segment_number()
        );
    }

    int nprocs = cp[nbytes - 1];
    if (nprocs > maxproc)
    {
        explain_output_error_and_die
        (
            "codefile %s, segment %s: procedure dictionary too long (%d)",
            sp->get_src_filename().quote_c().c_str(),
            sp->get_src_segname().quote_c().c_str(),
            nprocs
        );
    }

    //
    // Now we traverse the symbol table, filling in the ep->place values
    // for each defined procedure or function.
    //
    DEBUG(2, "build places");
    for
    (
        segrec::symbol_table_t::iterator it = sp->symtab.begin();
        it != sp->symtab.end();
        ++it
    )
    {
        link_info_entry::pointer ep = it->second;
        assert(ep);
        switch (ep->get_litype())
        {
        case CONSTDEF:
        case CONSTREF:
        case EOFMARK:
        case EXTFUNC:
        case EXTPROC:
        case GLOBDEF:
        case GLOBREF:
        case PRIVREF:
        case PUBLDEF:
        case PUBLREF:
        case SEPFREF:
        case SEPPREF:
        case UNITREF:
            break;

        case SEPPROC:
        case SEPFUNC:
            if (ep->get_srcproc() <= 0 || ep->get_srcproc() > nprocs)
            {
                explain_output_error_and_die
                (
                    "codefile %s, segment %s: symtab %s: procedure number %d "
                        "is not in the range 1..%d",
                    ep->origin->get_src_filename().quote_c().c_str(),
                    ep->origin->get_src_segname().quote_c().c_str(),
                    ep->get_name().quote_c().c_str(),
                    ep->get_srcproc(),
                    nprocs
                );
            }
            else
            {
                // navigate the procedure dictionary,
                // for the location of the procedure's jump table.
                assert((nbytes & 1) == 0);
                int i = nbytes - 2 - 2 * ep->get_srcproc();
                int offset = fetch_word(cp, i);
                if ((offset & 1) || offset == 0 || offset >= i)
                {
                    explain_output_error_and_die
                    (
                        "codefile %s, segment %s, symbol %s, procedure "
                            "number %d: the procedure dictionary pointer "
                            "is broken (%d)",
                        ep->origin->get_src_filename().quote_c().c_str(),
                        ep->origin->get_src_segname().quote_c().c_str(),
                        ep->get_name().quote_c().c_str(),
                        ep->get_srcproc(),
                        offset
                    );
                }
                i -= offset;
                int check_procnum = cp[i];
                if (check_procnum != ep->get_srcproc() && check_procnum != 0)
                {
                    explain_output_error_and_die
                    (
                        "codefile %s, segment %s, symbol %s, procedure number "
                            "%d: the jump table has the wrong procedure number "
                            "(%d)",
                        ep->origin->get_src_filename().quote_c().c_str(),
                        ep->origin->get_src_segname().quote_c().c_str(),
                        ep->get_name().quote_c().c_str(),
                        ep->get_srcproc(),
                        check_procnum
                    );
                }
                else
                {
                    int j = fetch_word(cp, i - 2) + 4;
                    ep->place.srcbase = i + 2 - j;
                    if
                    (
                        ep->place.srcbase < 0
                    ||
                        j <= 0
                    ||
                        j > maxic
                    )
                    {
                        explain_output_error_and_die
                        (
                            "codefile %s, segment %s, symbol %s, "
                                "procedure number %d: procedure placement "
                                "invalid (%d, %d)",
                            ep->origin->get_src_filename().quote_c().c_str(),
                            ep->origin->get_src_segname().quote_c().c_str(),
                            ep->get_name().quote_c().c_str(),
                            ep->get_srcproc(),
                            ep->place.srcbase,
                            j
                        );
                    }
                    else
                    {
                        ep->place.length = j;
                    }
                }
            }
            DEBUG(2, "%s", ep->representation().c_str());
            break;
        }
    }
}


/**
  * The phase2 function reads in all linker information associated
  * with the segments in #segdict and #seplist.  Again, all fields are
  * checked carefully.  As a help to the #phase3 function, ref lists are
  * collected and place records for separate proc/func are computed.
  * Some small optimization is done to eliminate the #seplist if it is
  * not going to be needed.
  */

static void
phase2(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    dumpseps = true;
    for (int s = 0; s < maxseg1; ++s)
    {
        segrec::pointer sp = segdict[s];
        if (!sp)
            continue;
        switch (sp->get_segkind())
        {
        case LINKED:
        case LINKED_INTRINS:
        case DATASEG:
            break;

        case HOSTSEG:
        case UNLINKED_INTRINS:
            {
                set ok =
                    set(PUBLDEF) + set(CONSTDEF) + set(EXTPROC) + set(EXTFUNC);
                read_link_info(sp, ok);
            }
            break;

        case SEGPROC:
            {
                set ok = set(EXTPROC) + set(EXTFUNC);
                read_link_info(sp, ok);
            }
            break;

        case UNITSEG:
            {
                set ok =
                    set(PUBLREF) + set(PRIVREF) + set(UNITREF) + set(CONSTDEF) +
                    set(EXTPROC) + set(EXTFUNC);
                read_link_info(sp, ok);
            }
            break;

        case SEPRTSEG:
            {
                set ok =
                    set(GLOBREF) + set(GLOBDEF) + set(CONSTDEF) + set(SEPPROC) +
                    set(SEPFUNC);
                read_link_info(sp, ok);
            }
            break;
        }
    }
    if (dumpseps)
        seplist.clear();
    for
    (
        segrec_list_t::iterator it = seplist.begin();
        it != seplist.end();
        ++it
    )
    {
        segrec::pointer sp = *it;
        set more = set(GLOBDEF) + set(CONSTDEF) + set(SEPPROC) + set(SEPFUNC);
        read_link_info(sp, reflitypes + more);
    }
    if (segdict[master_slot])
    {
        segrec::pointer sp = segdict[master_slot];
        if (sp->get_segkind() == SEPRTSEG)
            build_places(sp);
    }
    for
    (
        segrec_list_t::iterator it = seplist.begin();
        it != seplist.end();
        ++it
    )
    {
        segrec::pointer sp = *it;
        build_places(sp);
    }
    DEBUG(1, "}");
}




static bool sephost;

/**
  * The procsrch2 function goes through symtab and builds a list of
  * procedures and functions which occur in the tree and whose litype is
  * in the #okset.  The resulting list is not ordered in any particular
  * fashion.  It is called to build initial uproc list.
  */

static void
procsrch2(const set &okset, segrec::symbol_table_t &symtab,
    workrec_list_t &result)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for
    (
        segrec::symbol_table_t::iterator it = symtab.begin();
        it != symtab.end();
        ++it
    )
    {
        link_info_entry::pointer ep = it->second;
        if (okset.has_member(ep->get_litype()))
        {
            workrec::pointer wp = workrec::create(ep);
            wp->needsrch = true;
            if (sephost)
                wp->newproc = 0;
            else
                wp->newproc = ep->get_srcproc();

            result.push_back(wp);
        }
    }
}


/**
  * The procsrch3 function searches a symbol table to locate a specific
  * procedure number, and add it to #uprocs.
  */

static workrec::pointer
procsrch3(segrec::symbol_table_t &symtab, int pnum, workrec_list_t &uprocs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for
    (
        segrec::symbol_table_t::iterator it = symtab.begin();
        it != symtab.end();
        ++it
    )
    {
        link_info_entry::pointer sym = it->second;
        if
        (
            (sym->get_litype() == SEPPROC || sym->get_litype() == SEPFUNC)
        &&
            sym->get_srcproc() == pnum
        )
        {
            for
            (
                workrec_list_t::iterator it2 = uprocs.begin();
                it2 != uprocs.end();
                ++it2
            )
            {
                workrec::pointer wp = *it2;
                if (wp->refsym == sym)
                    return wp;
            }

            workrec::pointer wp = workrec::create(sym);
            uprocs.push_back(wp);
            return wp;
        }
    }
    return workrec::pointer();
}


/**
  * The find_and_add function finds the procedure numbered pnum in the
  * symbol table symtab.  An error is given if the required procedure
  * cannot be found.  It returns a work node for the procedure once it
  * has been found.  This node is also added into the uprocs list.  Any
  * procs added this way are "invisible", dragged along because of global
  * refs/defs.
  */

static workrec::pointer
find_and_add(segrec::symbol_table_t &symtab, int pnum, workrec_list_t &uprocs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    workrec::pointer wp = procsrch3(symtab, pnum, uprocs);
    if (!wp)
    {
        explain_output_error_and_die("missing proc %d", pnum);
    }
    return wp;
}


static workrec_list_t uprocs;


/**
  * The find_new_procs function is called to place new procedures into the
  * #uprocs work list that are needed to resolve GLOBDEFs, SEPPREFs, and
  * SEPFREFs.  The #other_symbols list is traversed and for each element whose
  * defining procedure has not been added into the #uprocs list, the defining
  * procedure is located and added into #uprocs.
  */

static void
find_new_procs(workrec_list_t &other2, const workrec_list_t &procs2)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for
    (
        workrec_list_t::const_iterator it = other2.begin();
        it != other2.end();
        ++it
    )
    {
        workrec::pointer wp = *it;
        assert(wp);
        if (wp->defproc == 0)
        {
            assert(wp->refsym);
            int pnum =
                (
                    wp->refsym->get_litype() == GLOBREF
                ?
                    wp->defsym->get_homeproc()
                :
                    wp->defsym->get_srcproc()
                );
            for
            (
                workrec_list_t::const_iterator it2 = procs2.begin();
                it2 != procs2.end();
                ++it2
            )
            {
                workrec::pointer wp2 = *it2;
                if
                (
                    wp->defseg == wp2->defseg
                &&
                    wp2->defsym->get_srcproc() == pnum
                )
                {
                    wp->defproc = wp2;
                    break;
                }
            }
            if (!wp->defproc)
                wp->defproc = find_and_add(wp->defseg->symtab, pnum, uprocs);
        }
    }
}


/**
  * The separate_procedure_search function sequentially search the
  * symbol tables in the #seplist to resolve the refsym of #inlist.  It
  * basically just calls symtab_lookup repetively and fixes up defsym and
  * defseg fields.  If the name of the refsym could not be found, an
  * error is given.
  */

static void
separate_procedure_search(const workrec::pointer &inlist,
    link_info_type_t oktype)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for
    (
        segrec_list_t::iterator it = seplist.begin();
        it != seplist.end();
        ++it
    )
    {
        segrec::pointer sp = *it;
        link_info_entry::pointer ep =
            sp->symtab_lookup(inlist->refsym->get_name(), oktype);
        if (ep)
        {
            inlist->defsym = ep;
            inlist->defseg = sp;
            break;
        }
    }
}


/**
  * The procedure_insert function is called to insert work into the
  * procedures list using a special set of sort keys so that the
  * #copy_in_procedures function will run reasonably fast and use the disk
  * efficiently.  The procedures list is sorted by segment and srcbase
  * keys.  The segment ordering is dictated by the #seplist, so user
  * ASECTS (etc) will retain their original ordering.
  */

static void
procedure_insert(const workrec::pointer &work, workrec_list_t &outlist)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for
    (
        workrec_list_t::iterator it = outlist.begin();
        it != outlist.end();
        ++it
    )
    {
        workrec::pointer crnt = *it;
        assert(crnt);
        if
        (
            (
                crnt->defsym->get_litype() != SEPPROC
            &&
                crnt->defsym->get_litype() != SEPFUNC
            )
        ||
            work->defseg < crnt->defseg
        ||
            (
                work->defseg == crnt->defseg
            &&
                work->defsym->place.srcbase < crnt->defsym->place.srcbase
            )
        )
        {
            outlist.insert(it, work);
            return;
        }
    }
    outlist.push_back(work);
}


/**
  * The resolve function removes work items from inlist, searches symtabs
  * for its corresponding definition symbol (error if not found), and
  * moves the work item into the output list.  Each flavor of work item
  * needs some special handling to collect extra info related to specific
  * things.  In general, defsym and defseg are filled in.  The insert
  * algorithm is special for procedure types to make life easier on
  * checkrefs.
  */

static void
resolve(workrec_list_t &inlist, workrec_list_t &outlist)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    workrec_list_t temp(inlist);
    inlist.clear();
    for
    (
        workrec_list_t::const_iterator it = temp.begin();
        it != temp.end();
        ++it
    )
    {
        workrec::pointer wp = *it;
        assert(wp);
        DEBUG(2, "wp = %s", wp->representation().c_str());
        switch (wp->refsym->get_litype())
        {
        case CONSTDEF:
        case EOFMARK:
        case GLOBDEF:
        case PUBLDEF:
            assert(!"should not be on work queue");
            break;

        case UNITREF:
            {
                DEBUG(2, "unitref");
                int seg = -1;
                assert(hostfile);
                codefile::pointer fp = unit_search(wp->refsym->get_name(), seg);
                if (!fp)
                {
                    explain_output_error_and_die
                    (
                        "codefile %s, segment %s: unit %s not found",
                        wp->refsym->origin->get_src_filename().
                            quote_c().c_str(),
                        wp->refsym->origin->get_src_segname().
                            quote_c().c_str(),
                        wp->refsym->get_name().quote_c().c_str()
                    );
                }
                else if (fp == hostfile)
                {
                    wp->defsym = wp->refsym;
                    wp->defsegnum = seg;
                }
                else
                {
                    // FIXME
                }
            }
            break;

        case GLOBREF:
            DEBUG(2, "globref");
            separate_procedure_search(wp, GLOBDEF);
            wp->defproc.reset();
            break;

        case PUBLREF:
            DEBUG(2, "publref");
            if (hostsp)
            {
                DEBUG(2, "hostsp = %p", hostsp.get());
                wp->defsym =
                    hostsp->symtab_lookup(wp->refsym->get_name(), PUBLDEF);
                assert(wp->defsym);
                wp->defseg = hostsp;
            }
            break;

        case PRIVREF:
            DEBUG(2, "privref");
            wp->newoffset = nextbaselc;
            nextbaselc = nextbaselc + wp->refsym->get_nwords();
            if (hostsp)
            {
                DEBUG(2, "hostsp = %p", hostsp.get());
                wp->defsym = wp->refsym;
            }
            wp->defseg = hostsp;
            break;

        case CONSTREF:
            DEBUG(2, "constref");
            if (hostsp)
            {
                DEBUG(2, "hostsp = %p", hostsp.get());
                wp->defsym =
                    hostsp->symtab_lookup(wp->refsym->get_name(), CONSTDEF);
                assert(wp->defsym);
                wp->defseg = hostsp;
            }
            break;

        case EXTPROC:
        case SEPPROC:
        case SEPPREF:
            {
                DEBUG(2, "external procedure");
                separate_procedure_search(wp, SEPPROC);
                if (wp->refsym->get_litype() == SEPPREF)
                    wp->defproc.reset();
                if (wp->defsym)
                {
                    int np =
                        (
                            wp->refsym->get_litype() == SEPPREF
                        ?
                            wp->refsym->get_nwords()
                        :
                            wp->refsym->get_nparams()
                        );
                    if (wp->defsym->get_nparams() != np)
                    {
                        explain_output_error_and_die
                        (
                            "codefile %s, segment %s, procedure %s: parameter "
                                "mismatch (def %d != ref %d)",
                            wp->refsym->origin->get_src_filename().
                                quote_c().c_str(),
                            wp->refsym->origin->get_src_segname().
                                quote_c().c_str(),
                            wp->refsym->get_name().quote_c().c_str(),
                            wp->defsym->get_nparams(),
                            np
                        );
                    }
                }
            }
            break;

        case EXTFUNC:
        case SEPFUNC:
        case SEPFREF:
            {
                DEBUG(2, "external function");
                separate_procedure_search(wp, SEPFUNC);
                if (wp->refsym->get_litype() == SEPFREF)
                    wp->defproc.reset();
                if (wp->defsym)
                {
                    int np =
                        (
                            wp->refsym->get_litype() == SEPFREF
                        ?
                            wp->refsym->get_nwords()
                        :
                            wp->refsym->get_nparams()
                        );
                    if (wp->defsym->get_nparams() != np)
                    {
                        explain_output_error_and_die
                        (
                            "codefile %s, segment %s, function %s: parameter "
                                "mismatch (def %d != ref %d)",
                            wp->refsym->origin->get_src_filename().
                                quote_c().c_str(),
                            wp->refsym->origin->get_src_segname().
                                quote_c().c_str(),
                            wp->refsym->get_name().quote_c().c_str(),
                            wp->defsym->get_nparams(),
                            np
                        );
                    }
                }
            }
            break;
        }

        if (!wp->defsym)
        {
            explain_output_error_and_die
            (
                "codefile %s, segment %s: %s %s undefined",
                wp->refsym->origin->get_src_filename().quote_c().c_str(),
                wp->refsym->origin->get_src_segname().quote_c().c_str(),
                link_info_type_name(wp->refsym->get_litype()).c_str(),
                wp->refsym->get_name().quote_c().c_str()
            );
        }
        else
        {
            DEBUG(2, "wp = %s", wp->representation().c_str());
            switch (wp->defsym->get_litype())
            {
            case SEPPROC:
            case SEPFUNC:
                procedure_insert(wp, outlist);
                break;

            case CONSTDEF:
            case CONSTREF:
            case EOFMARK:
            case EXTFUNC:
            case EXTPROC:
            case GLOBDEF:
            case GLOBREF:
            case PRIVREF:
            case PUBLDEF:
            case PUBLREF:
            case SEPFREF:
            case SEPPREF:
            case UNITREF:
            default:
                outlist.push_back(wp);
                break;
            }
        }
    }
    DEBUG(1, "}");
}


static workrec_list_t uother;


/**
  * The checkrefs function slowly goes through all reference lists in
  * symbols which are in the okset to see if any "occur" within the
  * procedures and functions selected to be linked, that is contained in
  * #procs list.  Any symbols which have any refs in selected procedures
  * are given work nodes and are placed in the #uother list so #resolve
  * can be called right away.
  */

static void
checkrefs(const set &okset, const segrec::pointer &sp, workrec_list_t &procs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for
    (
        segrec::symbol_table_t::iterator it2 = sp->symtab.begin();
        it2 != sp->symtab.end();
        ++it2
    )
    {
        link_info_entry::pointer refsym = it2->second;
        if (!okset.has_member(refsym->get_litype()))
            continue;

        const link_info::references_t &rr = refsym->get_references();
        for
        (
            link_info::references_t::const_iterator it = rr.begin();
            it != rr.end();
            ++it
        )
        {
            int ref = *it;
            for
            (
                workrec_list_t::iterator it3 = procs.begin();
                it3 != procs.end();
                ++it3
            )
            {
                workrec::pointer pp = *it3;
                assert(pp->defsym);
                if
                (
                    pp->defsym->get_name() == refsym->get_name()
                &&
                    pp->needsrch
                )
                {
                    int src_base = pp->defsym->place.srcbase;
                    int src_end = src_base + pp->defsym->place.length;
                    if (src_base <= ref && ref < src_end)
                    {
                        workrec::pointer wp = workrec::create(refsym);
                        wp->refseg = sp;
                        uother.push_back(wp);

                        // why wasn't pp->defsym stashed anywhere?
                        wp->defsym = pp->defsym;
                        return;
                    }
                }
            }
        }
    }
}


/**
  * The find_locals function searches the main segmentss symbol table to place
  * any unresolved things like public references in unit segments into the
  * #ulocal list so they can be fixed up in the #fix_up_references function, in
  * addition to the separate procedures stuff.
  */

static void
find_locals(segrec::symbol_table_t &symtab, workrec_list_t &ulocal2)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    set okset = set(UNITREF) + set(PUBLREF) + set(PRIVREF);
    for
    (
        segrec::symbol_table_t::iterator it = symtab.begin();
        it != symtab.end();
        ++it
    )
    {
        link_info_entry::pointer sym = it->second;
        if (okset.has_member(sym->get_litype()))
        {
            workrec::pointer wp = workrec::create(sym);
            ulocal2.push_back(wp);
        }
    }
}


static workrec_list_t procs;
static workrec_list_t local;
static workrec_list_t other_symbols;
static workrec_list_t ulocal;


/**
  * The build_work_lists function is called for all segments which need
  * to be copied, and maybe need to have sepprocs or other stuff fixed up
  * within them.  The idea here is to get a list of procs and other items
  * needing attention, with all the subtle implications of global defs
  * falling in procs which are not yet selected for linking etc.
  *
  * In fact, three lists are built:
  *
  * 1. The procs list with all procedures and functions to be grabbed from the
  *    various separate segments.
  * 2. The local list of refeferences in the original segment which must ALL be
  *    fixed up such as public or private references in a unit segment.
  * 3. The other_symbols list which has work items which have at least one
  *    reference occuring in the procedures or functions in the "procs" list.
  */

static void
build_work_lists(const segrec::pointer &segp)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    assert(segp);
    switch (segp->get_segkind())
    {
    case HOSTSEG:
    case SEGPROC:
    case UNITSEG:
    case UNLINKED_INTRINS:
    default:
        sephost = false;
        break;

    case SEPRTSEG:
        sephost = true;
        break;

    case DATASEG:
    case LINKED:
    case LINKED_INTRINS:
        DEBUG(2, "no need");
        return;
    }

    uprocs.clear();
    if (sephost)
    {
        seplist.push_back(segp);
        set ok = set(SEPPROC) + set(SEPFUNC);
        procsrch2(ok, segp->symtab, uprocs);
    }
    else
    {
        set ok = set(EXTPROC) + set(EXTFUNC);
        procsrch2(ok, segp->symtab, uprocs);
    }
    for (;;)
    {
        DEBUG(2, "uprocs = %s", workrec_list_representation(uprocs).c_str());
        if (uprocs.empty())
            break;
        resolve(uprocs, procs);
        set ok = set(GLOBREF) + set(SEPPREF) + set(SEPFREF);
        for
        (
            segrec_list_t::iterator it = seplist.begin();
            it != seplist.end();
            ++it
        )
        {
            segrec::pointer sp = *it;
            checkrefs(ok, sp, procs);
        }
        resolve(uother, other_symbols);
        find_new_procs(other_symbols, procs);
    }
    if (!sephost)
    {
        find_locals(segp->symtab, ulocal);
        resolve(ulocal, local);
    }

    for
    (
        workrec_list_t::iterator it = procs.begin();
        it != procs.end();
        ++it
    )
    {
        (*it)->needsrch = true;
    }

    for
    (
        segrec_list_t::iterator it = seplist.begin();
        it != seplist.end();
        ++it
    )
    {
        segrec::pointer sp = *it;
        set ok = set(PUBLREF) + set(PRIVREF) + set(CONSTREF);
        checkrefs(ok, sp, procs);
    }
    resolve(uother, other_symbols);
    DEBUG(1, "}");
}


/**
  * The write_to_codefile function takes the finalized destination
  * segment and writes it to the output codefile.  This also involves
  * setting up values in the final segment table.
  */

static void
write_to_codefile(const segrec::pointer &sp, const codefile::pointer &cfp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(sp);
    if (hostsp == sp)
        sp->update_proc1_varsize(nextbaselc);

    sp->write_to_codefile(cfp);
}


static FILE *map;


/**
  * The link_segment function is called for each segment to be placed
  * into the final codefile.  All the other procedures of Phase 3 are
  * called from here.
  *
  * This function facilitates linking the master segment separatly from
  * the other segments to ensure that the DATASZ of the outer block
  * correctly reflects the number of PRIVREF words allocated by resolve.
  */

static void
link_segment(const segrec::pointer &sp, const codefile::pointer &cfp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(sp);
    DEBUG(2, "%s", sp->get_src_segname().quote_c().c_str());
    sephost = false;
    assert(sp);
    if (verbose)
    {
        printf
        (
            "Linking %s, segnum %d\n",
            sp->get_src_segname().quote_c().c_str(),
            sp->get_segment_number()
        );
    }
    build_work_lists(sp);

    sp->read_source_segment(procs);
    if (map)
        sp->write_map(map, procs, local, other_symbols);
    sp->copy_in_procedures(procs);
    sp->fix_up_references(local, true, procs);
    sp->fix_up_references(other_symbols, false, procs);
    write_to_codefile(sp, cfp);

    if (sephost)
       seplist.push_front(sp);
}


/**
  * The phase3 function does all the real work of code manipulation.  For
  * each segment in #segdict to be placed into the output code file, all
  * referenced procedures and functions are found, globals and other_symbols
  * referencess are resolved, and finally the final code segment is
  * built.  In the case of a SEPRTSEG host (e.g. an interpreter), then
  * all the procs in it are put in the unresolved list and the host
  * segment is made to appear as just another separate segment.  This
  * drags along all the original procedures and maintains their original
  * ordering for possible ASECT integrity.
  */

static void
phase3(const rcstring &fname, const rcstring &mapname, const rcstring &notice)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    sephost = false;

    codefile::pointer cfp =
        p_machine_set_explicitly() ? codefile_file::create(fname, byte_sex) :
        hostfile ? hostfile->new_file_factory(fname) :
        !libfiles.empty() ? libfiles[0]->new_file_factory(fname) :
        codefile_file::create(fname, byte_sex);

    if (!mapname.empty())
    {
        if (mapname == "-")
            map = stdout;
        else
            map = explain_fopen_or_die(mapname.c_str(), "w");
        fprintf(map, "Link map for ");
        if (hostsp)
            fprintf(map, "%s", hostsp->get_src_segname().quote_c().c_str());
        else
            fprintf(map, "assembler host");
        fprintf(map, "\n");
    }

    for (int s = 0; s < maxseg1; ++s)
    {
        segrec::pointer sp = segdict[s];
        if (sp && sp != hostsp)
        {
            link_segment(sp, cfp);
        }
    }
    if (hostsp)
    {
        DEBUG(2, "hostsp = %p", hostsp.get());
        link_segment(hostsp, cfp);
    }

    cfp->set_copyright_notice(notice);
    cfp->write_segment_dictionary();
    cfp.reset();

    if (map)
    {
        if (hostsp)
            fprintf(map, "\nNext base LC = %d\n", nextbaselc);
        if (map == stdout)
            explain_fflush_or_die(map);
        else
            explain_fclose_or_die(map);
    }
    DEBUG(1, "}");
}


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <filename>...\n", prog);
    fprintf(stderr, "       %s --version\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    rcstring output_file_name;
    rcstring map_file_name;
    rcstring notice;
    for (;;)
    {
        static const struct option options[] =
        {
            { "copyright", 1, 0, 'n' },
            { "debug", 0, 0, 'd' },
            { "map", 1, 0, 'm' },
            { "notice", 1, 0, 'C' },
            { "notice", 1, 0, 'n' },
            { "output", 1, 0, 'o' },
            { "p-machine", 1, 0, 'P' },
            { "verbose", 0, 0, 'v' },
            { "version", 0, 0, 'V' },
            { 0, 0, 0, 0 }
        };
        int c = getopt_long(argc, argv, "dm:n:o:P:Vv", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'd':
            ++debug_level;
            break;

        case 'm':
            map_file_name = optarg;
            break;

        case 'n':
            notice = optarg;
            break;

        case 'o':
            output_file_name = optarg;
            break;

        case 'P':
            // The p-machine dictates the shape of the codefile.
            p_machine_set(optarg);
            break;

        case 'V':
            version_print();
            return 0;

        case 'v':
            verbose = true;
            setlinebuf(stdout);
            break;

        default:
            usage();
        }
    }
    rcstring_list filenames;
    while (optind < argc)
        filenames.push_back(argv[optind++]);
    if (filenames.empty())
        usage();

    if (output_file_name.empty())
        explain_output_error_and_die("no --output file specified");

    //
    // The linker is made up of three phases:
    //
    // Phase 1:
    //     which open all input files, reads up seg tables from them and
    //     decides which segments are to be linked into the final code
    //     file.
    // Phase 2:
    //     reads the linker info for each segment that is going to
    //     be used, either to select sep procs from or copy with
    //     modifications into output code.  The main symbol trees are
    //     built here, one for each code segment.
    // Phase 3:
    //     does the crunching of code segments into their final form by
    //     figuring out the procs that need to be linked in, resolves
    //     all references (PUBLREF, GLOBREF, etc), patches the code
    //     pointed to by their reflists, and writes the final code
    //     seg(s).
    //
    // In the UCSD sources, these phases are each a separate segment procedure,
    // so that the amount of code in memory can be minimized.
    //

    phase1(filenames);
    phase2();
    phase3(output_file_name, map_file_name, notice);
    return 0;
}


// vim: set ts=8 sw=4 et :
