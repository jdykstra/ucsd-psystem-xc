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

#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/translator.h>
#include <lib/type/integer.h>


expression::pointer
translator::modulo_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    if (lhs->is_error())
        return lhs;
    if (rhs->is_error())
        return rhs;
    if (!type_integer::is_a(lhs) || !type_integer::is_a(rhs))
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "invalid modulo expression, given a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }
    return modulo_expression_factory(lhs, rhs);
}


// vim: set ts=8 sw=4 et :
