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
#include <lib/type/anything.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/set/sized.h>
#include <lib/type/subrange.h>

#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_set_compile::~expression_set_compile()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


type::pointer
expression_set_compile::minimum_subrange(const type::pointer &a_tp,
    const set &value)
{
    type::pointer tp = a_tp;
    assert(type_set::is_a(tp));
    tp = tp->get_subtype();

    if (type_anything::is_a(tp))
    {
        // Empty set constants are initially type-less.  They remain that way
        // until there is enough context to give them a more specific type.
        assert(value.empty());
        return a_tp;
    }

    while (type_subrange::is_a(tp))
        tp = tp->get_subtype();
    if (type_boolean::is_a(tp))
        return tp;

    if
    (
        type_integer::is_a(tp)
    ||
        type_enumerated::is_a(tp)
    ||
        type_char::is_a(tp)
    )
    {
        int nwords = value.size();
        if (nwords < 1)
            nwords = 1;
        int maxval = (16 * nwords) - 1;
        int hi = tp->get_maximum_value();
        if (maxval > hi)
            maxval = hi;

        if (maxval == hi)
            return type_set_sized::create(tp);
        return type_set_sized::create(type_subrange::create(0, maxval, tp));
    }

    DEBUG(2, "tp = %s", tp->get_pascal_name().c_str());
    assert(!"should not be able to create a set of this type");
    return a_tp;
}


expression_set_compile::expression_set_compile(
    const location &a_locn,
    const type::pointer &a_tp,
    const set &a_value,
    translator_compile *cntxt
) :
#if 0
    expression_set(a_locn, minimum_subrange(a_tp, a_value), a_value),
#else
    expression_set(a_locn, a_tp, a_value),
#endif
    code(*cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "a_tp = %s", a_tp->get_pascal_name().c_str());
    assert(type_set::is_a(a_tp));
    assert(!boost::dynamic_pointer_cast<type_integer>(a_tp->get_subtype()));
}


expression_set_compile::pointer
expression_set_compile::create(const location &a_locn,
    const type::pointer &a_tp, const set &a_value, translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_set_compile(a_locn, a_tp, a_value, cntxt));
}


static inline int
min(int a, int b)
{
    return (a < b ? a : b);
}


void
expression_set_compile::traversal(int)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    const set &s = get_value();
    DEBUG(2, "value = %s", s.representation().c_str());
    code.breakpoint(get_location());
    if (type_set_sized::is_a(get_type()))
    {
        //
        // The value on the stack is to include the set size in words.
        //
        size_t set_nwords = s.size();
        DEBUG(2, "set_nwords = %d", int(set_nwords));
        assert(set_nwords < 256);

        if (set_nwords == 0)
        {
            // do nothing
        }
        else if (set_nwords == 1)
        {
            code.emit_ldci(s.nth(0));
        }
        else
        {
            code.emit_word_alignment(NOP);
            code.emit_byte(LDC);
            code.emit_byte(set_nwords);
            for (size_t jj = 0; jj < set_nwords; ++jj)
            {
                size_t j = set_nwords - 1 - jj;
                int n = s.nth(j);
                DEBUG(2, "word %d = 0x%04X", (int)j, n);
                code.emit_word(n);
            }
        }
        code.emit_ldci(set_nwords);
    }
    else
    {
        //
        // The value on the stack does NOT include the set size in
        // words.
        //
        // Note: The following code optimizes for space.  If we were
        //       optimizing for speed we would use LDC every time.
        //
        assert(!type_anything::is_a(get_type()->get_subtype()));
        DEBUG(2, "tp = %s", get_type()->get_pascal_name().c_str());
        unsigned nwords = get_type()->get_size_in_words();
        DEBUG(2, "nwords = %u", nwords);
        assert(nwords < 256);

        size_t set_nwords = s.size();
        DEBUG(2, "set_nwords = %d", int(set_nwords));
        assert(set_nwords < 256);

        // LDC n w...w
        int ldc_num_bytes_without_adj = 2 + nwords * 2;
        DEBUG(2, "ldc_num_bytes_without_adj = %d", ldc_num_bytes_without_adj);
        // LDCI*n
        int ldci_num_bytes_without_adj = 0;
        for (size_t jj = 0; jj < nwords; ++jj)
            ldci_num_bytes_without_adj +=
                code.ldci_nbytes(jj < set_nwords ? s.nth(jj) : 0);
        DEBUG(2, "ldci_num_bytes_without_adj = %d", ldci_num_bytes_without_adj);
        int num_bytes_without_adj =
            min(ldc_num_bytes_without_adj, ldci_num_bytes_without_adj);
        DEBUG(2, "num_bytes_without_adj = %d", num_bytes_without_adj);

        // LDC n w...w + SLDC n + ADJ n
        int ldc_num_bytes_with_adj =
            (set_nwords == 0 ? 0 : (2 + set_nwords * 2)) +
            (set_nwords != nwords ? code.ldci_nbytes(nwords) + 2 : 0);
        DEBUG(2, "ldc_num_bytes_with_adj = %d", ldc_num_bytes_with_adj);

        // LDC n w...w + SLDC n + ADJ n
        int ldci_num_bytes_with_adj =
            (set_nwords != nwords ? code.ldci_nbytes(nwords) + 2 : 0);
        for (size_t jj = 0; jj < set_nwords; ++jj)
            ldci_num_bytes_with_adj += code.ldci_nbytes(s.nth(jj));
        DEBUG(2, "ldci_num_bytes_with_adj = %d", ldci_num_bytes_with_adj);
        int num_bytes_with_adj =
            min(ldc_num_bytes_with_adj, ldci_num_bytes_with_adj);
        DEBUG(2, "num_bytes_with_adj = %d", num_bytes_with_adj);

        if (num_bytes_with_adj < num_bytes_without_adj)
        {
            if (ldc_num_bytes_with_adj < ldci_num_bytes_with_adj)
            {
                DEBUG(2, "ldc_num_bytes_with_adj wins");
                if (set_nwords)
                {
                    code.emit_word_alignment(NOP);
                    code.emit_byte(LDC);
                    code.emit_byte(set_nwords);
                    for (size_t jj = 0; jj < set_nwords; ++jj)
                    {
                        size_t j = set_nwords - 1 - jj;
                        int n = s.nth(j);
                        DEBUG(2, "word %d = 0x%04X", int(j), n);
                        code.emit_word(n);
                    }
                }
            }
            else
            {
                DEBUG(2, "ldci_num_bytes_with_adj wins");
                for (size_t jj = 0; jj < set_nwords; ++jj)
                {
                    size_t j = set_nwords - 1 - jj;
                    int n = s.nth(j);
                    DEBUG(2, "word %d = 0x%04X", int(j), n);
                    code.emit_ldci(n);
                }
            }
            if (set_nwords != nwords)
            {
                code.emit_ldci(set_nwords);
                code.emit_byte(ADJ);
                code.emit_byte(nwords);
            }
        }
        else
        {
            if (ldc_num_bytes_without_adj < ldci_num_bytes_without_adj)
            {
                DEBUG(2, "ldc_num_bytes_without_adj wins");
                code.emit_word_alignment(NOP);
                code.emit_byte(LDC);
                code.emit_byte(nwords);
                for (size_t jj = 0; jj < nwords; ++jj)
                {
                    size_t j = nwords - 1 - jj;
                    int n = j < set_nwords ? s.nth(j) : 0;
                    DEBUG(2, "word %d = 0x%04X", int(j), n);
                    code.emit_word(n);
                }
            }
            else
            {
                DEBUG(2, "ldci_num_bytes_without_adj wins");
                for (size_t jj = 0; jj < nwords; ++jj)
                {
                    size_t j = nwords - 1 - jj;
                    int n = j < set_nwords ? s.nth(j) : 0;
                    DEBUG(2, "word %d = 0x%04X", int(j), n);
                    code.emit_ldci(n);
                }
            }
        }
    }
    DEBUG(1, "}");
}


expression::pointer
expression_set_compile::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(get_location(), get_type(), get_value(), &code);
}


expression::pointer
expression_set_compile::clone(const location &locn)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(locn, get_type(), get_value(), &code);
}


bool
expression_set_compile::get_bit(unsigned n)
    const
{
    return get_value().get_bit(n);
}


// vim: set ts=8 sw=4 et :
