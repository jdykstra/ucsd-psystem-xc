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

#include <lib/cardinal.h>
#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/symbol/function.h>
#include <lib/translator.h>
#include <lib/type/anything.h>
#include <lib/type/char.h>
#include <lib/type/function.h>
#include <lib/type/integer.h>
#include <lib/type/long_integer.h>
#include <lib/type/nothing.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/real.h>
#include <lib/type/reference.h>
#include <lib/type/set.h>
#include <lib/type/string.h>


symbol_function::~symbol_function()
{
}


symbol_function::symbol_function(
    const rcstring &a_name,
    const scope::pointer &a_locals,
    int lxl,
    bool seg_proc
) :
    symbol(a_name, type_function::create(), lxl),
    forward_decl(false),
    locals(a_locals),
    ignore_segment_zero(false),
    segment_procedure(seg_proc)
{
    // It is tempting to say
    //
    //     assert(locals.is_not_null());
    //
    // except that it isn't true for built-in functions.
}


symbol_function::symbol_function(
    const rcstring &a_name,
    const location &dec_loc,
    const scope::pointer &a_locals,
    int lxl,
    bool seg_proc
) :
    symbol(a_name, dec_loc, type_function::create(), lxl),
    forward_decl(false),
    locals(a_locals),
    segment_procedure(seg_proc)
{
    // It is tempting to say
    //
    //     assert(locals.is_not_null());
    //
    // except that it isn't true for built-in functions.
}


void
symbol_function::attach_return_type(const type::pointer &tp)
{
    change_type(type_function::create(tp));
}


void
symbol_function::attach_function_parameters(const name_type_list &ntl)
{
    ntl.flatten_into(parameters);
}


void
symbol_function::parameter_error_wrong_number(const expression_list &given,
    int requires) const
{
    pascal_lex_error
    (
        given.get_location(),
        "call of %s %s with %s parameter%s, "
            "but it requires %s parameter%s",
        get_kind(),
        get_name().quote_c().c_str(),
        cardinal(given.size() - 1).c_str(),
        (given.size() == 2 ? "" : "s"),
        cardinal(requires).c_str(),
        (requires == 1 ? "" : "s")
    );
}


void
symbol_function::parameter_error_wrong_type(int pnum,
    const expression::pointer &given, const type::pointer &requires) const
{
    // don't nag
    if (given->is_error())
        return;

    pascal_lex_error
    (
        given->get_location(),
        "call of %s %s: parameter %s: given a %s, "
            "but it requires a %s",
        get_kind(),
        get_name().quote_c().c_str(),
        cardinal(pnum).c_str(),
        given->get_description().c_str(),
        requires->get_name().c_str()
    );
}


void
symbol_function::check_string_size_match(const name_type &nt,
    const expression::pointer &ep, int pnum) const
{
#if 0
    if (!issue_range_check())
        return;
#endif
    type::pointer formal_type = nt.get_type();
    if (type_reference::is_a(formal_type))
        formal_type = formal_type->get_subtype();
    type_string::pointer formal =
        boost::dynamic_pointer_cast<type_string>(formal_type);
    if (!formal)
        return;

    type::pointer actual_type = ep->get_type();
    if (type_reference::is_a(actual_type))
        actual_type = actual_type->get_subtype();
    type_string::pointer actual =
        boost::dynamic_pointer_cast<type_string>(actual_type);
    if (!actual)
        return;

    if (actual->get_size() > formal->get_size())
    {
        //
        // This is a warning, rather than an error, because when the copy is
        // performed by the SAS opcode at the start of the function, it will
        // give a run-time error if the string won't fit.
        //
        pascal_lex_warning
        (
            ep->get_location(),
            "call of %s %s: parameter %s: maximum string length of "
                "parameter (%d) is greater than...",
            get_kind(),
            get_name().quote_c().c_str(),
            cardinal(pnum).c_str(),
            actual->get_size()
        );
        pascal_lex_warning
        (
            nt.get_name(0).get_location(),
            "... declared maximum string length (%d)",
            formal->get_size()
        );
    }
}


expression_list
symbol_function::check_parameters(const expression_list &rhs, translator &xlat)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // rhs[0] is the function name, so everything is off by one.
    //
    bool ok = true;
    expression_list new_rhs;
    new_rhs.push_back(rhs[0]);
    const char *kind = get_kind();
    if (rhs.size() != parameters.size() + 1)
    {
        DEBUG(2, "mark");
        pascal_lex_error
        (
            rhs[0]->get_location(),
            "call of %s %s with %s parameter%s...",
            kind,
            get_name().quote_c().c_str(),
            cardinal(rhs.size() - 1).c_str(),
            (rhs.size() == 2 ? "" : "s")
        );
        DEBUG(2, "mark");
        pascal_lex_error
        (
            get_declaration_location(),
            "... but the declaration has %s parameter%s",
            cardinal(parameters.size()).c_str(),
            (parameters.size() == 1 ? "" : "s")
        );
        ok = false;
    }
    for (size_t j = 0; j < parameters.size() && j + 1 < rhs.size(); ++j)
    {
        DEBUG(2, "j = %d", (int)j);
        const name_type &nt = get_parameter(j);
        expression::pointer ep = rhs[j + 1];
        if (ep->is_error())
        {
            ok = false;
            continue;
        }
        DEBUG(2, "ep = %s", ep->lisp_representation().c_str());

        //
        // Silent cast of string constants to char constants.
        //
        if (type_char::is_a(nt.get_type()))
        {
            expression::pointer ep2 = xlat.char_from_string_of_length_one(ep);
            if (ep2)
            {
                ep = ep2;
                DEBUG(2, "ep = %s", ep->lisp_representation().c_str());
            }
        }

        //
        // Silent cast of string constants to packed array of char constants.
        // Not so silent if there was a problem.
        //
        if (type_string::is_a(ep))
        {
            type::pointer ft = nt.get_type();
            if (ft->is_an_implicit_reference())
                ft = ft->get_subtype();
            type_array::pointer tap =
                boost::dynamic_pointer_cast<type_array>(ft);
            if (tap)
            {
                ep = xlat.optional_paoc_from_string(tap, ep);
                DEBUG(1, "ep = %s", ep->lisp_representation().c_str());
            }
        }

        //
        // Ugly Pascal-ism: functions with no parameters get no parentheses.
        // Whena function name appears in a value context, assume that we are
        // supposed to call it woth no parameters.
        //
        // If UCSD pascal could pass procedures and functions as parameters
        // (like a C pointer to function) this would be conditional on the
        // declared type of the parameters.  But UCSD Pascal doesn't, so this
        // is unconditional.
        //
        // Ponder this:  The CXP opcode takes two values off the stack, for
        // segment number and procedure number.  If procedure "pointers" were
        // 32 bits, it would be simple enough to build such values and pass
        // them around.  BTW: this is how the built-in EXIT procedure works, so
        // it would regularise an exception.
        //
        ep = xlat.call_noparam_func_if_necessary(ep);

        //
        // Silent promotion of integer values to real values.
        // (but no auto promotion if reference to real)
        //
        if
        (
            type_real::is_a(nt.get_type())
        &&
            type_integer::is_a(ep->get_type())
        )
        {
            ep = xlat.real_from_integer_expression_factory(ep);
        }

        //
        // Silent promotion of integer values to long integer values.
        //
        if
        (
            type_long_integer::is_a(nt.get_type())
        &&
            type_integer::is_a(ep->get_type())
        )
        {
            ep = xlat.lint_from_integer_expression_factory(ep);
        }

        //
        // Not-so-silent conversion of long integers to integers.
        // (The common code issues the error.)
        //
        if
        (
            type_integer::is_a(nt.get_type())
        &&
            type_long_integer::is_a(ep->get_type())
        )
        {
            ep = xlat.integer_from_lint_expression(ep);
        }

        //
        // Silent conversion from one long integer type to another.
        //
        if
        (
            type_long_integer::is_a(nt.get_type())
        &&
            type_long_integer::is_a(ep)
        )
        {
            ep = xlat.lint2lint_expression_factory(nt.get_type(), ep);
        }

        // This will ignore non-strings
        check_string_size_match(nt, ep, j + 1);

        //
        // We need to give the empty set a concrete type.
        //
        if
        (
            type_set::is_a(nt.get_type())
        &&
            type_set::is_a(ep->get_type())
        &&
            type_anything::is_a(ep->get_type()->get_subtype())
        )
        {
            assert(ep->is_constant());
            ep = xlat.set2set_expression_factory(nt.get_type(), ep);
            DEBUG(2, "ep = %s", ep->lisp_representation().c_str());
        }

        //
        // Silently adjust set sizes.
        //
        if (type_set::is_a(nt.get_type()) && type_set::is_a(ep->get_type()))
        {
            // This will do nothing, whenever possible.
            ep = xlat.set2set_expression_factory(nt.get_type(), ep);
        }

        //
        // Check that the types match.
        //
        if (nt.get_type() == ep->get_type())
        {
            // All good
            // (even if both are references)
        }
        else if (type_reference::is_a(nt.get_type()))
        {
            if (nt.get_type()->is_an_implicit_reference())
            {
                // Implicit references can accept value parameters.
                // So far they only apply to strings and arrays and
                // records, but longints may be relevant one day.
                type::pointer type_expected = nt.get_type()->get_subtype();

                if
                (
                    type_expected != ep->get_type()
                &&
                    nt.get_type() != ep->get_type()
                )
                {
                    oops:
                    pascal_lex_error
                    (
                        ep->get_location(),
                        "call of %s %s: parameter %s: given a %s...",
                        kind,
                        get_name().quote_c().c_str(),
                        cardinal(j + 1).c_str(),
                        ep->get_description().c_str()
                    );
                    const variable_name &vn = nt.get_name(0);
                    pascal_lex_error
                    (
                        vn.get_location(),
                        "... but declaration of %s is %s type",
                        vn.get_name().quote_c().c_str(),
                        nt.get_type()->get_name().c_str()
                    );
                    ok = false;
                }
                if (!type_string::is_a(ep))
                {
                    expression::pointer ep2 = ep->strip_indirection();
                    if (!ep2)
                    {
                        assert(!"should have been possible to take the addr");
                        goto oops;
                    }
                    ep = ep2;
                }
            }
            else
            {
                //
                // If the function parameter type is a reference, but the
                // value is not, see if we can turn the value into a
                // reference (i.e. pass the address).
                //
                expression::pointer ep2 = ep->strip_indirection();

                //
                // Care must be taken that we expect "pointer to" as the
                // result type.
                //
                type::pointer type_expected =
                    type_pointer_unpacked::create(nt.get_type()->get_subtype());

                if
                (
                    !ep2
                ||
                    (
                        type_expected != ep2->get_type()
                    &&
                        nt.get_type() != ep2->get_type()
                    )
                )
                {
                    pascal_lex_error
                    (
                        ep->get_location(),
                        "call of %s %s: parameter %s: given a %s...",
                        kind,
                        get_name().quote_c().c_str(),
                        cardinal(j + 1).c_str(),
                        ep->get_description().c_str()
                    );
                    const variable_name &vn = nt.get_name(0);
                    // this will put the "reference to" in the message when
                    // it builds the name of the type.
                    pascal_lex_error
                    (
                        vn.get_location(),
                        "... but declaration of %s is %s type",
                        vn.get_name().quote_c().c_str(),
                        nt.get_type()->get_name().c_str()
                    );
                    ok = false;
                }
                else
                    ep = ep2;
            }
        }
        else
        {
            pascal_lex_error
            (
                ep->get_location(),
                "call of %s %s: parameter %s: given a %s...",
                kind,
                get_name().quote_c().c_str(),
                cardinal(j + 1).c_str(),
                ep->get_description().c_str()
            );
            assert(nt.size() == 1);
            const variable_name &vn = nt.get_name(0);
            pascal_lex_error
            (
                vn.get_location(),
                "... but declaration of %s is %s type",
                vn.get_name().quote_c().c_str(),
                nt.get_type()->get_name().c_str()
            );
            ok = false;
        }
        new_rhs.push_back(ep);
    }
    DEBUG(2, "ok = %d", ok);
    if (!ok)
        new_rhs.clear();
    DEBUG(1, "}");
    return new_rhs;
}


expression::pointer
symbol_function::function_call_expression_factory(const expression_list &rhs)
    const
{
    assert(rhs.size() >= 1);
    location here = rhs.get_location();
    pascal_lex_error(here, "left hand side of function call not appropriate");
    return expression_error::create(here);
}


scope::pointer
symbol_function::get_locals()
{
    notify_forward_resume();
    return locals;
}


void
symbol_function::notify_forward_resume(void)
{
}


void
symbol_function::set_forward(bool ign_seg0)
{
    assert(locals);
    forward_decl = true;
    ignore_segment_zero = ign_seg0;
    notify_forward_suspend();
}


void
symbol_function::notify_forward_suspend(void)
{
}


void
symbol_function::drop_locals(void)
{
    assert(locals);

    //
    // The function is defined in its own scope, as well as in its
    // parent's scioe.  We have to make it defined *before* we check
    // that all of our nested functions are defined.
    //
    forward_decl = false;

    //
    // Now make sure all or nested functions are defined.
    //
    locals->check_defined();

    //
    // Now toss all our local variables and functions.
    //
    locals.reset();
}


void
symbol_function::check_defined()
{
    if (ignore_segment_zero && get_segment_number() == 0)
    {
        // Do nothing.
    }
    else if (forward_decl)
    {
        pascal_lex_error
        (
            get_declaration_location(),
            "%s %s was declared forward, but has not been defined",
            get_kind(),
            get_name().quote_c().c_str()
        );
    }
    else
    {
        symbol::check_defined();
    }
}


const char *
symbol_function::get_kind()
    const
{
    assert(type_function::is_a(get_type()));
    if (type_nothing::is_a(get_type()->get_subtype()))
        return "procedure";
    return "function";
}


unsigned
symbol_function::get_segment_number(void)
    const
{
    // Fake it.
    return 1;
}


unsigned
symbol_function::get_procedure_number(void)
    const
{
    // Dummy.
    return 1;
}


// vim: set ts=8 sw=4 et :
