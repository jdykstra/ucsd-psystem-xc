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

#include <lib/case/list.h>
#include <lib/pascal/lex.h>


case_list::~case_list()
{
}


case_list::case_list(const type::pointer &a_base) :
    base(a_base),
    minimum(0),
    maximum(0)
{
}


case_list::case_list(const case_list &arg) :
    base(arg.base),
    minimum(arg.minimum),
    maximum(arg.maximum),
    values(arg.values)
{
}


case_list &
case_list::operator=(const case_list &arg)
{
    if (this != &arg)
    {
        base = arg.base;
        values.clear();
        push_back(arg);
    }
    return *this;
}


void
case_list::push_back(word_t value, const expression::pointer &ep,
    const statement::pointer &sp)
{
    values_t::const_iterator it = values.find(value);
    if (it != values.end())
    {
        pascal_lex_error
        (
            ep->get_location(),
            "duplicate case value %s...",
            base->get_human_readable_representation_of_value(value).c_str()
        );
        pascal_lex_error
        (
            it->second.ep->get_location(),
            "... here is the earlier case"
        );
        return;
    }

    if (values.empty())
    {
        minimum = value;
        maximum = value;
    }
    else
    {
        if (minimum > value)
            minimum = value;
        if (maximum < value)
            maximum = value;
    }

    values.insert(values_t::value_type(value, mapping(ep, sp)));
}


void
case_list::push_back(const case_list &rhs)
{
    for
    (
        values_t::const_iterator it = rhs.values.begin();
        it != rhs.values.end();
        ++it
    )
    {
        push_back(it->first, it->second.ep, it->second.sp);
    }
}


bool
case_list::execution_flows_out()
    const
{
    for
    (
        values_t::const_iterator it = values.begin();
        it != values.end();
        ++it
    )
    {
        if (!it->second.sp->execution_flows_out())
            return false;
    }
    return true;
}


statement::pointer
case_list::get_case(word_t value)
    const
{
    assert(value >= minimum);
    assert(value <= maximum);
    values_t::const_iterator it = values.find(value);
    if (it == values.end())
        return statement::pointer();
    return it->second.sp;
}


// vim: set ts=8 sw=4 et :
