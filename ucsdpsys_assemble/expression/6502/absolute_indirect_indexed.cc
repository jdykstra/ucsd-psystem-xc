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

#include <ucsdpsys_assemble/expression/6502/absolute_indirect_indexed.h>


expression_6502_absolute_indirect_indexed::
    ~expression_6502_absolute_indirect_indexed()
{
}


expression_6502_absolute_indirect_indexed::
expression_6502_absolute_indirect_indexed(
    const expression::pointer &a_address,
    const expression::pointer &a_special
) :
    expression_6502(a_address->get_location()),
    address(a_address),
    special(a_special)
{
}


expression_6502_absolute_indirect_indexed::pointer
expression_6502_absolute_indirect_indexed::create(
    const expression::pointer &a_address, const expression::pointer &a_special)
{
    return
        pointer
        (
            new expression_6502_absolute_indirect_indexed(a_address, a_special)
        );
}


expression::pointer
expression_6502_absolute_indirect_indexed::optimize(void)
    const
{
    return create(get_address()->optimize(), special->optimize());
}


rcstring
expression_6502_absolute_indirect_indexed::lisp_representation(void)
    const
{
    return
        (
            "(6502-absolute-indirect-indexed "
        +
            address->lisp_representation()
        +
            " "
        +
            special->lisp_representation()
        +
            ")"
        );
}


expression::pointer
expression_6502_absolute_indirect_indexed::clone(const location &locn)
    const
{
    return create(get_address()->clone(locn), special->clone(locn));
}


// vim: set ts=8 sw=4 et :
