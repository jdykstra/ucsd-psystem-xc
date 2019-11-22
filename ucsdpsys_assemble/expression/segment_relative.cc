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

#include <ucsdpsys_assemble/expression/segment_relative.h>
#include <ucsdpsys_assemble/type/segment_relative.h>


expression_segment_relative::~expression_segment_relative()
{
}


expression_segment_relative::expression_segment_relative(
    const location &locn,
    long a_value
) :
    expression(locn),
    value(a_value)
{
}


expression_segment_relative::pointer
expression_segment_relative::create(const location &locn, long a_value)
{
    return pointer(new expression_segment_relative(locn, a_value));
}


type::pointer
expression_segment_relative::get_type(void)
    const
{
    return type_segment_relative::create();
}


void
expression_segment_relative::traversal(int)
    const
{
    assert(false);
}


void
expression_segment_relative::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(false);
}


int
expression_segment_relative::get_precedence(void)
    const
{
    return 0;
}


bool
expression_segment_relative::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_segment_relative::has_side_effect(void)
    const
{
    return side_effect_no;
}


long
expression_segment_relative::get_integer_value(void)
    const
{
    return value;
}


bool
expression_segment_relative::is_constant(void)
    const
{
    return true;
}


expression::pointer
expression_segment_relative::optimize(void)
    const
{
    return create(get_location(), value);
}


rcstring
expression_segment_relative::lisp_representation(void)
    const
{
    return rcstring::printf("(segment-relative %ld)", value);
}


expression::pointer
expression_segment_relative::clone(location const&) const
{
    return create(get_location(), value);
}


// vim: set ts=8 sw=4 et :
