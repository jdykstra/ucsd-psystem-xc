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

#include <ucsdpsys_assemble/expression/6502/immediate.h>


expression_6502_immediate::~expression_6502_immediate()
{
}


expression_6502_immediate::expression_6502_immediate(
    const expression::pointer &a_operand
) :
    expression_6502(a_operand->get_location()),
    operand(a_operand)
{
}


expression_6502_immediate::pointer
expression_6502_immediate::create(const expression::pointer &a_operand)
{
    return pointer(new expression_6502_immediate(a_operand));
}


expression::pointer
expression_6502_immediate::optimize(void)
    const
{
    return create(operand->optimize());
}


rcstring
expression_6502_immediate::lisp_representation(void)
    const
{
    return ("(6502-immediate " + operand->lisp_representation() + ")");
}


expression::pointer
expression_6502_immediate::clone(const location &locn)
    const
{
    return create(operand->clone(locn));
}


// vim: set ts=8 sw=4 et :
