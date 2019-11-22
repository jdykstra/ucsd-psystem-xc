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

#include <ucsdpsys_littoral/expression/function_call/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_function_call_littoral::~expression_function_call_littoral()
{
}


expression_function_call_littoral::expression_function_call_littoral(
    const expression_function_call_littoral &arg
) :
    expression_function_call(arg),
    littoral(arg.littoral)
{
}


expression_function_call_littoral::expression_function_call_littoral(
    const expression_list &a_args,
    translator_littoral *cntxt
) :
    expression_function_call(a_args),
    littoral(*cntxt)
{
}


expression_function_call_littoral::pointer
expression_function_call_littoral::create(const expression_list &a_args,
    translator_littoral *cntxt)
{
    return pointer(new expression_function_call_littoral(a_args, cntxt));
}


void
expression_function_call_littoral::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        littoral.print_token("(");
}


void
expression_function_call_littoral::comma_order_traversal(int, int argnum)
    const
{
    if (argnum == 0)
        littoral.print_token("(");
    else
    {
        littoral.print_token(",");
        littoral.print_space();
    }
}


void
expression_function_call_littoral::post_order_traversal(int pprec, int argnum)
    const
{
    if (argnum == 0)
        littoral.print_token("(");
    littoral.print_token(")");
    if (need_parens(pprec))
        littoral.print_token(")");
}


expression::pointer
expression_function_call_littoral::optimize(void)
    const
{
    return pointer(new expression_function_call_littoral(*this));
}


expression::pointer
expression_function_call_littoral::clone(const location &locn)
    const
{
    expression_list args;
    for (size_t j = 0; j < get_child_count(); ++j)
        args.push_back(get_child(j)->clone(locn));
    return create(args, &littoral);
}


bool
expression_function_call_littoral::is_constant(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
