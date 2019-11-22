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

#include <lib/cardinal.h>
#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/type/anything.h>
#include <lib/type/nothing.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/record.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/new.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/symbol/function/new.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_new::~symbol_function_new()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_new::symbol_function_new(translator_compile *cntxt) :
    symbol_function("new", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // procedure new(pointer: ^void);
    //
    variable_name_list names;
    names.push_back(variable_name("pointer", location::builtin()));
    type::pointer tp = type_pointer_unpacked::create(type_anything::create());
    name_type_list args;
    args.push_back(name_type(names, tp));
    attach_function_parameters(args);
    // attach_return_type(type_nothing::create());
    DEBUG(1, "}");
}


symbol_function_new::pointer
symbol_function_new::create(translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_new(cntxt));
}


expression::pointer
symbol_function_new::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "rhs.size = %d", (int)rhs.size());

    //
    // Checking for number and types of parameters.
    //
    // rhs[0] => new itself
    // rhs[1] => the pointer
    // the rest are constants for the variant types
    //
    if (rhs.size() < 2)
    {
        parameter_error_wrong_number(rhs, 1);
        DEBUG(1, "}");
        return expression_error::create(rhs.get_location());
    }

    expression::pointer p1 = code.call_noparam_func_if_necessary(rhs[1]);
    if (!type_pointer::is_a(p1->get_type()))
    {
        oops:
        pascal_lex_error
        (
            p1->get_location(),
            "procedure NEW: parameter 1: must be a pointer variable"
        );
        DEBUG(1, "}");
        return expression_error::create(p1->get_location());
    }

    //
    // For variant records, there are optional parameters, indicating which of
    // the variants should be allocated.  They are used to calulate the size,
    // in bytes, of the heap space to be allocated.  This was done because UCSD
    // Pascal, and all early machines, were running with <= 64KiB of memory,
    // and every byte count was precious.
    //
    type::pointer tp = p1->get_type()->get_subtype();
    unsigned size_in_words = tp->get_size_in_words();
    type_record::pointer trp = boost::dynamic_pointer_cast<type_record>(tp);
    if (trp)
    {
        variant::pointer vp = trp->get_variant();
        if (vp)
        {
            DEBUG(2, "mark");
            for (size_t j = 2; ; ++j)
            {
                DEBUG(2, "j = %d", (int)j);
                assert(vp->get_size_in_words() <= size_in_words);
                size_in_words = vp->get_size_in_words();
                if (vp->terminal())
                {
                    if (j < rhs.size())
                    {
                        pascal_lex_error
                        (
                            rhs[j]->get_location(),
                            "procedure NEW: parameter %s: too many variant "
                                "selectors",
                            cardinal(j - 1).c_str()
                        );
                        return expression_error::create(rhs[j]->get_location());
                    }
                    break;
                }
                if (j >= rhs.size())
                    break;
                DEBUG(2, "mark");
                expression::pointer ep =
                    code.call_noparam_func_if_necessary(rhs[j]);
                if (!ep->is_constant())
                {
                    pascal_lex_error
                    (
                        ep->get_location(),
                        "procedure NEW: parameter %s: variant selectors must "
                            "be constant, not a %s",
                        cardinal(j - 1).c_str(),
                        ep->get_description().c_str()
                    );
                    return expression_error::create(ep->get_location());
                }
                type::pointer vtp = vp->get_type();
                if (vtp != ep->get_type())
                {
                    parameter_error_wrong_type(j, ep, vtp);
                    return expression_error::create(ep->get_location());
                }
                int n = ep->optimize()->get_integer_value();
                vp = vp->get_child(n);
                if (!vp)
                {
                    rcstring ns =
                        vtp->get_human_readable_representation_of_value(n);
                    pascal_lex_error
                    (
                        ep->get_location(),
                        "procedure NEW: parameter %s: variant selector %s not "
                            "declared",
                        cardinal(j - 1).quote_c().c_str(),
                        ns.c_str()
                    );
                    return expression_error::create(ep->get_location());
                }
            }
            DEBUG(2, "mark");
        }
        else
        {
            goto simple;
        }
    }
    else
    {
        simple:
        DEBUG(2, "mark");
        //
        // Not a record type.
        // Additional variant selectors are meaningless.
        //
        if (rhs.size() > 2)
        {
            pascal_lex_error
            (
                p1->get_location(),
                "procedure NEW: you may not specify variant parameters "
                    "for %s variables",
                tp->get_name().c_str()
            );
            return expression_error::create(rhs[2]->get_location());
        }
        DEBUG(2, "mark");
    }
    DEBUG(2, "size_in_words = %u", size_in_words);

    //
    // The first parameter is the address of the pointer variable,
    // the second parameter is the size of the object pointed to.
    //
    expression_list new_args;

    expression::pointer ptrptr = p1->strip_indirection();
    if (!ptrptr)
        goto oops;
    new_args.push_back(ptrptr);

    new_args.push_back
    (
        expression_integer_compile::create
        (
            p1->get_location(),
            size_in_words,
            &code
        )
    );

    //
    // Now build the expression node.
    //
    expression::pointer result =
        expression_function_call_new::create(new_args, &code);
    DEBUG(1, "}");
    return result;
}


expression::pointer
symbol_function_new::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
