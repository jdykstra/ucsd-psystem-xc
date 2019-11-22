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

#include <lib/ac/string.h>

#include <lib/cardinal.h>
#include <lib/debug.h>
#include <lib/expression/list.h>
#include <lib/rcstring/accumulator.h>
#include <lib/rcstring/list.h>
#include <lib/type/string.h>

#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pseudo/macro.h>


opcode_pseudo_macro::~opcode_pseudo_macro()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


opcode_pseudo_macro::opcode_pseudo_macro(
    const rcstring &a_name,
    const rcstring &a_body,
    assembler *a_context
) :
    opcode_pseudo(a_context),
    name(a_name),
    body(a_body)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "name = %s", name.quote_c().c_str());
}


opcode_pseudo_macro::pointer
opcode_pseudo_macro::create(const rcstring &a_name, const rcstring &a_body,
    assembler *a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new opcode_pseudo_macro(a_name, a_body, a_context));
}


void
opcode_pseudo_macro::interpret(const expression_list &args)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring_list params;
    switch (args.size())
    {
    case 0:
        break;

    case 1:
        assert(type_string::is_a(args[0]));
        // FIXME: do we need to count '<' and '>' pairs?
        params.split(args[0]->get_string_value(), ",", true);
        break;

    default:
        assert(!"this should not happen");
    }

    rcstring_accumulator ac;
    const char *cp = body.c_str();
    const char *end = cp + body.size();
    while (*cp)
    {
        const char *p = strchr(cp, '%');
        if (!p)
        {
            ac.push_back(cp, end - cp);
            break;
        }
        ac.push_back(cp, p - cp);
        cp = p + 1;
        switch (*cp)
        {
        default:
            ac.push_back('%');
            break;

        case '%':
            ++cp;
            ac.push_back('%');
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                int n = *cp - '1';
                assert(n >= 0);
                ++cp;
                if ((size_t)n < params.size())
                    ac.push_back(params[n]);
            }
            break;
        }
    }
    DEBUG(1, "body = %s", ac.mkstr().quote_c().c_str());
    lex_macro_insert(ac.mkstr());
}


rcstring
opcode_pseudo_macro::get_name(void)
    const
{
    return name;
}


bool
opcode_pseudo_macro::next_argument_always_a_string(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
