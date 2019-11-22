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

#include <ucsdpsys_assemble/expression/special.h>
#include <ucsdpsys_assemble/type/special.h>


expression_special::~expression_special()
{
}


expression_special::expression_special(const location &locn) :
    expression(locn)
{
}


expression::side_effect_t
expression_special::has_side_effect(void)
    const
{
    return side_effect_no;
}


bool
expression_special::is_lvalue(void)
    const
{
    return false;
}


type::pointer
expression_special::get_type(void)
    const
{
    return type_special::create();
}


rcstring
expression_special::lisp_representation(void)
    const
{
    return ("(special " + get_name() + ")");
}


int
expression_special::get_precedence(void)
    const
{
    return prec_id;
}


void
expression_special::traversal(int)
    const
{
    assert(!"should not happen");
}


void
expression_special::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(!"should not happen");
}


// vim: set ts=8 sw=4 et :
