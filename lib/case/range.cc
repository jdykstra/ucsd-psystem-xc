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
#include <lib/case/range.h>
#include <lib/pascal/lex.h>


case_range::~case_range()
{
}


case_range::case_range(const type::pointer &a_base) :
    base(a_base),
    minimum(0),
    maximum(0)
{
}


case_range::case_range(const case_range &arg) :
    base(arg.base),
    minimum(arg.minimum),
    maximum(arg.maximum),
    values(arg.values)
{
}


case_range &
case_range::operator=(const case_range &arg)
{
    if (this != &arg)
    {
        base = arg.base;
        minimum = arg.minimum;
        maximum = arg.maximum;
        values = arg.values;
    }
    return *this;
}


void
case_range::push_back(const expression::pointer &ep)
{
    assert(ep->is_constant());
    word_t value = ep->optimize()->get_integer_value();
    push_back(value, ep);
}


void
case_range::push_back(word_t value, const expression::pointer &ep)
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
            it->second->get_location(),
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

    values.insert(values_t::value_type(value, ep));
}


void
case_range::push_back(const expression::pointer &ep1,
    const expression::pointer &ep2)
{
    assert(ep1->is_constant());
    word_t value1 = ep1->optimize()->get_integer_value();
    assert(ep2->is_constant());
    word_t value2 = ep2->optimize()->get_integer_value();

    for (word_t value = value1; value < value2; ++value)
    {
        push_back(value, ep1);
    }
    push_back(value2, ep2);
}


void
case_range::operator+=(const case_range &rhs)
{
    for
    (
        values_t::const_iterator it = rhs.values.begin();
        it != rhs.values.end();
        ++it
    )
    {
        push_back(it->first, it->second);
    }
}


case_range
case_range::operator+(const case_range &rhs)
    const
{
    case_range result(*this);
    result += rhs;
    return result;
}


void
case_range::push_into_list(case_list &dest, const statement::pointer &sp)
    const
{
    for (values_t::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        dest.push_back(it->first, it->second, sp);
    }
}


// vim: set ts=8 sw=4 et :
