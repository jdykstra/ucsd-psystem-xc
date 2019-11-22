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

#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/translator/compile.h>
#include <lib/pcode.h>


expression_real_compile::~expression_real_compile()
{
}


expression_real_compile::expression_real_compile(
    const location &a_locn,
    const rcstring &arg,
    translator_compile *a_code
) :
    expression_real(a_locn, arg),
    code(*a_code)
{
}


expression_real_compile::pointer
expression_real_compile::create(const location &a_locn, const rcstring &arg,
    translator_compile *a_code)
{
    return pointer(new expression_real_compile(a_locn, arg, a_code));
}


expression_real_compile::expression_real_compile(
    const location &a_locn,
    double arg,
    translator_compile *a_code
) :
    expression_real(a_locn, arg),
    code(*a_code)
{
}


expression_real_compile::pointer
expression_real_compile::create(const location &a_locn, double arg,
    translator_compile *a_code)
{
    return pointer(new expression_real_compile(a_locn, arg, a_code));
}


void
expression_real_compile::traversal(int)
    const
{
    code.breakpoint(get_location());
    float v = get_value();
    if (v >= -32767 && v <= 32767)
    {
        int val = int(v);
        if (float(val) == v)
        {
            //
            // Real values take 6 bytes if we represent them as a real,
            // but they only take 1, 2 or 3 bytes if we use integer
            // constants for integer-value real numbers.
            //
            code.emit_ldci(val);

            //
            // Now turn it into a float.
            //
            // So, all up, we used 2 bytes (best case) or 4 bytes (worst
            // case) to produce an real constant, as opposed to 6 or 7
            // bytes using the LDC opcode.
            //
            code.emit_byte(FLT);
            return;
        }
    }

    code.emit_word_alignment(NOP);
    code.emit_byte(LDC);
    code.emit_byte(2);

#if 0
    // this code results in "strict alias" warning */
    const unsigned short *wp = (const unsigned short *)(void *)&v;
    code.emit_word(wp[1]);
    code.emit_word(wp[0]);
#else
    assert(sizeof(float) == 4);
    assert(sizeof(unsigned short) == 2);
    union{ float f; unsigned short s[2]; } u;
    u.f = v;
    code.emit_word(u.s[1]);
    code.emit_word(u.s[0]);
#endif
}


void
expression_real_compile::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"real constants are not bool");
}


expression::pointer
expression_real_compile::optimize(void)
    const
{
    return create(get_location(), get_real_value(), &code);
}


expression::pointer
expression_real_compile::clone(const location &locn)
    const
{
    return create(locn, get_real_value(), &code);
}


// vim: set ts=8 sw=4 et :
