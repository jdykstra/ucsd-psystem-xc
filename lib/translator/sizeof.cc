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
#include <lib/pascal/lex.h>
#include <lib/translator.h>
#include <lib/type/error.h>


expression::pointer
translator::sizeof_expression(const expression::pointer &arg)
{
    //
    // Suppress secondary errors.
    //
    if (arg->is_error())
        return arg;

    //
    // Make sure this is a sensable type
    //
    unsigned nbytes = arg->get_type()->get_size_in_bytes();
    if (nbytes == 0)
    {
        pascal_lex_error
        (
            arg->get_location(),
            "sizeof %s not appropriate",
            arg->get_description().c_str()
        );
        return expression_error::create(arg->get_location());
    }

    //
    // Call the translator-specific method.
    //
    return sizeof_expression_factory(arg);
}


expression::pointer
translator::sizeof_expression_factory(const expression::pointer &arg)
{
    return sizeof_expression_factory(arg->get_location(), arg->get_type());
}


expression::pointer
translator::sizeof_expression(const location &locn, const type::pointer &arg)
{
    //
    // Suppress secondary errors.
    //
    if (type_error::is_a(arg))
        return expression_error::create(pascal_lex_location());

    //
    // Make sure this is a sensable type
    //
    unsigned nbytes = arg->get_size_in_bytes();
    if (nbytes == 0)
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "sizeof %s not appropriate",
            arg->get_name().c_str()
        );
        return expression_error::create(pascal_lex_location());
    }

    //
    // Call the translator-specific method.
    //
    return sizeof_expression_factory(locn, arg);
}


// vim: set ts=8 sw=4 et :
