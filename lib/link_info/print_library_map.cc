//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/link_info.h>
#include <lib/opformat.h>
#include <lib/output.h>


void
link_info::print_library_map_ref(output::pointer &os)
    const
{
    os->fputs(opformat_name(format));
    os->fprintf(" reference ");
    if (references.size() != 1)
        os->fprintf("(%d times)", (int)references.size());
    else
        os->fprintf("(once)");

    if (!references.empty())
    {
        for
        (
            references_t::const_iterator it = references.begin();
            it != references.end();
            ++it
        )
        {
            if (it != references.begin())
                os->fputc(',');
            os->fprintf(" $%04X", *it);
        }
    }
}


void
link_info::print_library_map(library_map_columns &cols)
    const
{
    //
    // LITYPES = (EOFMARK, MODDULE, GLOBREF, PUBBLIC, PRIVVATE, CONNSTANT,
    //            GLOBDEF, PUBLICDEF, CONSTDEF, EXTPROC, EXTFUNC, SEPPROC
    //            SEPFUNC, SEPPREF, SEPFREF);
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
    //           END;
    cols.name->fputs(name);
    cols.segkind->fputs(link_info_type_name(litype));
    switch (litype)
    {
    case EOFMARK:
        cols.mtype->fprintf("%5d", nextlc);
        break;

    case GLOBREF:
    case PUBLREF:
    case CONSTREF:
    case SEPFREF:
    case SEPPREF:
    case UNITREF:
    case PRIVREF:
        cols.mtype->fprintf("%5d", nwords);
        print_library_map_ref(cols.comments);
        break;

    case EXTPROC:
    case EXTFUNC:
    case SEPPROC:
    case SEPFUNC:
        cols.mtype->fprintf("%5d", srcproc);
        cols.version->fprintf("%5d", nparams);
        break;

    case GLOBDEF:
        cols.mtype->fprintf("%5d", homeproc);
        cols.version->fprintf("$%04X", icoffset);
        break;

    case PUBLDEF:
        cols.mtype->fprintf("$%04X", baseoffset);
        break;

    case CONSTDEF:
        cols.mtype->fprintf("%5d", constval);
        break;
    }
    cols.eoln();
}


// vim: set ts=8 sw=4 et :
