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

#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/translator.h>
#include <lib/type/boolean.h>


expression::pointer
translator::logical_or_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    expression::pointer e1 = call_noparam_func_if_necessary(lhs);
    if (e1->is_error())
        return e1;
    expression::pointer e2 = call_noparam_func_if_necessary(rhs);
    if (e2->is_error())
        return e2;

    return op_or.dispatch(e1, e2);
}


// vim: set ts=8 sw=4 et :
