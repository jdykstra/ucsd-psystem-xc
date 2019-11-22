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

#include <lib/pascal/lex.h>
#include <lib/tcontrol/too_late.h>
#include <lib/translator.h>


tcontrol_too_late::~tcontrol_too_late()
{
}


tcontrol_too_late::tcontrol_too_late(
    translator *a_xlat,
    const tcontrol::pointer &a_deeper
) :
    xlat(a_xlat),
    deeper(a_deeper)
{
}


tcontrol_too_late::pointer
tcontrol_too_late::create(translator *a_xlat, const tcontrol::pointer &a_deeper)
{
    return pointer(new tcontrol_too_late(a_xlat, a_deeper));
}


void
tcontrol_too_late::set(const rcstring_list &args)
{
    assert(!args.empty());
    if (xlat->control_comment_too_late())
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "control comment %s too late in source code",
            args[0].quote_c().c_str()
        );
    }
    else
    {
        deeper->set(args);
    }
}


// vim: set ts=8 sw=4 et :
