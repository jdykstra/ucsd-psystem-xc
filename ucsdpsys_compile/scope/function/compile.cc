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
#include <lib/name_type/list.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/symbol/expression/constant.h>
#include <lib/type/file.h>
#include <lib/type/integer.h>
#include <lib/type/reference.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/scope/function/compile.h>
#include <ucsdpsys_compile/symbol/functor/constructor.h>
#include <ucsdpsys_compile/symbol/functor/destructor.h>
#include <ucsdpsys_compile/symbol/variable/normal/compile.h>
#include <ucsdpsys_compile/symbol/variable/normal/global.h>
#include <ucsdpsys_compile/translator/compile.h>


scope_function_compile::~scope_function_compile()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


scope_function_compile::scope_function_compile(
    const rcstring &a_name,
    bool afunc,
    bool new_seg,
    translator_compile *a_code
) :
    scope_function(a_name),
    code(*a_code),
    is_a_function(afunc),
    is_a_segment_procedure(new_seg),
    seg_num(0),
    proc_num(0)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    // FIXME: what happens when too many segments?
    if (is_a_segment_procedure)
        code.new_segment(get_name());
    seg_num = code.get_segment_number();
    proc_num = code.next_procedure_number();
    if (proc_num < 0)
        pascal_grammar_error("too many procedures");
    assert(is_a_segment_procedure ? proc_num == 1 : proc_num >= 2);
    if (is_a_function)
    {
        //
        // Reserve 4 bytes of parameters for the return value.  (Our
        // callers will push two zero words onto the stack.)  This
        // is because we have yet to parse the return type, so we
        // don't know how big it really will be.  The worst case is to
        // return a real of 4 bytes, all other types (integer, pointer,
        // boolean) are 2 bytes.
        //
        // If we got smarter about this (we already slurp all the
        // parameters, why not the return type too) we could produce
        // programs that will use less stack space, will use less code
        // space, and will run faster.
        //
        // To do this, the system has to have been compiled with the
        // same compiler, otherwise there will be a mismatch when user
        // programs call segment 0 functions.  Also, assembler functions
        // will have to allocate space differently, too.  So this is
        // probably not practical, from a portability point of view.
        //
        reserve_param_space_in_bytes(4);
    }
    DEBUG(1, "}");
}


scope_function_compile::pointer
scope_function_compile::create(const rcstring &a_name, bool afunc, bool new_seg,
    translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new scope_function_compile(a_name, afunc, new_seg, a_code));
}


void
scope_function_compile::notify_function_parameters(const name_type_list &ntl)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    // Allocate the local space for the call-by-value string parameters
    // to be copied (via their reference) at the start of the function.
    for (size_t j = 0; j < ntl.size(); ++j)
    {
        DEBUG(2, "j = %d/%d", (int)j, (int)ntl.size());
        const name_type &nt = ntl[j];
        if (type_file::is_a(nt.get_type()))
        {
            pascal_lex_error
            (
                nt.get_name(0).get_location(),
                "file parameters must be passed by reference"
            );
        }
        for (size_t k = 0; k < nt.size(); ++k)
        {
            DEBUG(2, "k = %d/%d", (int)k, (int)nt.size());
            const variable_name &vn = nt.get_name(k);
            symbol::pointer sp2 = lookup(vn.get_name());
            assert(sp2);
            if (sp2 && sp2->get_type()->is_an_implicit_reference())
            {
                variable_name vn3("copy:" + vn.get_name(), vn.get_location());
                type::pointer tp3 = sp2->get_type()->get_subtype();
                symbol::pointer sp3 =
                    variable_factory(vn3, tp3, code.get_lex_level());

                // define it in the current scope.
                define(sp3, false);
            }
        }
    }
    DEBUG(1, "}");
}


symbol::pointer
scope_function_compile::lookup(const rcstring &supplicant)
    const
{
    symbol::pointer sym_p = scope_function::lookup(supplicant);
    if (sym_p && sym_p->get_type()->is_an_implicit_reference())
    {
        symbol::pointer sym2_p = scope_function::lookup("copy:" + supplicant);
        if (sym2_p)
            return sym2_p;
    }
    return sym_p;
}


void
scope_function_compile::pre_order_traversal()
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    // From this point on, only code for this one procedure is being
    // injected into the segment's code space.  We have also passed the
    // point in the parser where any more variables can be declared.
    code.procedure_parameter_size(get_param_size_in_bytes());
    code.procedure_variable_size(get_data_size_in_bytes());
    code.procedure_enter_ic();

    //
    // Generate code for constructors, if necessary.
    //
    symbol_functor_constructor init(code, *this);
    symbol_table_walk(init);
    DEBUG(1, "}");
}


void
scope_function_compile::post_order_traversal()
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);

    //
    // The exit point is *before* the destructor code,
    // so that files are closed when they go out of scope.
    //
    code.procedure_exit_ic();

    //
    // Generate code for destructors, if necessary.
    //
    location loc = pascal_lex_location();
    symbol_functor_destructor fini(loc, code);
    symbol_table_walk(fini);

    //
    // generate return opcode(s)
    //
    // This is symmetric with
    // ucsdpsys_compile/expression/function_call/user_defined.cc
    // expression_function_call_user_defined::post_order_traversal
    //
    code.breakpoint(loc);
    unsigned words_of_return_value =
        get_sp()->get_type()->get_subtype()->get_size_in_words();
    code.emit_byte(get_sp()->get_lex_level() <= 0 ? RBP : RNP);
    code.emit_byte(words_of_return_value);
    code.emit_word_alignment(NOP);

    //
    // insert details into procedure dictionary
    //
    code.procedure_end(proc_num, get_sp()->get_lex_level());

    //
    // insert details into segment dictionary
    //
    if (is_a_segment_procedure)
        code.segment_end();
    DEBUG(1, "}");
}


void
scope_function_compile::constant_declaration_notify(const variable_name &,
    const expression::pointer &)
{
    // Do nothing.
}


void
scope_function_compile::type_declaration_notify(const variable_name &,
    const type::pointer &)
{
    // Do nothing.
}


void
scope_function_compile::variable_declaration_notify(const symbol_variable_p &)
{
    // Do nothing.
    //
    // This would be where we dealt with debugger symbols, but since
    // there is neither a debugger nor a debugger symbol table, take the
    // afternoon off.
}


symbol_variable::pointer
scope_function_compile::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    if (lex_level == 0)
    {
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
scope_function_compile::parameter_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    if (lex_level == 0)
    {
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
scope_function_compile::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    return symbol_expression_constant::create(vname, lex_level, value);
}


// vim: set ts=8 sw=4 et :
