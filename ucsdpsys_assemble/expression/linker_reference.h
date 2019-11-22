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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_LINKER_REFERENCE_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_LINKER_REFERENCE_H

#include <lib/expression.h>
#include <lib/link_info_type.h>

class assembler; // forward

/**
  * The expression_linker_reference class is used to represent
  * a reference to a symbol, that will emit link info when used.
  * There is no provision for an offset, use an addition expression node.
  */
class expression_linker_reference:
    public expression
{
public:
    typedef boost::shared_ptr<expression_linker_reference> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_linker_reference();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     where this expression appears in the source code.
      * @param litype
      *     The linker information type associated with the symbol.
      * @param tp
      *     The symbol's type (class type_linksym_relative, includes name).
      * @param offset
      *     The byte offset from the symbol.
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const location &locn, link_info_type_t litype,
        const type::pointer &tp, int offset, assembler *context);

    /**
      * The addition factory method is used to build a new
      * linker_reference expression, with the offset moved by the delta
      * amount of bytes.
      *
      * @param delta
      *     The symbol offset in bytes.
      */
    expression::pointer addition_factory(int delta) const;

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    bool is_constant(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param locn
      *     where this expression appears in the source code.
      * @param litype
      *     The linker information type associated with the symbol.
      * @param tp
      *     The symbol's type (class type_linksym_relative, includes name).
      * @param offset
      *     The byte offset from the symbol.
      * @param context
      *     The translation context we are working within.
      */
    expression_linker_reference(const location &locn, link_info_type_t litype,
        const type::pointer &tp, int offset, assembler *context);

    /**
      * The litype instance variable is used to remember the linker
      * information type associated with the symbol; this includes the
      * name of the symbol.
      */
    link_info_type_t litype;

    /**
      * The litype instance variable is used to remember the
      * type of this expression.
      */
    type::pointer tp;

    /**
      * The litype instance variable is used to remember the
      * byte offset from the referenced symbol.
      */
    int offset;

    /**
      * The context instance variable is used to remember the
      * translation context we are workign within.
      */
    assembler *context;

    /**
      * The default constructor.  Do not use.
      */
    expression_linker_reference();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_linker_reference(const expression_linker_reference &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_linker_reference &operator=(
        const expression_linker_reference &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_LINKER_REFERENCE_H
// vim: set ts=8 sw=4 et :
