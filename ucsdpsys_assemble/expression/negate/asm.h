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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_NEGATE_ASM_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_NEGATE_ASM_H

#include <lib/expression/negate.h>

class assembler; // forward

/**
  * The expression_negate_asm class is used to represent
  * a unary minus operation, in the assembler.
  */
class expression_negate_asm:
    public expression_negate
{
public:
    typedef boost::shared_ptr<expression_negate_asm> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_negate_asm();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param operand
      *     The expression to be negated.
      * @param context
      *     The assembler context we are operating within.
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
      *     The expression to be negated.
      * @param context
      *     The assembler context we are operating within.
      */
    expression_negate_asm(const expression::pointer &operand,
        assembler *context);

    /**
      * The context instance variable is used to remember the assembler
      * context we are operating within.
      */
    assembler *context;

    /**
      * The default constructor.  Do not use.
      */
    expression_negate_asm();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_negate_asm(const expression_negate_asm &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_negate_asm &operator=(const expression_negate_asm &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_NEGATE_ASM_H
// vim: set ts=8 sw=4 et :
