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

#include <lib/expression/list.h>

#include <ucsdpsys_compile/translator/compile.h>
#include <ucsdpsys_compile/expression/writeln/real.h>


expression_writeln_real::~expression_writeln_real()
{
}


expression_writeln_real::expression_writeln_real(
    const expression::pointer &a_value,
    const expression::pointer &a_width,
    const expression::pointer &a_precision,
    translator_compile *a_context
) :
    expression_writeln(a_value, a_width),
    context(a_context),
    precision(a_precision)
{
    //
    // We need to have segment 31 loaded, so we can call procedures in
    // that segment.
    //
    context->need_segment(31);
}


expression::pointer
expression_writeln_real::create(const expression::pointer &a_value,
    const expression::pointer &a_width, const expression::pointer &a_precision,
    translator_compile *a_context)
{
    return
        pointer
        (
            new expression_writeln_real
            (
                a_value,
                a_width,
                a_precision,
                a_context
            )
        );
}


expression::pointer
expression_writeln_real::function_call_expression_factory(
    const expression::pointer &file) const
{
    expression_list args;
    expression::pointer ep =
        context->name_expression(file->get_location(), "sys:fwritereal");
    args.push_back(ep);
    args.push_back(file);
    args.push_back(get_value());
    args.push_back(get_width());
    args.push_back(precision);
    return ep->function_call_expression_factory(args);
}


rcstring
expression_writeln_real::lisp_representation(void)
    const
{
    return
        (
            "(writeln_real "
        +
            get_value()->lisp_representation()
        +
            " "
        +
            get_width()->lisp_representation()
        +
            " "
        +
            precision->lisp_representation()
        +
            ")"
        );
}


expression::pointer
expression_writeln_real::optimize(void)
    const
{
    return
        create
        (
            get_value()->optimize(),
            get_width()->optimize(),
            precision->optimize(),
            context
        );
}


expression::pointer
expression_writeln_real::clone(const location &locn)
    const
{
    return
        create
        (
            get_value()->clone(locn),
            get_width()->clone(locn),
            precision->clone(locn),
            context
        );
}


// vim: set ts=8 sw=4 et :
