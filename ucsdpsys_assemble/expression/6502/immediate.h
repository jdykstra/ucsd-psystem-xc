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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_6502_IMMEDIATE_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_6502_IMMEDIATE_H

#include <ucsdpsys_assemble/expression/6502.h>

/**
  * The expression_6502_immediate class is used to represent a 6502 opcodee
  * argument of immediate mode, typically written "# <expr>"
  */
class expression_6502_immediate:
    public expression_6502
{
public:
    typedef boost::shared_ptr<expression_6502_immediate> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_6502_immediate();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param operand
      *     The value to be encoded.
      */
    static pointer create(const expression::pointer &operand);

    /**
      * The get_operand method may be used to obtain the value to be
      * encoded.
      */
    expression::pointer get_operand(void) const { return operand; }

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param operand
      *     The value to be encoded.
      */
    expression_6502_immediate(const expression::pointer &operand);

    /**
      * The operand instance vaiable is used to remember the value to be
      * encoded.
      */
    expression::pointer operand;

    /**
      * The default constructor.  Do not use.
      */
    expression_6502_immediate();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_6502_immediate(const expression_6502_immediate &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_6502_immediate &operator=(const expression_6502_immediate &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_6502_IMMEDIATE_H
// vim: set ts=8 sw=4 et :
