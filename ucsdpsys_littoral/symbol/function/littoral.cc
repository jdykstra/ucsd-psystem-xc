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

#include <ucsdpsys_littoral/symbol/function/littoral.h>
#include <ucsdpsys_littoral/expression/function_call/littoral.h>
#include <ucsdpsys_littoral/expression/name/littoral.h>


symbol_function_littoral::~symbol_function_littoral()
{
}


symbol_function_littoral::symbol_function_littoral(
    const rcstring &a_name,
    const scope::pointer &a_locals,
    int a_lex_level,
    bool new_seg,
    translator_littoral *cntxt
) :
    symbol_function(a_name, a_locals, a_lex_level, new_seg),
    littoral(*cntxt)
{
}


symbol_function_littoral::symbol_function_littoral(
    const rcstring &a_name,
    const location &dec_loc,
    const scope::pointer &a_locals,
    int a_lex_level,
    bool new_seg,
    translator_littoral *cntxt
) :
    symbol_function(a_name, dec_loc, a_locals, a_lex_level, new_seg),
    littoral(*cntxt)
{
}


symbol_function_littoral::pointer
symbol_function_littoral::create(const rcstring &a_name,
    const scope::pointer &a_locals, int lxlvl, bool new_seg,
    translator_littoral *cntxt)
{
    return
        pointer
        (
            new symbol_function_littoral
            (
                a_name,
                a_locals,
                lxlvl,
                new_seg,
                cntxt
            )
        );
}


symbol_function_littoral::pointer
symbol_function_littoral::create(const rcstring &a_name,
    const location &dec_loc, const scope::pointer &a_locals, int lxlvl,
    bool seg_proc, translator_littoral *cntxt)
{
    return
        pointer
        (
            new symbol_function_littoral
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


symbol_function_littoral::pointer
symbol_function_littoral::create_builtin(const rcstring &a_name,
    translator_littoral *cntxt)
{
    return
        create(a_name, location::builtin(), scope::pointer(), -1, false, cntxt);
}


expression::pointer
symbol_function_littoral::function_call_expression_factory(
    const expression_list &rhs) const
{
    return expression_function_call_littoral::create(rhs, &littoral);
}


expression::pointer
symbol_function_littoral::name_expression_factory(const symbol::pointer &sp,
    const location &where, int)
{
    return expression_name_littoral::create(where, sp, &littoral);
}


// vim: set ts=8 sw=4 et :
