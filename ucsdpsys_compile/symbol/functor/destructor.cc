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
#include <lib/symbol/variable.h>
#include <lib/type/file.h>
#include <ucsdpsys_compile/symbol/functor/destructor.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_functor_destructor::~symbol_functor_destructor()
{
}


symbol_functor_destructor::symbol_functor_destructor(
    const location &a_locn,
    translator_compile &a
) :
    locn(a_locn),
    code(a)
{
}


void
symbol_functor_destructor::operator()(const symbol::pointer &sp)
{
    assert(sp);
    if (!boost::dynamic_pointer_cast<symbol_variable>(sp))
        return;
    if (type_file::is_a(sp->get_type()))
    {
        //
        // Initialize file variables
        //
        // PROCEDURE FCLOSE(VAR F: FIB; FTYPE: CLOSETYPE);
        //
        // See system/system.b.text in the II.0 sources for where FCLOSE
        // is implemented.
        //

        code.breakpoint(locn);
        expression::pointer ep =
            sp->name_expression_factory
            (
                sp,
                locn,
                code.get_lex_level()
            );
        ep = ep->strip_indirection();
        assert(ep);
        ep->traversal(0);

        code.emit_byte(CNORMAL);

        code.emit_byte(CXP);
        code.emit_byte(0);
        code.emit_byte(CXP_0_FCLOSE);

        return;
    }
}


// vim: set ts=8 sw=4 et :
