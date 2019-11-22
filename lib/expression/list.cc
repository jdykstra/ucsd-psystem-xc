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

#include <lib/ac/string.h>

#include <lib/expression.h>
#include <lib/expression/list.h>


expression_list::~expression_list()
{
    delete [] item;
    length = 0;
    maximum = 0;
    item = 0;
}


expression_list::expression_list() :
    length(0),
    maximum(0),
    item(0)
{
}


expression_list::expression_list(const expression_list &arg) :
    length(0),
    maximum(0),
    item(0)
{
    push_back(arg);
}


void
expression_list::grow(size_t by)
{
    if (length + by > maximum)
    {
        size_t new_maximum = maximum;
        for (;;)
        {
            new_maximum = 2 * new_maximum + 16;
            if (length + by <= new_maximum)
                break;
        }
        expression::pointer *new_item = new expression::pointer [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_item[j] = item[j];
        delete [] item;
        item = new_item;
        maximum = new_maximum;
    }
}


expression_list &
expression_list::operator=(const expression_list &arg)
{
    if (this != &arg)
    {
        clear();
        push_back(arg);
    }
    return *this;
}


void
expression_list::push_back(const expression::pointer &ep)
{
    assert(ep);
    grow(1);
    item[length++] = ep;
}


void
expression_list::push_front(const expression::pointer &ep)
{
    assert(ep);
    grow(1);
    for (size_t k = length; k > 0; --k)
        item[k] = item[k - 1];
    ++length;
    item[0] = ep;
}


expression::side_effect_t
expression_list::has_side_effect()
    const
{
    expression::side_effect_t result = expression::side_effect_no;
    for (size_t j = 0; j < length; ++j)
        result += item[j]->has_side_effect();
    return result;
}


expression::pointer
expression_list::pop_front()
{
    if (!length)
        return expression::pointer();
    expression::pointer result = item[0];
    --length;
    for (size_t j = 0; j < length; ++j)
        item[j] = item[j + 1];
    item[length].reset();
    return result;
}


expression::pointer
expression_list::pop_back()
{
    if (!length)
        return expression::pointer();
    --length;
    expression::pointer result = item[length];
    item[length].reset();
    return result;
}


void
expression_list::clear()
{
    while (length > 0)
    {
        --length;
        item[length].reset();
    }
}


void
expression_list::push_back(const expression_list &arg)
{
    grow(arg.size());
    for (size_t j = 0; j < arg.size(); ++j)
        item[length++] = arg.item[j];
}


location
expression_list::get_location()
    const
{
    location result;
    for (size_t j = 0; j < length; ++j)
        result += item[j]->get_location();
    return result;
}

expression::pointer
expression_list::get(size_t n)
    const
{
    assert(n < length);
    assert(item);
    assert(item[n]);
    return item[n];
}


rcstring
expression_list::lisp_representation(void)
    const
{
    rcstring result("(expresion-list");
    for (size_t j = 0; j < length; ++j)
        result += " " + item[j]->lisp_representation();
    result += ")";
    return result;
}


expression_list
expression_list::tail(void)
    const
{
    expression_list result;
    for (size_t j = 1; j < length; ++j)
        result.push_back(item[j]);
    return result;
}


expression_list
expression_list::optimize(void)
    const
{
    expression_list result;
    for (size_t j = 0; j < length; ++j)
        result.push_back(item[j]->optimize());
    return result;
}


expression_list
expression_list::clone(const location &locn)
    const
{
    expression_list result;
    for (size_t j = 0; j < length; ++j)
        result.push_back(item[j]->clone(locn));
    return result;
}


// vim: set ts=8 sw=4 et :
