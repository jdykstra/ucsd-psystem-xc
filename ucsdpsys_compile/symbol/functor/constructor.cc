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
#include <lib/type/nothing.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/string.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/expression/address.h>
#include <ucsdpsys_compile/symbol/functor/constructor.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_functor_constructor::~symbol_functor_constructor()
{
}


symbol_functor_constructor::symbol_functor_constructor(
    translator_compile &a1,
    const scope &a2
) :
    code(a1),
    fscope(a2)
{
}


void
symbol_functor_constructor::operator()(const symbol::pointer &sp)
{
    assert(sp);
    if (!boost::dynamic_pointer_cast<const symbol_variable>(sp))
        return;
    if (sp->get_type()->is_an_implicit_reference())
    {
        //
        // Non-var strings and array parameters have to be copied.  You can
        // tell when you have one, because is is declared as an implicit
        // reference.  Copy them into their local copy variables.
        //
        // Build an assignment expression and use that, to avoid duplicating
        // large amounts of assignment code here.
        //
        // FIXME: do records and longints do this too?
        //
        rcstring copy_name = "copy:" + sp->get_name();
        symbol::pointer copy_sp = fscope.lookup(copy_name);
        assert(copy_sp);
        expression::pointer lhs =
            copy_sp->name_expression_factory
            (
                copy_sp,
                copy_sp->get_declaration_location(),
                code.get_lex_level()
            );
        expression::pointer rhs =
            sp->name_expression_factory
            (
                sp,
                sp->get_declaration_location(),
                code.get_lex_level()
            );
        expression::pointer asgn = lhs->assignment_expression_factory(rhs);
        asgn->optimize()->traversal(0);
        return;
    }
    if (type_file::is_a(sp->get_type()))
    {
        //
        // Ininialize file variables
        //
        // PROCEDURE FINIT(VAR F: FIB; WINDOW: WINDOWP; RECWORDS: INTEGER);
        //
        // See system/system.b.text in the II.0 sources for where FINIT
        // is implemented.
        //
        expression::pointer ep =
            sp->name_expression_factory
            (
                sp,
                sp->get_declaration_location(),
                code.get_lex_level()
            );
        ep = ep->strip_indirection();
        assert(ep);
        ep->traversal(0);

        if (type_nothing::is_a(sp->get_type()->get_subtype()))
        {
            // window = NIL
            //
            // Note: this code differs from the II.0 compiler, which
            // passes a window pointer anyway, even though it is not
            // used.
            //
            // The actual code of the system call doesn't care.
            code.emit_byte(LDCN);
        }
        else
        {
            // &window
            expression_address::pointer ep2 =
                boost::dynamic_pointer_cast<expression_address>(ep);
            assert(ep2);
            expression::pointer ep3 =
                ep2->advance_by_words
                (
                    type_file::file_size_in_words,
                    type_pointer_unpacked::create
                    (
                        sp->get_type()->get_subtype()
                    ),
                    sp
                );
            assert(ep3);
            ep3->traversal(0);
        }

        //
        // There are four get_finit_recwords cases
        //     -1  file (of void)
        //     -2  text = file of char
        //     0   interactive = file of char
        //     >0  and everything else
        // the various derived classes if "type" take care of it.
        //
        code.emit_ldci(sp->get_type()->get_finit_recwords());

        code.emit_byte(CXP);
        code.emit_byte(0);
        code.emit_byte(CXP_0_FINIT);

        return;
    }
}


// vim: set ts=8 sw=4 et :
