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

#include <lib/interval.h>

#include <ucsdpsys_pretty/statement/case/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


statement_case_pretty::~statement_case_pretty()
{
}


statement_case_pretty::statement_case_pretty(
    const expression::pointer &a_control,
    const case_list &a_cases,
    const statement::pointer &a_otherwise,
    translator_pretty *a_pretty
) :
    statement_case(a_control, a_cases, a_otherwise),
    pretty(*a_pretty)
{
}


statement_case_pretty::pointer
statement_case_pretty::create(const expression::pointer &a_control,
    const case_list &a_cases, const statement::pointer &a_otherwise,
    translator_pretty *a_pretty)
{
    return
        pointer
        (
            new statement_case_pretty(a_control, a_cases, a_otherwise, a_pretty)
        );
}


void
statement_case_pretty::traversal(void)
    const
{
    pretty.print_token("case");
    pretty.print_space();
    get_control()->traversal(0);
    pretty.print_space();
    pretty.print_token("of");
    pretty.print_eoln();

    int minimum = get_minimum();
    int maximum = get_maximum();
    typedef std::map<statement::pointer, interval> magic_t;
    magic_t magic;
    typedef std::list<statement::pointer> order_t;
    order_t order;
    for (int value = minimum; value <= maximum; ++value)
    {
        statement::pointer sp = get_case(value);
        if (sp)
        {
            if (std::find(order.begin(), order.end(), sp) == order.end())
                order.push_back(sp);
            magic[sp] += interval((unsigned short)value);
        }
    }
    pretty.indent_more();
    bool first_sp = true;
    for (order_t::const_iterator it = order.begin(); it != order.end(); ++it)
    {
        statement::pointer sp = *it;

        // print the case value(s)
        if (first_sp)
            first_sp = false;
        else
        {
            pretty.print_token(";");
            pretty.print_eoln();
        }
        pretty.indent_less();
        interval i = magic[sp];
        bool first_i = true;
        while (!i.empty())
        {
            if (first_i)
                first_i = false;
            else
            {
                pretty.print_token(",");
                pretty.print_space();
            }
            interval nn(i);
            nn.first_interval_only();
            i -= nn;
            int n1 = nn.get_lowest();
            if (n1 >= 32768)
                n1 -= 65536;
            int n2 = nn.get_highest() - 1;
            if (n2 >= 32768)
                n2 -= 65536;
            pretty.print_token(rcstring::printf("%d", n1));
            if (n2 > n1)
            {
                pretty.print_token("..");
                pretty.print_token(rcstring::printf("%d", n2));
            }
        }
        pretty.print_token(":");
        pretty.print_eoln();

        // print the statement.
        pretty.indent_more();
        sp->traversal();
    }

    if (get_otherwise())
    {
        pretty.print_eoln();
        pretty.indent_less();
        pretty.print_token("otherwise");
        pretty.print_eoln();
        pretty.indent_more();
        get_otherwise()->traversal();
    }
    pretty.print_eoln();
    pretty.indent_less();
    pretty.print_token("end");
}


// vim: set ts=8 sw=4 et :
