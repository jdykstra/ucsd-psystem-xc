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

#include <lib/ac/assert.h>

#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pcode.h>
#include <lib/type.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/symbol/function/user_defined.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_user_defined::~symbol_function_user_defined()
{
}


symbol_function_user_defined::symbol_function_user_defined(
    const rcstring &a_name,
    const scope::pointer &scp,
    int a_lex_level,
    bool seg_proc,
    int seg_num,
    int proc_num,
    translator_compile *cntxt
) :
    symbol_function(a_name, scp, a_lex_level, seg_proc),
    code(*cntxt),
    segment_number(seg_num),
    procedure_number(proc_num)
{
    assert(!seg_proc || proc_num == 1);
    // It is tempting to say
    //
    //     assert(scp);
    //
    // but this happens when the segment 0 procedures are being declared
    // as builtins.
    //
}


symbol_function_user_defined::symbol_function_user_defined(
    const rcstring &a_name,
    const location &dec_loc,
    const scope::pointer &scp,
    int a_lex_level,
    bool seg_proc,
    int seg_num,
    int proc_num,
    translator_compile *cntxt
) :
    symbol_function(a_name, dec_loc, scp, a_lex_level, seg_proc),
    code(*cntxt),
    segment_number(seg_num),
    procedure_number(proc_num)
{
    assert(!seg_proc || proc_num == 1);
    // It is tempting to say
    //
    //     assert(scp);
    //
    // but this happens when the segment 0 procedures are being declared
    // as builtins.
    //
}


symbol_function_user_defined::pointer
symbol_function_user_defined::create(const rcstring &a_name,
    const scope::pointer &scp, int a_lex_level, bool seg_proc,
    int seg_num, int proc_num, translator_compile *cntxt)
{
    return
        pointer
        (
            new symbol_function_user_defined
            (
                a_name,
                scp,
                a_lex_level,
                seg_proc,
                seg_num,
                proc_num,
                cntxt
            )
        );
}


symbol_function_user_defined::pointer
symbol_function_user_defined::create(const rcstring &a_name,
    const location &dec_loc, const scope::pointer &scp, int a_lex_level,
    bool seg_proc, int seg_num, int proc_num, translator_compile *cntxt)
{
    return
        pointer
        (
            new symbol_function_user_defined
            (
                a_name,
                dec_loc,
                scp,
                a_lex_level,
                seg_proc,
                seg_num,
                proc_num,
                cntxt
            )
        );
}


expression::pointer
symbol_function_user_defined::function_call_expression_factory(
    const expression_list &rhs) const
{
    //
    // Checking for number and types of parameters.
    // (this does automatic promotions as well)
    //
    expression_list params = check_parameters(rhs, code);
    if (params.empty())
        return expression_error::create(rhs.get_location());
    assert(!params.get_location().empty());

    //
    // Now build the expression node.
    //
    return expression_function_call_user_defined::create(this, params, &code);
}


unsigned
symbol_function_user_defined::get_segment_number()
    const
{
    return segment_number;
}


void
symbol_function_user_defined::notify_forward_suspend(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    // if (procedure_number == 1)
    if (is_segment_procedure())
    {
        assert(procedure_number == 1);
        // We are looking at a SEGMENT procedure FORWARD in this case.
        segbuilder = code.segment_procedure_forward_suspend();
    }
}


void
symbol_function_user_defined::notify_forward_resume(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (segbuilder)
    {
        code.segment_procedure_forward_resume(segbuilder);
        segbuilder.reset();
    }
}


unsigned
symbol_function_user_defined::get_procedure_number(void)
    const
{
    return procedure_number;
}


expression::pointer
symbol_function_user_defined::name_expression_factory(const symbol::pointer &sp,
    const location &ref_locn, int)
{
    assert(this == sp.get());
    //
    // Functions are tricky.  They could be a return variable
    // assignment, or they could be an imminent function call, or it
    // could be just wrong.
    //
    // Note: Pascal also permits passing function pointers into
    // functions, but UCSD Pascal does not.
    //
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sp);
    assert(sfp);
    return expression_function::create(ref_locn, sfp, &code);
}


// vim: set ts=8 sw=4 et :
