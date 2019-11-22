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

#include <lib/expression/string.h>
#include <lib/pascal/lex.h>
#include <lib/type/string.h>


expression_string::~expression_string()
{
}


expression_string::expression_string(
    const location &locn,
    const rcstring &arg
) :
    expression(locn),
    value(arg)
{
}


int
expression_string::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_string::get_type()
    const
{
    size_t len = value.size();
    if (len < 1)
        len = 1;
    else if (len > 255)
        len = 255;
    return type_string::create(len);
}


bool
expression_string::is_lvalue()
    const
{
    return false;
}


bool
expression_string::is_constant()
    const
{
    return true;
}


expression::side_effect_t
expression_string::has_side_effect()
    const
{
    return side_effect_no;
}


rcstring
expression_string::lisp_representation(void)
    const
{
    return
        (
            "(string {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            value.quote_c()
        +
            ")"
        );
}


rcstring
expression_string::get_printable_value(void)
    const
{
    return value;
}


// vim: set ts=8 sw=4 et :
