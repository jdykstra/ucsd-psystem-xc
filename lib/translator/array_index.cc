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

#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/translator.h>
#include <lib/type/array.h>
#include <lib/type/integer.h>
#include <lib/type/string.h>
#include <lib/type/subrange.h>


expression::pointer
translator::array_index_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    if (lhs->is_error())
        return lhs;
    if (rhs->is_error())
        return rhs;

    // Strings are a special case of "array [0..N] of char"
    // but the user can't get at [0] without (*$R-*).
    type_string::pointer tsp =
        boost::dynamic_pointer_cast<type_string>(lhs->get_type());
    if (tsp)
    {
        type::pointer index_type = type_integer::create();
        if (rhs->get_type() != index_type)
        {
            pascal_lex_error
            (
                lhs->get_location(),
                "array index must be of %s type, not a %s",
                index_type->get_name().c_str(),
                rhs->get_description().c_str()
            );
            location locn = lhs->get_location() + rhs->get_location();
            return expression_error::create(locn);
        }

        return lhs->array_index_expression_factory(rhs);
    }

    const type_array *tap =
        dynamic_cast<const type_array *>(lhs->get_type().operator->());
    if (!tap)
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "expected array type, not a %s",
            lhs->get_description().c_str()
        );
        location locn = lhs->get_location() + rhs->get_location();
        return expression_error::create(locn);
    }
    if (!lhs->is_lvalue())
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "only array variables may be indexed, "
                "but an array value was given"
        );
        location locn = lhs->get_location() + rhs->get_location();
        return expression_error::create(locn);
    }
    type::pointer index_type = tap->get_index_type();
    if (type_subrange::is_a(index_type))
        index_type = index_type->get_subtype();
    if (index_type != rhs->get_type())
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "array index must be of %s type, not a %s",
            index_type->get_name().c_str(),
            rhs->get_description().c_str()
        );
        location locn = lhs->get_location() + rhs->get_location();
        return expression_error::create(locn);
    }

    //
    // Generate the real array index expression, now that we know the
    // parameters are valid.
    //
    return lhs->array_index_expression_factory(rhs);
}


expression::pointer
translator::array_index_expression(const expression::pointer &lhs,
    const expression_list &rhs)
{
    assert(!rhs.empty());
    expression::pointer ep = lhs;
    for (size_t j = 0; j < rhs.size(); ++j)
        ep = array_index_expression(ep, rhs[j]);
    return ep;
}


// vim: set ts=8 sw=4 et :
