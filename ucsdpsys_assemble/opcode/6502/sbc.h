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

#ifndef UCSDPSYS_ASSEMBLE_OPCODE_6502_SBC_H
#define UCSDPSYS_ASSEMBLE_OPCODE_6502_SBC_H

#include <ucsdpsys_assemble/opcode/6502.h>

/**
  * The opcode_6502_sbc class is used to represent
  * the sbc opcode.
  */
class opcode_6502_sbc:
    public opcode_6502
{
public:
    typedef boost::shared_ptr<opcode_6502_sbc> pointer;

    /**
      * The destructor.
      */
    virtual ~opcode_6502_sbc();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param context
      *     The assemly context we are working within.
      * @param model
      *     The model of 6502 we are targeting.
      */
    static pointer create(assembler *context, model_t model);

protected:
    // See base class for documentation.
    void interpret(const expression_list &args);

    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    kind_t get_kind(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param context
      *     The assemly context we are working within.
      * @param model
      *     The model of 6502 we are targeting.
      */
    opcode_6502_sbc(assembler *context, model_t model);

    /**
      * The default constructor.  Do not use.
      */
    opcode_6502_sbc();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    opcode_6502_sbc(const opcode_6502_sbc &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    opcode_6502_sbc &operator=(const opcode_6502_sbc &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_OPCODE_6502_SBC_H
// vim: set ts=8 sw=4 et :
