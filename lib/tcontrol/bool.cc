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

#include <lib/cardinal.h>
#include <lib/tcontrol/bool.h>
#include <lib/pascal/lex.h>
#include <lib/sizeof.h>


tcontrol_bool::~tcontrol_bool()
{
}


tcontrol_bool::tcontrol_bool()
{
}


tcontrol_bool::tcontrol_bool(const tcontrol_bool &rhs) :
    tcontrol(rhs)
{
}


tcontrol_bool &
tcontrol_bool::operator=(const tcontrol_bool &rhs)
{
    if (this != &rhs)
    {
        tcontrol::operator=(rhs);
    }
    return *this;
}


static int
interpret(const rcstring &arg)
{
    struct table_t
    {
        const char *name;
        int value;
    };

    static const table_t table[] =
    {
        { "+", 1 },
        { "-", 0 },
        { "disable", 0 },
        { "enable", 1 },
        { "false", 0 },
        { "no", 0 },
        { "ok", 1 },
        { "true", 1 },
        { "yes", 1 },
    };

    rcstring text = arg.downcase();
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (text == tp->name)
            return tp->value;
    }
    return -1;
}


bool
tcontrol_bool::looks_boolean(const rcstring &arg)
{
    return (interpret(arg) >= 0);
}


bool
tcontrol_bool::check_value_implicit(const rcstring_list &args, bool dflt)
{
    switch (args.size())
    {
    case 0:
    case 1:
        return dflt;

    case 2:
        break;

    default:
        pascal_lex_error
        (
            pascal_lex_location(),
            "setting the %s control requires one parameter, not %s",
            args[0].quote_c().c_str(),
            cardinal(args.size() - 1).c_str()
        );
        break;
    }

    int value = interpret(args[1]);

    if (value < 0)
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "setting the %s boolean: value %s not understood, "
                "expected \"true\" or \"false\"",
            args[0].quote_c().c_str(),
            args[1].quote_c().c_str()
        );
        return dflt;
    }

    return (value > 0);
}


bool
tcontrol_bool::check_value(const rcstring_list &args, bool dflt)
{
    switch (args.size())
    {
    case 0:
        pascal_grammar_error
        (
            "setting a boolean control requires an parameter"
        );
        return dflt;

    default:
        pascal_lex_error
        (
            pascal_lex_location(),
            "setting the %s control requires one parameter, not %s",
            args[0].quote_c().c_str(),
            cardinal(args.size() - 1).c_str()
        );
        return dflt;

    case 2:
        return check_value_implicit(args, dflt);
    }
}


// vim: set ts=8 sw=4 et :
