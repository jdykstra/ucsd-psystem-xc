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

#include <ucsdpsys_pretty/expression/function_call/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_function_call_pretty::~expression_function_call_pretty()
{
}


expression_function_call_pretty::expression_function_call_pretty(
    const expression_function_call_pretty &arg
) :
    expression_function_call(arg),
    pretty(arg.pretty)
{
}


expression_function_call_pretty::expression_function_call_pretty(
    const expression_list &a_args,
    translator_pretty *cntxt
) :
    expression_function_call(a_args),
    pretty(*cntxt)
{
}


expression_function_call_pretty::pointer
expression_function_call_pretty::create(const expression_list &a_args,
    translator_pretty *cntxt)
{
    return pointer(new expression_function_call_pretty(a_args, cntxt));
}


void
expression_function_call_pretty::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        pretty.print_token("(");
}


void
expression_function_call_pretty::comma_order_traversal(int, int argnum)
    const
{
    if (argnum == 0)
        pretty.print_token("(");
    else
    {
        pretty.print_token(",");
        pretty.print_space();
    }
}


void
expression_function_call_pretty::post_order_traversal(int pprec, int argnum)
    const
{
    if (argnum >= 1)
        pretty.print_token(")");
    if (need_parens(pprec))
        pretty.print_token(")");
}


expression::pointer
expression_function_call_pretty::optimize(void)
    const
{
    return pointer(new expression_function_call_pretty(*this));
}


expression::pointer
expression_function_call_pretty::clone(const location &locn)
    const
{
    expression_list args;
    for (size_t j = 0; j < get_child_count(); ++j)
        args.push_back(get_child(j)->clone(locn));
    return create(args, &pretty);
}


// vim: set ts=8 sw=4 et :
