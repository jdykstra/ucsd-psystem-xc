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

#ifndef UCSDPSYS_ASSEMBLE_PERSONALITY_6502_H
#define UCSDPSYS_ASSEMBLE_PERSONALITY_6502_H

#include <ucsdpsys_assemble/personality.h>
#include <ucsdpsys_assemble/opcode/6502.h>

class assembler; // forward

/**
  * The personality_6502 class is used to represent the MOS Technology
  * 6502 microprocessor, and the interpretation of its assembly language
  * opcodes.
  */
class personality_6502:
    public personality
{
public:
    typedef boost::shared_ptr<personality_6502> pointer;

    typedef opcode_6502::model_t chip_model_t;

    /**
      * The destructor.
      */
    virtual ~personality_6502();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param context
      *     The assembly context we are working within.
      * @param model
      *     The model of 6502 being assembled.  It matters because not
      *     all models have all opcodes, or all addressing modes.
      */
    static personality::pointer create(assembler *context, chip_model_t model);

    /**
      * The create_orig class method is used to create new dynamically
      * allocated instances of this class, using chip_model_6502.
      *
      * @param context
      *     The assembly context we are working within.
      */
    static personality::pointer create_6502(assembler *context);

    /**
      * The create_65c02 class method is used to create new dynamically
      * allocated instances of this class, using chip_model_65c02.
      *
      * @param context
      *     The assembly context we are working within.
      */
    static personality::pointer create_65c02(assembler *context);

    /**
      * The create_65c02s class method is used to create new dynamically
      * allocated instances of this class, using chip_model_65c02s.
      *
      * @param context
      *     The assembly context we are working within.
      */
    static personality::pointer create_65c02s(assembler *context);

    /**
      * The create_65c02s_wdc class method is used to create new dynamically
      * allocated instances of this class, using chip_model_65c02s_wdc.
      *
      * @param context
      *     The assembly context we are working within.
      */
    static personality::pointer create_65c02s_wdc(assembler *context);

protected:
    // See base class for documentation.
    mtype_t get_mtype(void) const;

    // See base class for documentation.
    unsigned char get_one_byte_nop(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param context
      *     The assembly context we are working within.
      * @param model
      *     The model of 6502 being assembled.  It matters because not
      *     all models have all opcodes, or all addressing modes.
      */
    personality_6502(assembler *context, chip_model_t model);

    /**
      * The context instance variable is used to remember the assembly
      * context we are working within.
      */
    assembler &context;

    /**
      * The default constructor.  Do not use.
      */
    personality_6502();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    personality_6502(const personality_6502 &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    personality_6502 &operator=(const personality_6502 &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_PERSONALITY_6502_H
// vim: set ts=8 sw=4 et :
