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

#ifndef UCSDPSYS_ASSEMBLE_OPCODE_6502_H
#define UCSDPSYS_ASSEMBLE_OPCODE_6502_H

#include <ucsdpsys_assemble/opcode.h>

/**
  * The opcode_6502 class is used to represent
  * a generic 6502 opcode.
  *
  * The 6502 has had several revisions, some opcodes are only relevant
  * to some revision.  We always accept them all, but we issue warnings
  * if they appear to be using too early a model.
  */
class opcode_6502:
    public opcode
{
public:
    typedef boost::shared_ptr<opcode_6502> pointer;

    /**
      * There are several revisions of the 6502, this enum captures some
      * of them (roughly in order) so that warnings can be issued.
      */
    enum model_t
    {
        model_6502,
        model_65c02,
        model_65c02s,
        model_65c02s_wdc,
    };

    /**
      * The destructor.
      */
    virtual ~opcode_6502();

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
      *
      * @param context
      *     The assembly context we are working within.
      * @param model
      *     which model of 6502 we are assemblin for,
      *     used to warn about unsupported opcodes.
      */
    opcode_6502(assembler *context, model_t model = model_6502);

    /**
      * The mode_is_immediate method is used to examine the opcode
      * argument list, determine whether or not is looks like an
      * Immediate opcode, typically written
      *
      *     Mnemonic &#35;value
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_immediate(const expression_list &args, unsigned char opc);

    /**
      * The mode_is_absolute method is used to examine the opcode
      * argument list, determine whether or not is looks like an
      * Absolute opcode, typically written
      *
      *     Mnemonic address
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_absolute(const expression_list &args, unsigned char opc);

    /**
      * The mode_is_segrel2absolute method is used to examine the opcode
      * argument list, determine whether or not is looks like an segment
      * relative opcode that can be turned into an absolute opcode,
      * using the relocation tables.
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_segrel2absolute(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_zeropage method is used to examine the opcode
      * argument list, determine whether or not is looks like a Zero
      * Page opcode, typically written
      *
      *     Mnemonic address
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_zeropage(const expression_list &args, unsigned char opc);

    /**
      * The mode_is_absolute_indirect method is used to examine the
      * opcode argument list, determine whether or not is looks like an
      * Absolute Indirect opcode, typically written
      *
      *     Mnemonic (address)
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_absolute_indirect(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_zeropage_indirect method is used to examine the
      * opcode argument list, determine whether or not is looks like a
      * Zero Page Indirect opcode, typically written
      *
      *     Mnemonic (address)
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_zeropage_indirect(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_absolute_indexed_x method is used to examine the
      * opcode argument list, determine whether or not is looks like an
      * Absolute Indexed with X opcode, typically written
      *
      *     Mnemonic address, X
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_absolute_indexed_x(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_zeropage_indexed_x method is used to examine the
      * opcode argument list, determine whether or not is looks like an
      * Zero Page Indexed with X opcode, typically written
      *
      *     Mnemonic address, X           ; where address < 0x100
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_zeropage_indexed_x(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_absolute_indexed_y method is used to examine the
      * opcode argument list, determine whether or not is looks like an
      * Absolute Indexed with Y opcode, typically written
      *
      *     Mnemonic address, Y
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_absolute_indexed_y(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_zeropage_indexed_y method is used to examine the
      * opcode argument list, determine whether or not is looks like an
      * Zero Page Indexed with Y opcode, typically written
      *
      *     Mnemonic address, Y           ; where address < 0x100
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_zeropage_indexed_y(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_absolute_indexed_indirect_x method is used to examine the
      * opcode argument list, determine whether or not is looks like an
      * Absolute Indexed Indirect opcode, typically written
      *
      *     Mnemonic (address, X)
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_absolute_indexed_indirect_x(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_zeropage_indexed_indirect_x method is used to examine the
      * opcode argument list, determine whether or not is looks like an
      * Zero Page Indexed Indirect opcode, typically written
      *
      *     Mnemonic (address, X)         ; where address < 0x100
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_zeropage_indexed_indirect_x(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_zeropage_indrect_indexed_y method is used to examine
      * the opcode argument list, determine whether or not is looks like
      * an Zero Page Indirect Indexed opcode, typically written
      *
      *     Mnemonic (address, Y)         ; where address < 0x100
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_zeropage_indirect_indexed_y(const expression_list &args,
        unsigned char opc);

    /**
      * The mode_is_accumulator method is used to examine the opcode
      * argument list, determine whether or not is looks like an
      * Accumulator opcode, typically written
      *
      *     Mnemonic A
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_accumulator(const expression_list &args, unsigned char opc);

    /**
      * The mode_is_relative method is used to examine the opcode
      * argument list, determine whether or not is looks like a
      * Relative opcode, typically written
      *
      *     Mnemonic address
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_relative(const expression_list &args, unsigned char opc);

    /**
      * The mode_is_implicit method is used to examine the opcode
      * argument list, determine whether or not is looks like an
      * Implicit opcode.
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_implicit(const expression_list &args, unsigned char opc);

    /**
      * The mode_is_normal8 method is used to test for the normal eight
      * addressing modes used by most opcodes, and ORing in the mode
      * bits into the supplied opcode.
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_normal8(const expression_list &args, unsigned char opc);

    /**
      * The mode_is_normal5a method is used to test for the normal five
      * addressing modes used by arithmetic opcodes, and ORing in the
      * mode bits into the supplied opcode.
      *
      * @param args
      *     The opcode's arguments
      * @param opc
      *     The byte to emit if the addressing mode is satisfied
      * @returns
      *     true of the arguments matched and the codee was emitted, or
      *     false if the argument didn't match (no bytes emitted).
      */
    bool mode_is_normal5a(const expression_list &args, unsigned char opc);

    /**
      * The argument_list_error method may be called to produce an error
      * message (when none of th above method find a match) that the
      * argument list is broken.
      *
      * @param args
      *     The opcode's arguments
      */
    void argument_list_error(const expression_list &args) const;

    /**
      * The model_warning
      *
      * @param first_present_in
      *     The model in which the opcode first (officially) appeared.
      * @returns
      *     true, always.
      *
      * @par
      * This method is intended to be used as follows
      * @code
      * if
      * (
      *    !(model_is_zero_page_indirect(0x72) && warning(chip_model_65c02))
      * &&
      *    !mode_is_normal8(args, 0x65)
      * )
      *    argument_list_error(args);
      * @endcode
      * @par
      * to issue a warning about opcodes when they may not be
      * present, but not alter the semantic meaning of the
      * condition.
      */
    bool model_warning(model_t first_present_in) const;

private:
    /**
      * The model of 6502 chip we are targeting.
      */
    model_t model;

    /**
      * The default constructor.  Do not use.
      */
    opcode_6502();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    opcode_6502(const opcode_6502 &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    opcode_6502 &operator=(const opcode_6502 &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_OPCODE_6502_H
// vim: set ts=8 sw=4 et :
