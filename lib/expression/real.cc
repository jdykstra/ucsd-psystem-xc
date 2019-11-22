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

#include <lib/expression/real.h>
#include <lib/pascal/lex.h>
#include <lib/type/real.h>


expression_real::~expression_real()
{
}


expression_real::expression_real(const expression_real &arg) :
    expression(arg),
    value(arg.value)
{
}


expression_real::expression_real(const location &locn, const rcstring &arg) :
    expression(locn),
    value(atof(arg.c_str()))
{
}


expression_real::expression_real(const location &locn, double arg) :
    expression(locn),
    value(arg)
{
}


int
expression_real::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_real::get_type()
    const
{
    return type_real::create();
}


bool
expression_real::is_lvalue()
    const
{
    return false;
}


bool
expression_real::is_constant()
    const
{
    return true;
}


expression::side_effect_t
expression_real::has_side_effect()
    const
{
    return side_effect_no;
}


rcstring
expression_real::lisp_representation(void)
    const
{
    return rcstring::printf("(real %g)", value);
}


// vim: set ts=8 sw=4 et :
