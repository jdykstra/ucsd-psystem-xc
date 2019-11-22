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

#include <ucsdpsys_compile/expression/logical_not/compile.h>
#include <ucsdpsys_compile/label/compile.h>
#include <ucsdpsys_compile/statement/goto/compile.h>
#include <ucsdpsys_compile/statement/if/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


statement_if_compile::~statement_if_compile()
{
}


statement_if_compile::statement_if_compile(
    const expression::pointer &a1,
    const statement::pointer &a2,
    const statement::pointer &a3,
    translator_compile *a4
) :
    statement_if(a1, a2, a3, a4),
    code(*a4)
{
}


statement_if_compile::pointer
statement_if_compile::create(const expression::pointer &a_cond,
    const statement::pointer &a_then, const statement::pointer &a_else,
    translator_compile *a_code)
{
    if (a_then->empty() && a_else)
    {
        expression::pointer c =
            expression_logical_not_compile::create(a_cond, a_code);
        statement::pointer nada;
        return pointer(new statement_if_compile(c, a_else, nada, a_code));
    }
    return pointer(new statement_if_compile(a_cond, a_then, a_else, a_code));
}


static label::pointer
get_goto_label(const statement::pointer &sp)
{
    statement_goto::pointer sgp =
        boost::dynamic_pointer_cast<statement_goto>(sp);
    if (!sgp)
        return label::pointer();
    return sgp->get_label();
}


void
statement_if_compile::traversal()
    const
{
    //
    // In the UCSD system sources, and in the UCSD compiler, these
    // pattern occur often enough to be worth while optimizing.
    // I'm sure Mr Wirth would be unimpressed.
    //
    label::pointer then_goto = get_goto_label(get_then_clause());
    if (then_goto)
    {
        label::pointer else_goto = get_goto_label(get_else_clause());
        if (else_goto)
        {
            // IF condition
            // THEN GOTO nnn
            // ELSE GOTO mmm
            get_condition()->logical_traversal(0, then_goto, else_goto, true);
            then_goto->goto_from_here();
        }
        else
        {
            // IF condition         IF condition
            // THEN GOTO nnn        THEN GOTO nnn
            // ELSE else_clause
            label::pointer false_bran = label_compile::create(&code);
            get_condition()->logical_traversal(0, then_goto, false_bran, false);
            false_bran->define_here();
            if (get_else_clause())
                get_else_clause()->traversal();
        }
    }
    else
    {
        label::pointer else_goto = get_goto_label(get_else_clause());
        if (else_goto)
        {
            // IF condition
            // THEN then_clause
            // ELSE GOTO mmm
            label::pointer true_branch = label_compile::create(&code);
            get_condition()->logical_traversal(0, true_branch, else_goto, true);
            true_branch->define_here();
            get_then_clause()->traversal();
        }
        else
        {
            // IF condition         IF condition
            // THEN then_clause     THEN then_clause
            // ELSE else_clause
            statement_if::traversal();
        }
    }
}


// vim: set ts=8 sw=4 et :
