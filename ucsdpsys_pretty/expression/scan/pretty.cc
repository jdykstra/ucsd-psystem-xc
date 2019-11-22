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

#include <ucsdpsys_pretty/expression/scan/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_scan_pretty::~expression_scan_pretty()
{
}


expression_scan_pretty::expression_scan_pretty(
    const expression::pointer &a_maxdisp,
    int a_mask,
    const expression::pointer &a_value,
    const expression::pointer &a_start,
    translator_pretty *a_pretty
) :
    expression_scan(a_maxdisp, a_mask, a_value, a_start),
    pretty(*a_pretty)
{
}


expression_scan_pretty::pointer
expression_scan_pretty::create(const expression::pointer &a_maxdisp,
    int a_mask, const expression::pointer &a_value,
    const expression::pointer &a_start, translator_pretty *a_pretty)
{
    return
        pointer
        (
            new expression_scan_pretty
            (
                a_maxdisp,
                a_mask,
                a_value,
                a_start,
                a_pretty
            )
        );
}


void
expression_scan_pretty::traversal(int)
    const
{
    pretty.print_token("scan");
    pretty.print_token("(");
    get_maxdisp()->traversal(0);
    pretty.print_token(",");
    pretty.print_space();
    pretty.print_token(get_mask() ? "<>" : "=");
    pretty.print_space();
    get_value()->traversal(get_precedence());
    pretty.print_token(",");
    pretty.print_space();
    get_start()->traversal(get_precedence());
    pretty.print_token(")");
}


expression::pointer
expression_scan_pretty::optimize(void)
    const
{
    return create(get_maxdisp(), get_mask(), get_value(), get_start(), &pretty);
}


expression::pointer
expression_scan_pretty::clone(const location &locn)
    const
{
    return
        create
        (
            get_maxdisp()->clone(locn),
            get_mask(),
            get_value()->clone(locn),
            get_start()->clone(locn),
            &pretty
        );
}


// vim: set ts=8 sw=4 et :
