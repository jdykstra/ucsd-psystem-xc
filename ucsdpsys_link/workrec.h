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

#ifndef UCSDPSYS_LINK_WORKREC_H
#define UCSDPSYS_LINK_WORKREC_H

#include <ucsdpsys_link/link_info_entry.h>

class segrec; // forward

/**
  * The workrec class is used to represent a piece of work do be done my
  * the linker.
  */
class workrec
{
public:
    typedef boost::shared_ptr<workrec> pointer;
    typedef boost::shared_ptr<segrec> segrec_pointer;

    ~workrec();

private:
    workrec(const link_info_entry::pointer &a_refsym) :
        refsym(a_refsym),
        defsegnum(0),
        newoffset(0),
        needsrch(false),
        newproc(0)
    {
    }

public:
    static pointer create(const link_info_entry::pointer &a_refsym);

    rcstring representation(void) const;

    // symtab entry of resolving entry
    link_info_entry::pointer defsym;

    // symtab entry of unresolved name
    link_info_entry::pointer refsym;

    // segment where defsym was found
    segrec_pointer defseg;

    // segment refls point into, refrange only
    segrec_pointer refseg;

    // SEPPREF,
    // SEPFREF,
    // GLOBREF:
    // work item of homeproc
    workrec::pointer defproc;

    // UNITREF:
    // resolved seg #, def = ref
    int defsegnum;

    // PRIVREF:
    // newly assigned base offset
    int newoffset;

    // EXTPROC:
    // EXTFUNC:
    // SEPPROC:
    // SEPFUNC:
    // refs haven't been found
    bool needsrch;

    // EXTPROC:
    // EXTFUNC:
    // SEPPROC:
    // SEPFUNC:
    // proc num, comp or link chosen
    // 0 implies added proc
    int newproc;
};


#include <lib/ac/vector>

typedef std::vector<workrec::pointer> workrec_list_t;

rcstring workrec_list_representation(const workrec_list_t &wl);

#endif // UCSDPSYS_LINK_WORKREC_H
// vim: set ts=8 sw=4 et :
