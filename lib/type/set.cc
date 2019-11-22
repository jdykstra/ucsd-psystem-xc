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

#include <lib/expression.h>
#include <lib/set.h>
#include <lib/type/integer.h>
#include <lib/type/set.h>
#include <lib/type/subrange.h>


type_set::~type_set()
{
}


type::pointer
type_set::bounded(const type::pointer &tp)
{
    //
    // For integers, use the biggest valid subrange, it will be
    // reducared later by the compiler during expression::optimize().
    //
    // FIXME: enumerated type could be a problem, if there were
    // (set::maximum_range + 1) enumerated constants defined.
    //
    if (boost::dynamic_pointer_cast<type_integer>(tp))
        return type_subrange::create(0, set::range_maximum, tp);

    //
    // Check that subrange types are small enough.
    //
    type_subrange::pointer tsp = boost::dynamic_pointer_cast<type_subrange>(tp);
    if
    (
        tsp
    &&
        (
            tsp->get_maximum_value() > set::range_maximum
        ||
            tsp->get_minimum_value() < 0
        )
    )
        return type_subrange::create(0, set::range_maximum, tp->get_subtype());

    //
    // Looks safe enough.
    //
    return tp;
}


type_set::type_set(const type::pointer &a_subtype) :
    subtype(bounded(a_subtype))
{
    assert(!boost::dynamic_pointer_cast<type_integer>(get_subtype()));
}


rcstring
type_set::get_name()
    const
{
    return "set of " + subtype->get_name();
}


rcstring
type_set::get_pascal_name()
    const
{
    return "set of " + subtype->get_pascal_name();
}


rcstring
type_set::get_c_declaration(const rcstring &vname)
    const
{
    return ("set " + vname);
}


type::pointer
type_set::get_subtype()
    const
{
    return subtype;
}


bool
type_set::same_type_as(const type::pointer &rhs)
    const
{
    return (is_a(rhs) && subtype == rhs->get_subtype());
}


bool
type_set::is_a_variable()
    const
{
    return true;
}


bool
type_set::is_a(const type::pointer &arg)
{
    return is_a(arg.get());
}


bool
type_set::is_a(const type *arg)
{
    return (0 != dynamic_cast<const type_set *>(arg));
}


bool
type_set::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


void
type_set::name_hint(const rcstring &suggestion)
{
    subtype->name_hint(suggestion);
}


bool
type_set::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_set::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :
