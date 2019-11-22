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

#include <lib/pcode.h>
#include <ucsdpsys_compile/expression/address.h>
#include <ucsdpsys_compile/expression/inc.h>
#include <ucsdpsys_compile/expression/ind.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_inc::~expression_inc()
{
}


expression_inc::expression_inc(
    const type::pointer &a_tp,
    const expression::pointer &a_lhs,
    int a_rhs,
    const symbol::pointer &a_hint,
    translator_compile *cntxt
) :
    expression(a_lhs->get_location()),
    code(*cntxt),
    tp(a_tp),
    lhs(a_lhs),
    rhs(a_rhs),
    hint(a_hint)
{
    // The Big parameter of the INC opcode can only take values in the
    // range 0..32767
    assert(rhs >= 0);
    assert(rhs < 32768);
}


expression_inc::pointer
expression_inc::create(const type::pointer &a_tp,
    const expression::pointer &a_lhs, int a_rhs, const symbol::pointer &a_hint,
    translator_compile *cntxt)
{
    return pointer(new expression_inc(a_tp, a_lhs, a_rhs, a_hint, cntxt));
}


void
expression_inc::traversal(int)
    const
{
    lhs->traversal(0);

    //
    // Rather than look for +0 all over the code, quietly take care of
    // it here.
    //
    if (rhs)
    {
        code.breakpoint(get_location());
        code.emit_byte(INC);
        code.emit_big(rhs);
    }
}


void
expression_inc::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"inc is not bool");
}


expression::pointer
expression_inc::optimize(void)
    const
{
    expression::pointer o_lhs = lhs->optimize();

    //
    // Look for expressions of the form
    //     (inc (inc ptr k1) k2)
    // and turn them into expressions of the form
    //     (inc ptr (k1+k2))
    // and optimize again
    //
    {
        expression_inc::pointer x_lhs =
            boost::dynamic_pointer_cast<expression_inc>(o_lhs);
        if (x_lhs)
        {
            int new_offset = x_lhs->get_rhs() + rhs;
            if (new_offset >= 0 && new_offset < 32768)
            {
                expression::pointer ep =
                    expression_inc::create
                    (
                        tp,
                        x_lhs->get_lhs(),
                        new_offset,
                        get_symbol_hint(),
                        &code
                    );
                return ep->optimize();
            }
        }
    }

    //
    // Each of the various load address expressions (opcodes) can have
    // their offset advanced...
    //
    {
        expression_address::pointer eap =
            boost::dynamic_pointer_cast<expression_address>(o_lhs);
        if (eap)
        {
            expression::pointer ep2 =
                eap->advance_by_words(rhs, tp, get_symbol_hint());
            if (ep2)
                return ep2;
        }
    }

    return expression_inc::create(tp, o_lhs, rhs, get_symbol_hint(), &code);
}


expression::pointer
expression_inc::clone(const location &locn)
    const
{
    return create(tp, lhs->clone(locn), rhs, get_symbol_hint(), &code);
}


symbol::pointer
expression_inc::get_symbol_hint()
    const
{
    return (hint ? hint : lhs->get_symbol_hint());
}


type::pointer
expression_inc::get_type()
    const
{
    return tp;
}


bool
expression_inc::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_inc::has_side_effect()
    const
{
    return (side_effect_no + lhs->has_side_effect());
}


int
expression_inc::get_precedence()
    const
{
    return lhs->get_precedence();
}


expression::pointer
expression_inc::indirect_load(const type::pointer &rtp, int offset)
    const
{
    int rhs2 = rhs + offset;
    if (rhs2 < 0)
        return expression::pointer();
    if (rhs2 >= 32768)
        return expression::pointer();
    return expression_ind::create(lhs, rhs2, rtp, code);
}


rcstring
expression_inc::lisp_representation(void)
    const
{
    return
        (
            "(inc {"
        +
            get_type()->get_pascal_name()
        +
            "} ["
        +
            get_location().representation()
        +
            "] "
        +
            lhs->lisp_representation()
        +
            " "
        +
            rcstring::printf("%d", rhs)
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
