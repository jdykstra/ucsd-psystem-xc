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

#include <lib/statement/case.h>


statement_case::~statement_case()
{
}


statement_case::statement_case(
    const expression::pointer &a_control,
    const case_list &a_cases,
    const statement::pointer &a_otherwise
) :
    control(a_control),
    cases(a_cases),
    otherwise(a_otherwise)
{
}


bool
statement_case::execution_flows_out(void)
    const
{
    if (!otherwise)
        return true;
    return cases.execution_flows_out();
}


// vim: set ts=8 sw=4 et :
