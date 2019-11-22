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
#include <lib/expression/dispatcher/binary.h>
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/type.h>


expression_dispatcher_binary::~expression_dispatcher_binary()
{
}


expression_dispatcher_binary::expression_dispatcher_binary(const char *arg) :
    operator_name(arg)
{
}


expression::pointer
expression_dispatcher_binary::dispatch(const expression::pointer &lhs,
    const expression::pointer &rhs) const
{
    //
    // Do not generate secondary errors.  Silently fail if we have
    // failed before.
    //
    if (lhs->is_error())
        return lhs;
    if (rhs->is_error())
        return rhs;

    //
    // Look for a match in the dispatch table, and use it if you find one.
    //
    for (items_t::const_iterator it = items.begin(); it != items.end(); ++it)
    {
        const dispatch_t &ref = *it;
        if (ref.matches(lhs, rhs))
            return ref.dispatch(lhs, rhs);
    }

    //
    // Nothing matches.  Report the error, and then return an error expression.
    //
    return complain(lhs, rhs);
}


expression::pointer
expression_dispatcher_binary::complain(const expression::pointer &lhs,
    const expression::pointer &rhs) const
{
    if (lhs->is_error())
        return lhs;
    if (rhs->is_error())
        return rhs;
    location locn = lhs->get_location() + rhs->get_location();
    pascal_lex_error
    (
        locn,
        "invalid %s expression, given a %s and a %s",
        operator_name,
        lhs->get_description().c_str(),
        rhs->get_description().c_str()
    );
    return expression_error::create(locn);
}


void
expression_dispatcher_binary::push_back(const dispatch_t &arg)
{
    items.push_back(arg);
}


void
expression_dispatcher_binary::push_back(test_t lhs_test,
    const expression_dispatcher_binary_functor::pointer &factory,
    test_t rhs_test)
{
    push_back(dispatch_t(lhs_test, factory, rhs_test));
}


static bool
any(const expression::pointer &)
{
    return true;
}


expression_dispatcher_binary::dispatch_t::~dispatch_t()
{
}


expression_dispatcher_binary::dispatch_t::dispatch_t() :
    lhs_test(any),
    factory(),
    rhs_test(any)
{
}


expression_dispatcher_binary::dispatch_t::dispatch_t(
    test_t a_lhs_test,
    const expression_dispatcher_binary_functor::pointer &a_factory,
    test_t a_rhs_test
) :
    lhs_test(a_lhs_test ? a_lhs_test : any),
    factory(a_factory),
    rhs_test(a_rhs_test ? a_rhs_test : any)
{
}


expression_dispatcher_binary::dispatch_t::dispatch_t(const dispatch_t &arg) :
    lhs_test(arg.lhs_test),
    factory(arg.factory),
    rhs_test(arg.rhs_test)
{
}


expression_dispatcher_binary::dispatch_t &
expression_dispatcher_binary::dispatch_t::operator=(const dispatch_t &arg)
{
    if (this != &arg)
    {
        lhs_test = arg.lhs_test;
        factory = arg.factory;
        rhs_test = arg.rhs_test;
    }
    return *this;
}


bool
expression_dispatcher_binary::dispatch_t::matches(
    const expression::pointer &lhs, const expression::pointer &rhs) const
{
    return (lhs_test(lhs) && rhs_test(rhs));
}


expression::pointer
expression_dispatcher_binary::dispatch_t::dispatch(
    const expression::pointer &lhs, const expression::pointer &rhs) const
{
    assert(matches(lhs, rhs));
    return factory->action(lhs, rhs);
}


// vim: set ts=8 sw=4 et :
