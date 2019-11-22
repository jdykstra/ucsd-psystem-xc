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

#include <lib/pcode.h>
#include <lib/expression/list.h>
#include <lib/type/string.h>

#include <ucsdpsys_assemble/expression/range_check.h>
#include <ucsdpsys_assemble/opcode/pcode/lsa.h>


opcode_pcode_lsa::~opcode_pcode_lsa()
{
}


opcode_pcode_lsa::opcode_pcode_lsa(assembler *a_context) :
    opcode_pcode(a_context)
{
}


opcode_pcode_lsa::pointer
opcode_pcode_lsa::create(assembler *a_context)
{
    return pointer(new opcode_pcode_lsa(a_context));
}


void
opcode_pcode_lsa::interpret(const expression_list &args)
{
    if (args.empty())
    {
        check_argument_count(args, 1);
        return;
    }
    if (!check_argument_const_int(1, args[0], 0, 255))
        return;
    unsigned nbytes = args[0]->optimize()->get_integer_value();
    if (args.size() == 2)
    {
        expression::pointer ep = args[1];
        if (ep->is_constant() && type_string::is_a(ep))
        {
            rcstring value = ep->optimize()->get_string_value();
            if (value.size() == nbytes)
            {
                emit_byte(LSA);
                emit_byte(nbytes);
                for (size_t j = 0; j < value.size(); ++j)
                {
                    emit_byte(value[j]);
                }
                return;
            }
        }
    }
    if (!check_argument_count(args, 1 + nbytes))
        return;

    emit_byte(LSA);
    emit_byte(nbytes);
    for (size_t j = 0; j < nbytes; ++j)
    {
        expression::pointer ep = args[j + 1];
        emit_byte(expression_range_check::create(j + 2, ep, 0, 255));
    }
}


rcstring
opcode_pcode_lsa::get_name(void)
    const
{
    return "lsa";
}


// vim: set ts=8 sw=4 et :
