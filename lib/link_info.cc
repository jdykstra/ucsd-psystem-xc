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

#include <lib/ac/stdio.h>
#include <lib/ac/string.h>
#include <libexplain/output.h>

#include <lib/codefile.h>
#include <lib/link_info.h>
#include <lib/pascal_id.h>


static const int maxlc = 32767;


link_info::~link_info()
{
}


link_info::link_info(
    const rcstring &a_name,
    link_info_type_t a_litype,
    int word6,
    int word7,
    int word8
) :
    name(a_name),
    litype(a_litype),
    format(opformat_word),
    nwords(0),
    srcproc(0),
    nparams(0),
    homeproc(0),
    icoffset(0),
    baseoffset(0),
    constval(0),
    nextlc(0)
{
    // format of link info records...
    //
    // LITYPES = (EOFMARK,MODDULE,GLOBREF,PUBBLIC,PRIVVATE,CONNSTANT,GLOBDEF,
    //            PUBLICDEF,CONSTDEF,EXTPROC,EXTFUNC,SSEPPROC,SSEPFUNC,
    //            SEPPREF,SEPFREF);
    // OPFORMAT = (WORD, BYTE, BIG);
    // LIENTRY = RECORD
    //             LINAME: ALPHA;            // word 0..3
    //             CASE LITYPE: LITYPES OF   // word 4
    //               MODDULE,
    //               PUBBLIC,
    //               PRIVVATE,
    //               SEPPREF,
    //               SEPFREF: (
    //                 FORMAT: OPFORMAT;     // word 5
    //                 NREFS: INTEGER;       // word 6
    //                 NWORDS: INTEGER);     // word 7
    //               CONSTDEF: (
    //                 CONSTANT: INTEGER);   // word 5
    //               PUBLICDEF: (
    //                 BASEOFFSET: INTEGER); // word 5
    //               EXTPROC,
    //               EXTFUNC,
    //               SEPPROC,
    //               SEPFUNC: (
    //                 PROCNUM: INTEGER;     // word 5
    //                 NPARAMS: INTEGER;     // word 6
    //                 RANGE: ^INTEGER)      // word 7
    //               GLOBDEF: (
    //                 procnum: integer;     // word 5
    //                 procoffset: integer)  // word 6
    //           END;
    switch (litype)
    {
    case CONSTREF:
    case GLOBREF:
    case PRIVREF:
    case PUBLREF:
    case SEPFREF:
    case SEPPREF:
    case UNITREF:
        format = (opformat_t)word6;
        nwords = word8;
        break;

    case EXTPROC:
    case EXTFUNC:
    case SEPPROC:
    case SEPFUNC:
        srcproc = word6;
        nparams = word7;
        break;

    case GLOBDEF:
        homeproc = word6;
        icoffset = word7;
        break;

    case PUBLDEF:
        assert(word6 > 0);
        assert(word6 <= maxlc);
        baseoffset = word6;
        break;

    case CONSTDEF:
        constval = word6;
        break;

    case EOFMARK:
#if 0
        assert(word6 > 0);
#endif
        nextlc = word6;
        break;
    }
}


link_info::pointer
link_info::create(const rcstring &a_name, link_info_type_t a_litype,
    int a_word6, int a_word7, int a_word8)
{
    return pointer(new link_info(a_name, a_litype, a_word6, a_word7, a_word8));
}


unsigned
link_info::serialize_size(void)
    const
{
    unsigned result = 16;
    switch (litype)
    {
    case EOFMARK:
        assert(nextlc > 0);
        break;

    case UNITREF:
    case GLOBREF:
    case PUBLREF:
    case CONSTREF:
    case SEPPREF:
    case SEPFREF:
    case PRIVREF:
        {
            size_t nrefs = references.size();
            unsigned nrefs8 = (nrefs + 7) & ~7;
            result += nrefs8 * 2;
        }
        break;

    case GLOBDEF:
    case PUBLDEF:
    case CONSTDEF:
    case EXTPROC:
    case EXTFUNC:
    case SEPPROC:
    case SEPFUNC:
        break;
    }
    return result;
}


unsigned
link_info::serialize(unsigned char *data, const codefile *cfp)
    const
{
    rcstring pname = pascal_id(name).upcase();
    if (pname.size() < 8)
        pname += rcstring::spaces(8 - pname.size());
    memcpy(data, pname.c_str(), 8);
    cfp->put_word(data + 8, litype);
    unsigned result = 16;
    switch (litype)
    {
    case EOFMARK:
        cfp->put_word(data + 10, nextlc);
        cfp->put_word(data + 12, 0);
        cfp->put_word(data + 14, 0);
        memset(data, ' ', 8);
        break;

    case UNITREF:
    case GLOBREF:
    case PUBLREF:
    case CONSTREF:
    case SEPPREF:
    case SEPFREF:
    case PRIVREF:
        {
            cfp->put_word(data + 10, format);
            unsigned nrefs = references.size();
            cfp->put_word(data + 12, nrefs);
            cfp->put_word(data + 14, nwords);
            unsigned nrefs8 = (nrefs + 7) & ~7;
            size_t nbytes = nrefs8 * 2;
            unsigned pos = 0;
            for
            (
                references_t::const_iterator it = references.begin();
                it != references.end();
                ++it
            )
            {
                cfp->put_word(data + 16 + pos, *it);
                pos += 2;
            }
            memset(data + 16 + pos, 0, nbytes - pos);
            result += nbytes;
        }
        break;

    case EXTPROC:
    case EXTFUNC:
    case SEPPROC:
    case SEPFUNC:
        cfp->put_word(data + 10, srcproc);
        cfp->put_word(data + 12, nparams);
        cfp->put_word(data + 14, 0);
        break;

    case GLOBDEF:
        cfp->put_word(data + 10, homeproc);
        cfp->put_word(data + 12, icoffset);
        cfp->put_word(data + 14, 0);
        break;

    case PUBLDEF:
        cfp->put_word(data + 10, baseoffset);
        cfp->put_word(data + 12, 0);
        cfp->put_word(data + 14, 0);
        break;

    case CONSTDEF:
        cfp->put_word(data + 10, constval);
        cfp->put_word(data + 12, 0);
        cfp->put_word(data + 14, 0);
        break;

    default:
        cfp->put_word(data + 10, 0);
        cfp->put_word(data + 12, 0);
        cfp->put_word(data + 14, 0);
        break;
    }
    return result;
}


void
link_info::add_reference(int addr)
{
    references.push_back(addr);
}


bool
link_info::same_as(const link_info &other)
    const
{
    if (litype != other.litype)
        return false;
    if (name != other.name)
        return false;

    switch (litype)
    {
    case EOFMARK:
        break;

    case UNITREF:
    case GLOBREF:
    case PUBLREF:
    case CONSTREF:
    case SEPPREF:
    case SEPFREF:
    case PRIVREF:
        if (format != other.format)
            return false;
        if (nwords != other.nwords)
            return false;
        // The UCSD native linker has a limit of 500 references per
        // link info record.  It isn't clear why.
        if (references.size() >= 500)
            return false;
        return true;

    case GLOBDEF:
    case PUBLDEF:
    case CONSTDEF:
    case EXTPROC:
    case EXTFUNC:
    case SEPPROC:
    case SEPFUNC:
        break;
    }
    return false;
}


void
link_info::copy_references(const link_info &other)
{
    for
    (
        references_t::const_iterator it = other.references.begin();
        it != other.references.end();
        ++it
    )
    {
        add_reference(*it);
    }
}


rcstring
link_info::representation(void)
    const
{
    rcstring_accumulator buf;
    buf.push_back("{ name = ");
    buf.push_back(name.quote_c());
    buf.push_back(", litype = ");
    buf.push_back(link_info_type_name(litype));

    switch (litype)
    {
    case CONSTREF:
    case GLOBREF:
    case PRIVREF:
    case PUBLREF:
    case SEPFREF:
    case SEPPREF:
    case UNITREF:
        buf.push_back(", format = ");
        buf.push_back(opformat_name(format));
        buf.printf(", nrefs = %d", (int)references.size());
        buf.printf(", nwords = %d", nwords);
        buf.push_back(", refs = [");
        for
        (
            references_t::const_iterator it = references.begin();
            it != references.end();
            ++it
        )
        {
            if (it != references.begin())
                buf.push_back(", ");
            buf.printf("%d", *it);
        }
        buf.push_back(']');
        break;

    case EXTPROC:
    case EXTFUNC:
        buf.printf(", srcproc = %d", srcproc);
        buf.printf(", nparams = %d", nparams);
        break;

    case SEPPROC:
    case SEPFUNC:
        buf.printf(", srcproc = %d", srcproc);
        buf.printf(", nparams = %d", nparams);
        break;

    case GLOBDEF:
        buf.printf(", homeproc = %d", homeproc);
        buf.printf(", icoffset = %d", icoffset);
        break;

    case PUBLDEF:
        buf.printf(", baseoffset = %d", baseoffset);
        break;

    case CONSTDEF:
        buf.printf(", constval = %d", constval);
        break;

    case EOFMARK:
        buf.printf(", nextlc = %d", nextlc);
        break;
    }
    buf.push_back(" }");
    return buf.mkstr();
}


void
link_info::sanity_check(const rcstring &caption)
    const
{
    if (name != name.identifier())
    {
        explain_output_error_and_die
        (
            "%s: link info has non-alpha name %s",
            caption.c_str(),
            name.quote_c().c_str()
        );
    }
    static const int maxproc = 160;
    static const int maxic = 20000;

    switch (litype)
    {
    case CONSTREF:
    case GLOBREF:
    case PRIVREF:
    case PUBLREF:
    case SEPFREF:
    case SEPPREF:
    case UNITREF:
        if (references.empty() || references.size() > 500)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: too many references (%d)",
                caption.c_str(),
                name.quote_c().c_str(),
                (int)references.size()
            );
        }
        switch (format)
        {
        case opformat_word:
        case opformat_byte:
        case opformat_big:
            break;

        default:
            explain_output_error_and_die
            (
                "%s: link info %s: bad format (%d)",
                caption.c_str(),
                name.quote_c().c_str(),
                format
            );
        }
        break;

    case CONSTDEF:
    case EOFMARK:
    case EXTFUNC:
    case EXTPROC:
    case GLOBDEF:
    case PUBLDEF:
    case SEPFUNC:
    case SEPPROC:
        break;
    }
    switch (litype)
    {
    case CONSTREF:
    case GLOBREF:
    case PUBLREF:
    case SEPFREF:
    case SEPPREF:
        break;

    case PRIVREF:
        if (nwords <= 0)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: bad private (%d) too small",
                caption.c_str(),
                name.quote_c().c_str(),
                nwords
            );
        }
        if (nwords > maxlc)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: bad private (%d) too large",
                caption.c_str(),
                name.quote_c().c_str(),
                nwords
            );
        }
        break;

    case UNITREF:
        break;

    case GLOBDEF:
        if (homeproc <= 0 || homeproc > maxproc)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: bad homeproc (%d)",
                caption.c_str(),
                name.quote_c().c_str(),
                homeproc
            );
        }
        if (icoffset < 0 || icoffset > maxic)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: bad icoffset (%d)",
                caption.c_str(),
                name.quote_c().c_str(),
                icoffset
            );
        }
        break;

    case PUBLDEF:
        if (baseoffset <= 0 || baseoffset > maxlc)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: bad baseoffset (%d)",
                caption.c_str(),
                name.quote_c().c_str(),
                baseoffset
            );
        }
        break;

    case EXTPROC:
    case EXTFUNC:
    case SEPPROC:
    case SEPFUNC:
        if (srcproc <= 0 || srcproc > maxproc)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: bad srcproc (%d)",
                caption.c_str(),
                name.quote_c().c_str(),
                srcproc
            );
        }
        if (nparams < 0 || nparams > 100)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: bad nparams (%d)",
                caption.c_str(),
                name.quote_c().c_str(),
                nparams
            );
        }
        break;

    case CONSTDEF:
        break;

    case EOFMARK:
        if (nextlc <= 0 || nextlc > maxlc)
        {
            explain_output_error_and_die
            (
                "%s: link info %s: bad nextlc (%d)",
                caption.c_str(),
                name.quote_c().c_str(),
                nextlc
            );
        }
        break;
    }
}


// vim: set ts=8 sw=4 et :
