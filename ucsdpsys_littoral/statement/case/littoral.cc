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

#include <lib/debug.h>
#include <lib/interval.h>

#include <ucsdpsys_littoral/statement/case/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


statement_case_littoral::~statement_case_littoral()
{
}


statement_case_littoral::statement_case_littoral(
    const expression::pointer &a_control,
    const case_list &a_cases,
    const statement::pointer &a_otherwise,
    translator_littoral *a_littoral
) :
    statement_case(a_control, a_cases, a_otherwise),
    littoral(*a_littoral)
{
}


statement_case_littoral::pointer
statement_case_littoral::create(const expression::pointer &a_control,
    const case_list &a_cases, const statement::pointer &a_otherwise,
    translator_littoral *a_littoral)
{
    return
        pointer
        (
            new statement_case_littoral
            (
                a_control,
                a_cases,
                a_otherwise,
                a_littoral
            )
        );
}


void
statement_case_littoral::traversal(void)
    const
{
    littoral.print_token("switch");
    littoral.print_space();
    littoral.print_token("(");
    get_control()->traversal(0);
    littoral.print_token(")");
    littoral.print_eoln();
    littoral.print_token("{");
    littoral.print_eoln();

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
    bool first = true;
    for (order_t::const_iterator it = order.begin(); it != order.end(); ++it)
    {
        statement::pointer sp = *it;

        if (first)
            first = false;
        else
            littoral.print_eoln();
        interval i = magic[sp];
        while (!i.empty())
        {
            interval nn(i);
            nn.first_interval_only();
            i -= nn;
            int n1 = nn.get_lowest();
            if (n1 >= 32767)
                n1 -= 65536;
            int n2 = nn.get_highest();
            if (n2 >= 32767)
                n2 -= 65536;
            for(int n = n1; n < n2; ++n)
            {
                littoral.print_token("case");
                littoral.print_space();
                rcstring v =
                    get_control()->get_type()->
                        get_human_readable_representation_of_value(n);
                littoral.print_token(v.downcase());
                littoral.print_token(":");
                littoral.print_eoln();
            }
        }

        // print the statement.
        littoral.indent_more();
        sp->traversal();
        littoral.print_token("break");
        littoral.print_token(";");
        littoral.print_eoln();
        littoral.indent_less();
    }

    if (get_otherwise())
    {
        littoral.print_eoln();
        littoral.print_token("default");
        littoral.print_token(":");
        littoral.print_eoln();
        littoral.indent_more();
        get_otherwise()->traversal();
        littoral.print_token("break");
        littoral.print_token(";");
        littoral.print_eoln();
        littoral.indent_less();
    }

    littoral.print_token("}");
    littoral.print_eoln();
}


// vim: set ts=8 sw=4 et :
