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

#include <ucsdpsys_pretty/expression/lint2lint/pretty.h>


expression_lint2lint_pretty::~expression_lint2lint_pretty()
{
}


expression_lint2lint_pretty::expression_lint2lint_pretty(
    const type::pointer &a_to,
    const expression::pointer &a_from,
    translator_pretty *a_pretty
) :
    expression_lint2lint(a_to, a_from),
    pretty(*a_pretty)
{
}


expression_lint2lint_pretty::pointer
expression_lint2lint_pretty::create(const type::pointer &a_to,
    const expression::pointer &a_from, translator_pretty *a_pretty)
{
    return pointer(new expression_lint2lint_pretty(a_to, a_from, a_pretty));
}


void
expression_lint2lint_pretty::traversal(int pprec)
    const
{
    get_from()->traversal(pprec);
}


expression::pointer
expression_lint2lint_pretty::optimize(void)
    const
{
    return create(get_type(), get_from(), &pretty);
}


expression::pointer
expression_lint2lint_pretty::clone(const location &locn)
    const
{
    return create(get_type(), get_from()->clone(locn), &pretty);
}


// vim: set ts=8 sw=4 et :
