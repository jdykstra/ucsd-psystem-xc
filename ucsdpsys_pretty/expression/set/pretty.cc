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

#include <ucsdpsys_pretty/expression/set/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_set_pretty::~expression_set_pretty()
{
}


expression_set_pretty::expression_set_pretty(
    const location &a_locn,
    const type::pointer &a_tp,
    const set &a_value,
    translator_pretty *cntxt
) :
    expression_set(a_locn, a_tp, a_value),
    pretty(*cntxt)
{
}


expression_set_pretty::pointer
expression_set_pretty::create(const location &a_locn, const type::pointer &a_tp,
    const set &a_value, translator_pretty *cntxt)
{
    return pointer(new expression_set_pretty(a_locn, a_tp, a_value, cntxt));
}


void
expression_set_pretty::traversal(int)
    const
{
    const set &val = get_value();
    pretty.print_token("[");
    size_t n = val.size() << 4;
    bool first = true;
    for (size_t j = 0; j < n; ++j)
    {
        if (val.get_bit(j))
        {
            if (!first)
            {
                pretty.print_token(",");
                pretty.print_space();
            }
            // FIXME: it could be an enum or a char
            // so we may need to do something different.
            pretty.print_token(rcstring::printf("%u", (unsigned)j));
            size_t k = j;
            while (val.get_bit(k + 1))
                ++k;
            if (k > j)
            {
                pretty.print_token("..");
                pretty.print_token(rcstring::printf("%u", (unsigned)k));
                j = k;
            }
            first = false;
        }
    }
    pretty.print_token("]");
}


expression::pointer
expression_set_pretty::optimize(void)
    const
{
    return create(get_location(), get_type(), get_value(), &pretty);
}


expression::pointer
expression_set_pretty::clone(const location &locn)
    const
{
    return create(locn, get_type(), get_value(), &pretty);
}


// vim: set ts=8 sw=4 et :
