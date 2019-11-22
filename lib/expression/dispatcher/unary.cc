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

#include <lib/ac/assert.h>

#include <lib/expression.h>
#include <lib/expression/dispatcher/unary.h>
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/type.h>


expression_dispatcher_unary::~expression_dispatcher_unary()
{
    delete [] item;
    length = 0;
    maximum = 0;
    item = 0;
}


expression_dispatcher_unary::expression_dispatcher_unary(const char *arg) :
    operator_name(arg),
    length(0),
    maximum(0),
    item(0)
{
}


expression::pointer
expression_dispatcher_unary::dispatch(const expression::pointer &operand,
        translator *context)
    const
{
    //
    // Do not generate secondary errors.
    // Silently fail if we have failed before.
    //
    if (operand->is_error())
    {
        return expression_error::create(operand->get_location());
    }

    //
    // Look for a patch in the dispatch table, and use it if you find one.
    //
    for (size_t j = 0; j < length; ++j)
    {
        const dispatch_t &ref = item[j];
        if (ref.matches(operand))
            return ref.dispatch(operand, context);
    }

    //
    // Nothing matches.  Report the error, and then return an error expression.
    //
    return complain(operand);
}


expression::pointer
expression_dispatcher_unary::complain(const expression::pointer &operand)
    const
{
    if (operand->is_error())
        return operand;
    pascal_lex_error
    (
        operand->get_location(),
        "invalid %s expression, given a %s",
        operator_name,
        operand->get_description().c_str()
    );
    return expression_error::create(operand->get_location());
}


void
expression_dispatcher_unary::push_back(const dispatch_t &arg)
{
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 16;
        dispatch_t *new_item = new dispatch_t [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_item[j] = item[j];
        delete [] item;
        item = new_item;
        maximum = new_maximum;
    }
    item[length++] = arg;
}


void
expression_dispatcher_unary::push_back(test_t operand_test, factory_t factory)
{
    push_back(dispatch_t(operand_test, factory));
}


static bool
any(const expression::pointer &)
{
    return true;
}


expression_dispatcher_unary::dispatch_t::~dispatch_t()
{
}


expression_dispatcher_unary::dispatch_t::dispatch_t() :
    operand_test(any),
    factory(0)
{
}


expression_dispatcher_unary::dispatch_t::dispatch_t(test_t a_operand_test,
        factory_t a_factory) :
    operand_test(a_operand_test ? a_operand_test : any),
    factory(a_factory)
{
}


expression_dispatcher_unary::dispatch_t::dispatch_t(const dispatch_t &arg) :
    operand_test(arg.operand_test),
    factory(arg.factory)
{
}


expression_dispatcher_unary::dispatch_t &
expression_dispatcher_unary::dispatch_t::operator=(const dispatch_t &arg)
{
    if (this != &arg)
    {
        operand_test = arg.operand_test;
        factory = arg.factory;
    }
    return *this;
}


bool
expression_dispatcher_unary::dispatch_t::matches(
        const expression::pointer &operand)
    const
{
    return operand_test(operand);
}


expression::pointer
expression_dispatcher_unary::dispatch_t::dispatch(
        const expression::pointer &operand, translator *context)
    const
{
    assert(matches(operand));
    return (context->*factory)(operand);
}


// vim: set ts=8 sw=4 et :
