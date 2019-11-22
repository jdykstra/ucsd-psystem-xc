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

#include <lib/expression/char.h>
#include <lib/type/char.h>


expression_char::~expression_char()
{
}


expression_char::expression_char(const location &a_loc, unsigned char a_val) :
    expression(a_loc),
    value(a_val)
{
}


expression_char::expression_char(const expression_char &arg) :
    expression(arg),
    value(arg.value)
{
}


void
expression_char::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(0);
}


int
expression_char::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_char::get_type()
    const
{
    return type_char::create();
}


bool
expression_char::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_char::has_side_effect()
    const
{
    return side_effect_no;
}

bool
expression_char::is_constant()
    const
{
    return true;
}


long
expression_char::get_integer_value()
    const
{
    return value;
}


rcstring
expression_char::lisp_representation(void)
    const
{
    char c[1];
    c[0] = value;
    return "(char " + rcstring(c, 1).quote_c() + ")";
}


// vim: set ts=8 sw=4 et :
