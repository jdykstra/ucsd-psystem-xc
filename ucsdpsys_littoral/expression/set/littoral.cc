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

#include <ucsdpsys_littoral/expression/set/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_set_littoral::~expression_set_littoral()
{
}


expression_set_littoral::expression_set_littoral(
    const location &a_locn,
    const type::pointer &a_tp,
    const set &a_value,
    translator_littoral *cntxt
) :
    expression_set(a_locn, a_tp, a_value),
    littoral(*cntxt)
{
}


expression_set_littoral::pointer
expression_set_littoral::create(const location &a_locn,
    const type::pointer &a_tp, const set &a_value, translator_littoral *cntxt)
{
    return pointer(new expression_set_littoral(a_locn, a_tp, a_value, cntxt));
}


void
expression_set_littoral::traversal(int)
    const
{
    const set &val = get_value();
    if (val.empty())
    {
        littoral.print_token("set()");
        return;
    }
    littoral.print_token("set(");
    size_t n = val.size() << 4;
    bool first = true;
    for (size_t j = 0; j < n; ++j)
    {
        if (val.get_bit(j))
        {
            if (!first)
            {
                littoral.print_token(",");
                littoral.print_space();
            }
            // FIXME: it could be an enum or a char
            // so we may need to do something different.
            littoral.print_token(rcstring::printf("%u", (unsigned)j));
            size_t k = j;
            while (val.get_bit(k + 1))
                ++k;
            if (k > j)
            {
                littoral.print_token("..");
                littoral.print_token(rcstring::printf("%u", (unsigned)k));
                j = k;
            }
            first = false;
        }
    }
    littoral.print_token(")");
}


expression::pointer
expression_set_littoral::optimize(void)
    const
{
    return create(get_location(), get_type(), get_value(), &littoral);
}


expression::pointer
expression_set_littoral::clone(const location &locn)
    const
{
    return create(locn, get_type(), get_value(), &littoral);
}


// vim: set ts=8 sw=4 et :
