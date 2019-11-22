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

#include <lib/type/real.h>

#include <ucsdpsys_compile/expression/addition/integer.h>
#include <ucsdpsys_compile/expression/addition/real.h>
#include <ucsdpsys_compile/expression/assignment/compile.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/expression/subtraction/integer.h>
#include <ucsdpsys_compile/expression/subtraction/real.h>
#include <ucsdpsys_compile/label/compile.h>
#include <ucsdpsys_compile/statement/for/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


statement_for_compile::~statement_for_compile()
{
}


statement_for_compile::statement_for_compile(
    const expression::pointer &a_var,
    const expression::pointer &a_limit1,
    bool a_descending,
    const expression::pointer &a_limit2,
    const statement::pointer &a_body,
    translator_compile *a_code
) :
    statement_for(a_var, a_limit1, a_descending, a_limit2, a_body),
    code(*a_code)
{
}


statement_for_compile::pointer
statement_for_compile::create(const expression::pointer &a_var,
    const expression::pointer &a_limit1, bool a_descending,
    const expression::pointer &a_limit2, const statement::pointer &a_body,
    translator_compile *a_code)
{
    return
        pointer
        (
            new statement_for_compile
            (
                a_var,
                a_limit1,
                a_descending,
                a_limit2,
                a_body,
                a_code
            )
        );
}


void
statement_for_compile::traversal(void)
    const
{
    //
    // the initial assignment
    //
    {
        expression::pointer ep  =
            code.assignment_expression_factory(get_var(), get_limit1());
        ep = ep->optimize();
        ep->traversal(0);
    }

    label::pointer the_top = label_compile::create(&code);
    the_top->define_here();
    label::pointer the_body = label_compile::create(&code);
    label::pointer the_end = label_compile::create(&code);

    //
    // The comparison
    //
    {
        expression::pointer ep =
            (
                is_descending()
            ?
                code.ge_expression(get_var(), get_limit2())
            :
                code.le_expression(get_var(), get_limit2())
            );
        ep = ep->optimize();
        ep->logical_traversal(0, the_body, the_end, true);
    }

    //
    // The loop body
    //
    the_body->define_here();
    get_body()->traversal();

    //
    // advance the loop control variable
    //
    {
        // We go direct, rather than using the translator factory
        // methods, because the translator factories check too much.
        if (type_real::is_a(get_var()))
        {
            expression::pointer one =
                expression_real_compile::create
                (
                    get_var()->get_location(),
                    1.0,
                    &code
                );
            if (is_descending())
            {
                expression::pointer rhs =
                    expression_subtraction_real::create(get_var(), one, &code);
                expression::pointer ep =
                    get_var()->assignment_expression_factory(rhs);
                ep = ep->optimize();
                ep->traversal(0);
            }
            else
            {
                expression::pointer rhs =
                    expression_addition_real::create(get_var(), one, &code);
                expression::pointer ep =
                    get_var()->assignment_expression_factory(rhs);
                ep = ep->optimize();
                ep->traversal(0);
            }
        }
        else
        {
            expression::pointer one =
                expression_integer_compile::create
                (
                    get_var()->get_location(),
                    1,
                    &code
                );
            if (is_descending())
            {
                expression::pointer rhs =
                    expression_subtraction_integer::create
                    (
                        get_var(),
                        one,
                        &code
                    );
                expression::pointer ep =
                    get_var()->assignment_expression_factory(rhs);
                ep = ep->optimize();
                ep->traversal(0);
            }
            else
            {
                expression::pointer rhs =
                    expression_addition_integer::create(get_var(), one, &code);
                expression::pointer ep =
                    get_var()->assignment_expression_factory(rhs);
                ep = ep->optimize();
                ep->traversal(0);
            }
        }
    }

    the_top->goto_from_here();
    the_end->define_here();
}


// vim: set ts=8 sw=4 et :
