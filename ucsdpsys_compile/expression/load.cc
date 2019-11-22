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

#include <lib/label.h>
#include <lib/type.h>
#include <ucsdpsys_compile/expression/load.h>


expression_load::~expression_load()
{
}


expression_load::expression_load(const location &locn,
        const type::pointer &a_type) :
    expression(locn),
    var_type(a_type)
{
}


bool
expression_load::is_lvalue()
    const
{
    return true;
}


type::pointer
expression_load::get_type()
    const
{
    return var_type;
}


int
expression_load::get_precedence()
    const
{
    return prec_unary;
}


expression::side_effect_t
expression_load::has_side_effect()
    const
{
    return side_effect_no;
}


// vim: set ts=8 sw=4 et :
