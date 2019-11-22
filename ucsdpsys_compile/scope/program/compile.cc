//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/symbol/expression/constant.h>
#include <lib/type/integer.h>

#include <ucsdpsys_compile/scope/program/compile.h>
#include <ucsdpsys_compile/symbol/functor/constructor.h>
#include <ucsdpsys_compile/symbol/functor/destructor.h>
#include <ucsdpsys_compile/symbol/variable/normal/compile.h>
#include <ucsdpsys_compile/symbol/variable/normal/global.h>
#include <ucsdpsys_compile/translator/compile.h>


scope_program_compile::~scope_program_compile()
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
}


scope_program_compile::scope_program_compile(
    const rcstring &a_name,
    translator_compile *a_code
) :
    scope_program(a_name),
    code(*a_code),
    proc_num(0)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    code.new_segment(get_name());
    proc_num = code.next_procedure_number();
    assert(proc_num == 1);
    if (proc_num < 0)
        pascal_grammar_error("too many procedures");
}


scope_program_compile::pointer
scope_program_compile::create(const rcstring &a_name,
    translator_compile *a_code)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    return pointer(new scope_program_compile(a_name, a_code));
}


void
scope_program_compile::pre_order_traversal(void)
    const
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);

    // From this point on, only code for this one procedure is being
    // injected into the segment's code space.  We have also passed the
    // point in the parse where any more variables can be declared.
    code.procedure_parameter_size(get_param_size_in_bytes());
    code.procedure_variable_size(get_data_size_in_bytes());
    code.procedure_enter_ic();

    // Load intrinsics
    for (int sn = 63; sn >= 0; --sn)
    {
        if (code.segment_needed(sn))
        {
            code.emit_byte(SLDC_0 + sn);
            code.emit_byte(CSP);
            code.emit_byte(CSP_LOADSEGMENT);

            if (sn != 31 && sn != 30)
            {
                // call the unit initializer
                code.emit_byte(CXP);
                code.emit_byte(sn);
                code.emit_byte(1);
            }
        }
    }

    //
    // Generate code for constructors, if necessary.
    //
    symbol_functor_constructor init(code, *this);
    symbol_table_walk(init);
}


void
scope_program_compile::post_order_traversal(void)
    const
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    location loc = pascal_lex_location();
    code.procedure_exit_ic();

    //
    // Generate code for destructors, if necessary.
    //
    symbol_functor_destructor fini(loc, code);
    symbol_table_walk(fini);

    // Unload intrinsics
    for (int sn = 0; sn < 64; ++sn)
    {
        if (code.segment_needed(sn))
        {
            code.breakpoint(loc);
            code.emit_byte(SLDC_0 + sn);
            code.emit_byte(CSP);
            code.emit_byte(CSP_UNLOADSEGMENT);
        }
    }

    //
    // Emit return opcode(s)
    //
    code.breakpoint(loc);
    if (code.is_a_sysprog())
        code.emit_byte(XIT);
    else
    {
        unsigned words_of_return_value = 0;
        code.emit_byte(RBP);
        code.emit_byte(words_of_return_value);
    }
    code.emit_word_alignment(NOP);

    //
    // Insert details into procedure dictionary.
    //
    code.procedure_end(proc_num, get_sp()->get_lex_level());

    //
    // Insert details into segment dictionary.
    //
    code.segment_end();
}


void
scope_program_compile::constant_declaration_notify(const variable_name &,
    const expression::pointer &)
{
    // Do nothing.
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
}


void
scope_program_compile::type_declaration_notify(const variable_name &,
    const type::pointer &)
{
    // Do nothing.
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
}


void
scope_program_compile::variable_declaration_notify(const symbol_variable_p &)
{
    // Do nothing.
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
}


void
scope_program_compile::notify_function_parameters(const name_type_list &)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    assert(!"no function parameters for programs");
}


symbol_variable::pointer
scope_program_compile::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    if (lex_level == 0)
    {
        // You could be forgiven for thinking that this is always the
        // case, and it is: for user programs.
        return
            symbol_variable_normal_global::create
            (
                vname,
                vtype,
                lex_level,
                symbol_variable::kind_variable,
                &code
            );
    }
    else
    {
        // This case is for (*$U-*) system programs.
        assert(lex_level == -1);
        return
            symbol_variable_normal_compile::create
            (
                vname,
                vtype,
                lex_level,
                symbol_variable::kind_variable,
                &code
            );
    }
}


symbol_variable::pointer
scope_program_compile::parameter_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    if (lex_level == 0)
    {
        // You could be forgiven for thinking that this is always the
        // case, and it is, for user programs.
        return
            symbol_variable_normal_global::create
            (
                vname,
                vtype,
                lex_level,
                symbol_variable::kind_parameter,
                &code
            );
    }
    else
    {
        // This case is for (*$U-*) system programs.
        return
            symbol_variable_normal_compile::create
            (
                vname,
                vtype,
                lex_level,
                symbol_variable::kind_parameter,
                &code
            );
    }
}


symbol::pointer
scope_program_compile::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    return symbol_expression_constant::create(vname, lex_level, value);
}


// vim: set ts=8 sw=4 et :
