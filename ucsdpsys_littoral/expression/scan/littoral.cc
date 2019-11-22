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

#include <ucsdpsys_littoral/expression/scan/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_scan_littoral::~expression_scan_littoral()
{
}


expression_scan_littoral::expression_scan_littoral(
    const expression::pointer &a_maxdisp,
    int a_mask,
    const expression::pointer &a_value,
    const expression::pointer &a_start,
    translator_littoral *a_littoral
) :
    expression_scan(a_maxdisp, a_mask, a_value, a_start),
    littoral(*a_littoral)
{
}


expression_scan_littoral::pointer
expression_scan_littoral::create(const expression::pointer &a_maxdisp,
    int a_mask, const expression::pointer &a_value,
    const expression::pointer &a_start, translator_littoral *a_littoral)
{
    return
        pointer
        (
            new expression_scan_littoral
            (
                a_maxdisp,
                a_mask,
                a_value,
                a_start,
                a_littoral
            )
        );
}


void
expression_scan_littoral::traversal(int)
    const
{
    littoral.print_token("scan");
    littoral.print_token("(");
    get_maxdisp()->traversal(0);
    littoral.print_token(",");
    littoral.print_space();
    littoral.print_token(get_mask() ? "<>" : "=");
    littoral.print_space();
    get_value()->traversal(get_precedence());
    littoral.print_token(",");
    littoral.print_space();
    get_start()->traversal(get_precedence());
    littoral.print_token(")");
}


expression::pointer
expression_scan_littoral::optimize(void)
    const
{
    return
        create(get_maxdisp(), get_mask(), get_value(), get_start(), &littoral);
}


expression::pointer
expression_scan_littoral::clone(const location &locn)
    const
{
    return
        create
        (
            get_maxdisp()->clone(locn),
            get_mask(),
            get_value()->clone(locn),
            get_start()->clone(locn),
            &littoral
        );
}


// vim: set ts=8 sw=4 et :
