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
#include <lib/pcode.h>
#include <lib/type/set/sized.h>
#include <lib/type/set/unsized.h>

#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/expression/set2set/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_set2set_compile::~expression_set2set_compile()
{
}


expression_set2set_compile::expression_set2set_compile(
    const type::pointer &a_to,
    const expression::pointer &a_from,
    translator_compile *cntxt
) :
    expression_set2set(a_to, a_from),
    code(*cntxt)
{
}


expression::pointer
expression_set2set_compile::create(const type::pointer &a_to,
    const expression::pointer &a_from, translator_compile *cntxt)
{
    if (type_set_sized::is_a(a_to) && type_set_sized::is_a(a_from->get_type()))
        return a_from;
    return pointer(new expression_set2set_compile(a_to, a_from, cntxt));
}


void
expression_set2set_compile::post_order_traversal(int)
    const
{
    type::pointer to_type = get_type();
    type::pointer from_type = get_from()->get_type();
    if (type_set_sized::is_a(to_type))
    {
        if (type_set_sized::is_a(from_type))
        {
            // To a sized type
            // from a sized type...
            // do nothing.
        }
        else
        {
            // To a sized type
            // from an unsized type...
            // just push the size onto the stack.
            unsigned from_words = from_type->get_size_in_words();
            code.breakpoint(get_location());
            code.emit_ldci(from_words);
        }
    }
    else
    {
        unsigned to_words = to_type->get_size_in_words();
        assert(to_words < 256);
        if (type_set_sized::is_a(from_type))
        {
            // To an unsized type
            // from a sized type
            code.breakpoint(get_location());
            code.emit_byte(ADJ);
            code.emit_byte(to_words);
        }
        else
        {
            // To an unsized type
            // from an unsized type
            unsigned from_words = from_type->get_size_in_words();
            if (to_words != from_words)
            {
                // Only need to adjust if they differ in size.
                code.breakpoint(get_location());
                code.emit_ldci(from_words);
                code.emit_byte(ADJ);
                code.emit_byte(to_words);
            }
        }
    }
}


expression::pointer
expression_set2set_compile::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression::pointer a1 = get_from();
    DEBUG(2, "a1 = %s", a1->lisp_representation().c_str());
    expression::pointer e1 = a1->optimize();
    DEBUG(2, "e1 = %s", e1->lisp_representation().c_str());
    if (e1->is_constant())
    {
        expression_set::pointer esp =
            boost::dynamic_pointer_cast<expression_set>(e1);
        assert(esp);
        expression::pointer e2 =
            expression_set_compile::create
            (
                get_location(),
                get_type(),
                esp->get_value(),
                &code
            );
        DEBUG(2, "e2 = %s", e2->lisp_representation().c_str());
        return e2;
    }

    return create(get_type(), e1, &code);
}


expression::pointer
expression_set2set_compile::clone(const location &locn)
    const
{
    return create(get_type(), get_from()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
