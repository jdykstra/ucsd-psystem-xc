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
#include <lib/pcode.h>
#include <lib/type/nothing.h>
#include <lib/type/function.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/symbol/function/user_defined.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_user_defined::~expression_function_call_user_defined()
{
}


expression_function_call_user_defined::expression_function_call_user_defined(
    const symbol_function_user_defined *a_sp,
    const expression_list &a_rhs,
    translator_compile *cntxt
) :
    expression_function_call(a_rhs.get_location(), a_rhs.tail()),
    code(*cntxt),
    sp(a_sp)
{
    assert(!get_location().empty());
}


expression_function_call_user_defined::pointer
expression_function_call_user_defined::create(
    const symbol_function_user_defined *a_sp, const expression_list &a_rhs,
    translator_compile *cntxt)
{
    return
        pointer(new expression_function_call_user_defined(a_sp, a_rhs, cntxt));
}


expression_function_call_user_defined::pointer
expression_function_call_user_defined::create(const expression_list &a_rhs,
    translator_compile *cntxt)
{
    //
    // Some voodoo to extract a suitable 'sp' for the user defined
    // function call.
    //
    assert(a_rhs.size() >= 1);
    expression_function::pointer efp =
        boost::dynamic_pointer_cast<expression_function>(a_rhs[0]);
    assert(efp);
    symbol_function::pointer sfp = efp->get_sfp();
    symbol_function_user_defined::pointer sfudp =
        boost::dynamic_pointer_cast<symbol_function_user_defined>(sfp);
    assert(sfudp);

    //
    // Now call the regular create class method.
    //
    return create(sfudp.get(), a_rhs, cntxt);
}


void
expression_function_call_user_defined::post_order_traversal(int, int)
    const
{
    assert(!get_location().empty());
    code.breakpoint(get_location());
    assert(type_function::is_a(sp->get_type()));
    if (!type_nothing::is_a(sp->get_type()->get_subtype()))
    {
        //
        // Make room on the stack for the return value.
        // FIXME: this could be better, see rant in expression
        //        except for system.pascal compatibility
        //
        assert(sp->get_type()->get_subtype()->get_size_in_bits() <= 32);
        code.emit_byte(SLDC_0);
        code.emit_byte(SLDC_0);
    }

    if (sp->get_segment_number() == code.get_segment_number())
    {
        if (sp->get_lex_level() <= 0)
        {
            //
            // Call Base Procedure number UB, which is at lexical level
            // -1 or 0.
            //
            // This is symmetric with
            // ucsdpsys_compile/scope/function/compile.cc
            // scope_function_compile::post_order_traversal
            //
            code.emit_byte(CBP);
            code.emit_byte(sp->get_procedure_number());
        }
        else if (sp->get_lex_level() > code.get_lex_level())
        {
            //
            // Call local procedure number UB, which is an immediate
            // child of the currently executing procedure and in the
            // same segment.
            //
            code.emit_byte(CLP);
            code.emit_byte(sp->get_procedure_number());
        }
        else if (sp->get_lex_level() == 1)
        {
            //
            // Call global procedure number UB, which is at lexical
            // level 1 and in the same segment as the currently
            // executing procedure.
            //
            code.emit_byte(CGP);
            code.emit_byte(sp->get_procedure_number());
        }
        else
        {
            //
            // Call intermediate procedure number UB, in the same
            // segment as the currently executing procedure.
            //
            code.emit_byte(CIP);
            code.emit_byte(sp->get_procedure_number());
        }
    }
    else
    {
        //
        // Call eXternal Procedure: number UB2, in segment UB1.  Used
        // to call any procedure not in the same segment as the calling
        // procedure, including base procedures.
        //
        code.emit_byte(CXP);
        code.emit_byte(sp->get_segment_number());
        code.emit_byte(sp->get_procedure_number());
    }

    //
    // add CSP IOCHECK if needed
    //
    if (code.issue_iocheck())
    {
        bool iocheck = false;
        switch (sp->get_segment_number())
        {
        case 0:
            // FIXME: read the system sources to determine which ones
            // need to be followed by IOCHECK.
            switch (sp->get_procedure_number())
            {
            case CXP_0_FBLOCKIO:
            case CXP_0_FCLOSE:
            case CXP_0_FGET:
            case CXP_0_FOPEN:
            case CXP_0_FPUT:
            case CXP_0_FREADCHAR:
            case CXP_0_FREADINT:
            case CXP_0_FREADLN:
            case CXP_0_FREADSTRING:
            case CXP_0_FRESET:
            case CXP_0_FWRITEBYTES:
            case CXP_0_FWRITECHAR:
            case CXP_0_FWRITEINT:
            case CXP_0_FWRITELN:
            case CXP_0_FWRITESTRING:
                iocheck = true;
                break;

            default:
                break;
            }
            break;

        case DECOPS_SEG_NUM:
            switch (sp->get_procedure_number())
            {
            case DECOPS_FREADDEC:
            case DECOPS_FWRITEDEC:
                iocheck = true;
                break;

            default:
                break;
            }
            break;

        case 31:
            // FIXME: read the system sources to determine which ones
            // need to be followed by IOCHECK.
            switch (sp->get_procedure_number())
            {
            case CXP_31_FREADREAL:
            case CXP_31_FSEEK:
            case CXP_31_FWRITEREAL:
                iocheck = true;
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
        if (iocheck)
        {
            code.emit_byte(CSP);
            code.emit_byte(CSP_IOCHECK);
        }
    }
}


type::pointer
expression_function_call_user_defined::get_type()
    const
{
    return sp->get_type()->get_subtype();
}


expression::pointer
expression_function_call_user_defined::optimize(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "this = %s", lisp_representation().c_str());
    expression_list temp;
    temp.push_back(expression_error::create(get_location())); // gets discarded
    for (size_t j = 0; j < get_child_count(); ++j)
        temp.push_back(get_child(j)->optimize());
    DEBUG(2, "mark");
    expression::pointer result =
        expression_function_call_user_defined::create(sp, temp, &code);
    DEBUG(2, "return %s", result->lisp_representation().c_str());
    DEBUG(1, "}");
    return result;
}


expression::pointer
expression_function_call_user_defined::clone(const location &locn)
    const
{
    expression_list temp;
    temp.push_back(expression_error::create(locn)); // gets discarded
    for (size_t j = 0; j < get_child_count(); ++j)
        temp.push_back(get_child(j)->clone(locn));
    return create(sp, temp, &code);
}


// vim: set ts=8 sw=4 et :
