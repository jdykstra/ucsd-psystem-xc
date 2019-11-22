//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#include <lib/debug.h>
#include <lib/pascal/lex.h>
#include <lib/tcontrol/list.h>


tcontrol_list::~tcontrol_list()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


tcontrol_list::tcontrol_list()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


tcontrol_list::pointer
tcontrol_list::create(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new tcontrol_list());
}


void
tcontrol_list::set(const rcstring_list &args)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "size now %u", (unsigned)dispatch.size());
    //
    // Make sure our parameter list is sane enough to proceed.
    //
    switch (args.size())
    {
    case 0:
        pascal_grammar_error
        (
            "expected more parameters"
        );
        DEBUG(1, "}");
        return;

    case 1:
        pascal_lex_error
        (
            pascal_lex_location(),
            "%s: requires more parameters",
            args[0].c_str()
        );
        DEBUG(1, "}");
        return;

    default:
        break;
    }

    //
    // locate the sub-tcontrol
    //
    rcstring name = args[1];
    rcstring lc_name = name.downcase();
    DEBUG(2, "lc_name = %s", lc_name.quote_c().c_str());
    tcontrol::pointer *p = dispatch.query(lc_name);
    if (!p)
    {
        rcstring guess = dispatch.query_fuzzy(lc_name);
        DEBUG(2, "guess = %s", guess.quote_c().c_str());
        if (!guess.empty())
            p = dispatch.query(guess);
        if (!p)
        {
            pascal_lex_error
            (
                pascal_lex_location(),
                "%s %s unknown",
                args[0].c_str(),
                lc_name.quote_c().c_str()
            );
            DEBUG(1, "}");
            return;
        }

        //
        // for example
        //
        //     warning "empty-parents" unknown, guessing you meant
        //     "empty-parens" instead.
        //
        pascal_lex_error
        (
            pascal_lex_location(),
            "%s %s unknown, guessing you meant %s instead",
            args[0].c_str(),
            lc_name.quote_c().c_str(),
            guess.quote_c().c_str()
        );
    }

    //
    // set the sub-tcontrol
    //
    DEBUG(2, "about to run");
    (*p)->set(args.tail());
    DEBUG(1, "}");
}


void
tcontrol_list::register_command(const rcstring &name,
    const tcontrol::pointer &sub_tcontrol)
{
    rcstring lc_name = name.downcase();
    DEBUG(1, "name = %s", lc_name.quote_c().c_str());
    dispatch.assign(lc_name, sub_tcontrol);
    DEBUG(2, "size now %u", (unsigned)dispatch.size());
}


// vim: set ts=8 sw=4 et :
