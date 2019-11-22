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

#include <lib/ac/string.h>

#include <lib/pcode.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/expression/lpa.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_lpa::~expression_lpa()
{
    delete [] data;
}


static unsigned char *
copy_of(const void *data, size_t data_size)
{
    unsigned char *result = new unsigned char [data_size];
    memcpy(result, data, data_size);
    return result;
}


expression_lpa::expression_lpa(
    const void *a_data,
    size_t a_data_size,
    const location &locn,
    const type::pointer &a_atype,
    translator_compile *a_code
) :
    expression(locn),
    data(copy_of(a_data, a_data_size)),
    data_size(a_data_size),
    atype(a_atype),
    code(*a_code)
{
    assert(a_data_size < 256);
}


expression_lpa::pointer
expression_lpa::create(const void *a_data, size_t a_data_size,
    const location &locn, const type::pointer &a_atype,
    translator_compile *a_code)
{
    return
        pointer
        (
            new expression_lpa(a_data, a_data_size, locn, a_atype, a_code)
        );
}


void
expression_lpa::traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_word_alignment(NOP);
    code.emit_byte(LPA);
    code.emit_big(data_size); // in bytes
    for (size_t j = 0; j < data_size; ++j)
        code.emit_byte(data[j]);
}


void
expression_lpa::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"this should not happen, it's nothing like a bool");
}


int
expression_lpa::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_lpa::get_type()
    const
{
    return type_pointer_unpacked::create(atype);
}


bool
expression_lpa::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_lpa::has_side_effect()
    const
{
    return side_effect_no;
}


expression::pointer
expression_lpa::optimize(void)
    const
{
    return create(data, data_size, get_location(), atype, &code);
}


expression::pointer
expression_lpa::clone(const location &locn)
    const
{
    return create(data, data_size, locn, atype, &code);
}


rcstring
expression_lpa::lisp_representation(void)
    const
{
    return
        rcstring::printf
        (
            "(lpa {%s} %p %d)",
            get_type()->get_pascal_name().c_str(),
            data,
            (int)data_size
        );
}


// vim: set ts=8 sw=4 et :
