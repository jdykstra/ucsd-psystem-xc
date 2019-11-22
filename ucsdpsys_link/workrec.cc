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

#include <ucsdpsys_link/workrec.h>


workrec::~workrec()
{
}


workrec::pointer
workrec::create(const link_info_entry::pointer &a_refsym)
{
    return pointer(new workrec(a_refsym));
}


rcstring
workrec::representation(void)
    const
{
    // symtab entry of unresolved name
    assert(refsym);
    rcstring_accumulator buf;
    buf.push_back("{ refsym = ");
    buf.push_back(refsym->representation());

    // symtab entry of resolving name
    if (defsym)
    {
        buf.push_back(", defsym = ");
        buf.push_back(defsym->representation());
    }

    // segment where defsym was found
    if (defseg)
        buf.printf(", defseg = %p", defseg.get());

    // segment refls point into, refrange only
    if (refseg)
        buf.printf(", refseg = %p", refseg.get());

    switch (refsym->get_litype())
    {
    case CONSTDEF:
    case CONSTREF:
    case EOFMARK:
    case GLOBDEF:
    case PUBLDEF:
    case PUBLREF:
        break;

    case SEPPREF:
    case SEPFREF:
    case GLOBREF:
        buf.printf(", defproc = %p", defproc.get());
        break;

    case UNITREF:
        buf.printf(", defsegnum = %d", defsegnum);
        break;

    case PRIVREF:
        buf.printf(", newoffset = %d", newoffset);
        break;

    case EXTPROC:
    case EXTFUNC:
    case SEPPROC:
    case SEPFUNC:
        buf.printf(", needsrch = %d", needsrch);
        buf.printf(", newproc = %d", newproc);
        break;
    }

    buf.push_back(" }");
    return buf.mkstr();
}


rcstring
workrec_list_representation(const workrec_list_t &wl)
{
    rcstring_accumulator buf;
    buf.push_back('{');
    for (workrec_list_t::const_iterator it = wl.begin(); it != wl.end(); ++it)
    {
        if (it != wl.begin())
            buf.push_back(',');
        buf.push_back(' ');
        buf.push_back((*it)->representation());
    }
    buf.push_back(" }");
    return buf.mkstr();
}


// vim: set ts=8 sw=4 et :
