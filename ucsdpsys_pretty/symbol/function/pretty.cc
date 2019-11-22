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

#include <ucsdpsys_pretty/symbol/function/pretty.h>
#include <ucsdpsys_pretty/expression/function_call/pretty.h>
#include <ucsdpsys_pretty/expression/name/pretty.h>


symbol_function_pretty::~symbol_function_pretty()
{
}


symbol_function_pretty::symbol_function_pretty(
    const rcstring &a_name,
    const scope::pointer &a_locals,
    int a_lex_level,
    bool new_seg,
    translator_pretty *cntxt
) :
    symbol_function(a_name, a_locals, a_lex_level, new_seg),
    pretty(*cntxt)
{
}


symbol_function_pretty::symbol_function_pretty(
    const rcstring &a_name,
    const location &dec_loc,
    const scope::pointer &a_locals,
    int a_lex_level,
    bool new_seg,
    translator_pretty *cntxt
) :
    symbol_function(a_name, dec_loc, a_locals, a_lex_level, new_seg),
    pretty(*cntxt)
{
}


symbol_function_pretty::pointer
symbol_function_pretty::create(const rcstring &a_name,
    const scope::pointer &a_locals, int lxlvl, bool new_seg,
    translator_pretty *cntxt)
{
    return
        pointer
        (
            new symbol_function_pretty(a_name, a_locals, lxlvl, new_seg, cntxt)
        );
}


symbol_function_pretty::pointer
symbol_function_pretty::create(const rcstring &a_name, const location &dec_loc,
    const scope::pointer &a_locals, int lxlvl, bool seg_proc,
    translator_pretty *cntxt)
{
    return
        pointer
        (
            new symbol_function_pretty
            (
                a_name,
                dec_loc,
                a_locals,
                lxlvl,
                seg_proc,
                cntxt
            )
        );
}


symbol_function_pretty::pointer
symbol_function_pretty::create_builtin(const rcstring &a_name,
    translator_pretty *cntxt)
{
    return
        create(a_name, location::builtin(), scope::pointer(), -1, false, cntxt);
}


expression::pointer
symbol_function_pretty::function_call_expression_factory(
    const expression_list &rhs) const
{
    return expression_function_call_pretty::create(rhs, &pretty);
}


expression::pointer
symbol_function_pretty::name_expression_factory(const symbol::pointer &sp,
    const location &where, int)
{
    assert(this == sp.get());
    return expression_name_pretty::create(where, sp, &pretty);
}


// vim: set ts=8 sw=4 et :
