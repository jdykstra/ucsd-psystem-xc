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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_BITWISE_NOT_ASM_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_BITWISE_NOT_ASM_H

#include <lib/expression/bitwise_not.h>

class assembler; // forward

/**
  * The expression_bitwise_not_asm class is used to represent
  * a bit-wise not expression, in the assembler.
  */
class expression_bitwise_not_asm:
    public expression_bitwise_not
{
public:
    typedef boost::shared_ptr<expression_bitwise_not_asm> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_bitwise_not_asm();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param operand
      *     The operand to the expression.
      * @param context
      *     The assembler context we are working within.
      */
    static pointer create(const expression::pointer &operand,
        assembler *context);

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param operand
      *     The operand to the expression.
      * @param context
      *     The assembler context we are working within.
      */
    expression_bitwise_not_asm(const expression::pointer &operand,
        assembler *context);

    /**
      * The context instance variable is used to remember the assembler
      * context we are working within.
      */
    assembler *context;

    /**
      * The default constructor.  Do not use.
      */
    expression_bitwise_not_asm();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_bitwise_not_asm(const expression_bitwise_not_asm &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_bitwise_not_asm &operator=(
        const expression_bitwise_not_asm &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_BITWISE_NOT_ASM_H
// vim: set ts=8 sw=4 et :
