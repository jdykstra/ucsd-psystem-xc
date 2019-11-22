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

#include <lib/ac/stdlib.h>

#include <lib/expression/integer.h>
#include <lib/pascal/lex.h>
#include <lib/type/integer.h>


expression_integer::~expression_integer()
{
}


expression_integer::expression_integer(const expression_integer &arg) :
    expression(arg),
    value(arg.value)
{
}


expression_integer::expression_integer(
    const rcstring &arg,
    const location &a_locn
) :
    expression(a_locn),
    value(pascal_lex_integer_constant_to_int(arg.c_str()))
{
}


expression_integer::expression_integer(long arg, const location &a_locn) :
    expression(a_locn),
    value(arg)
{
}


int
expression_integer::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_integer::get_type()
    const
{
    return type_integer::create();
}


bool
expression_integer::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_integer::has_side_effect()
    const
{
    return side_effect_no;
}


bool
expression_integer::is_constant()
    const
{
    return true;
}


long
expression_integer::get_integer_value()
    const
{
    return value;
}


rcstring
expression_integer::lisp_representation(void)
    const
{
    return
        rcstring::printf
        (
            "(integer {%s} %ld)",
            get_type()->get_pascal_name().c_str(),
            value
        );
}


rcstring
expression_integer::get_printable_value(void)
    const
{
    return rcstring::printf("%ld", value);
}


// vim: set ts=8 sw=4 et :
