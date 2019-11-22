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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_DEREFERENCE_ASM_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_DEREFERENCE_ASM_H

#include <lib/expression/dereference.h>

/**
  * The expression_dereference_asm class is used to represent
  * a dereference operation, specific to the assembler.
  */
class expression_dereference_asm:
    public expression_dereference
{
public:
    typedef boost::shared_ptr<expression_dereference_asm> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_dereference_asm();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param operand
      *     The operand of the dereference.
      */
    static pointer create(const expression::pointer &operand);

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param operand
      *     The operand of the dereference.
      */
    expression_dereference_asm(const expression::pointer &operand);

    /**
      * The default constructor.  Do not use.
      */
    expression_dereference_asm();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_dereference_asm(const expression_dereference_asm &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_dereference_asm &operator=(
        const expression_dereference_asm &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_DEREFERENCE_ASM_H
// vim: set ts=8 sw=4 et :
