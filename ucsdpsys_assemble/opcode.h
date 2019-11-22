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

#ifndef UCSDPSYS_ASSEMBLE_OPCODE_H
#define UCSDPSYS_ASSEMBLE_OPCODE_H

#include <boost/shared_ptr.hpp>

#include <lib/expression.h>

class assembler; // forward
class expression_list; // forward

/**
  * The opcode class is used to represent an abstract opcode that the
  * assembler can pass parameters to.
  */
class opcode
{
public:
    typedef boost::shared_ptr<opcode> pointer;

    /**
      * The destructor.
      */
    virtual ~opcode();

    /**
      * The interpret method is used to perform the operation of the
      * opcode.  It could be a real opcode, and it could be a pseudo-op.
      *
      * @param args
      *     The argument list.
      */
    virtual void interpret(const expression_list &args) = 0;

    /**
      * The get_name method is sued to obtain the name of this opcode.
      */
    virtual rcstring get_name(void) const = 0;

    enum kind_t
    {
        /**
          * Opcode of this kind have sensable argumenbts that use a
          * simple C grammar for building their operand values (sure
          * would be nice if they all could).
          */
        kind_standard,

        /**
          * The 6502 opcode that need special grammar support.
          * Fortunately, some can function without change using the
          * standard grammar.
          */
        kind_6502,

        /**
          * A subset of kind_standard that may occure before the first
          * .proc pseudo-op.
          */
        kind_prelude,

        /**
          * A subset of kind_prelude that takes a list of names as arguments.
          */
        kind_name_list,
    };

    /**
      * The get_kind method is used to determine what kind of opcode
      * this is.  It is used to select different specialist productions
      * in the grammar, if the manufacturers came up with an especially
      * hinky way of formatting CPU opcode arguments.
      */
    virtual kind_t get_kind(void) const;

    /**
      * The next_argument_always_a_string method is used to determien
      * whether the next argument to the (peudo)opcode is always a string.
      * The intended use is the .include pseudo-op.
      */
    virtual bool next_argument_always_a_string(void) const;

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param context
      *     The assembler context we are working within.
      */
    opcode(assembler *context);

    /**
      * The emit_byte method is used to output a byte of code at the
      * current location, and advance the current location.
      *
      * @param value
      *     The value of the byte to be emitted.
      */
    void emit_byte(unsigned char value);

    /**
      * The emit_byte method is used to output a byte of code at the
      * current location, and advance the current location.  It may need
      * to be deferred until later.
      *
      * @param value
      *     The value to place into the code stream.
      */
    void emit_byte(const expression::pointer &value);

    /**
      * The emit_word method is used to output a two-byte value at the
      * current location, and advance the current location.
      *
      * @param value
      *     The value of the word to be emitted.
      */
    void emit_word(unsigned short value);

    /**
      * The emit_word method is used to output a two-byte value at the
      * current location, and advance the current location.
      *
      * @param value
      *     The value of the word to be emitted.
      */
    void emit_word(const expression::pointer &value);

    /**
      * The emit_big method is used to output a one-or-two-byte value at the
      * current location, and advance the current location.
      *
      * @param value
      *     The value of the "big" to be emitted (128..32767).
      */
    void emit_big(unsigned short value);

    /**
      * The emit_big method is used to output a one-or-two-byte value at the
      * current location, and advance the current location.
      *
      * @param value
      *     The value of the "big" to be emitted.
      */
    void emit_big(const expression::pointer &value);

    void emit_even_alignment(void);

    /**
      * The get_address method is used to obtain the current code offset
      * in the current segment.
      */
    unsigned get_address(void) const;

    /**
      * The check_argument_count method is used to examine an argument
      * list, and check that it has the correct number of arguments.
      *
      * @param args
      *     The arguments to be checked.
      * @param expect
      *     The number of arguments expected.
      * @returns
      *     True if the argument count is as expected, or false (after
      *     printing an error message) if the argument list is not as
      *     exprected.
      */
    bool check_argument_count(const expression_list &args, unsigned expect)
        const;

    bool check_argument_const_int(int argnum, const expression::pointer &arg,
        long lo, long hi) const;

    void dot_architecture(const rcstring &name);

    void dot_host(const rcstring &name);

    assembler *get_context(void) const { return context; }

    static rcstring trim_quotes(const rcstring &text);

    /**
      * The reloc_type1_lcrel method is used to add another address to
      * the Type 1 (address relative) relocation word data.
      * Passed to the segment builder #context.
      *
      * @param address
      *     The addres sof the word within the segment.
      */
    void reloc_type1_lcrel(unsigned address);

    /**
      * The reloc_type1_lcrel method is used to add the current address
      * to the Type 1 (address relative) relocation word data.
      * Passed to the segment builder #context.
      */
    void reloc_type1_lcrel(void);

    /**
      * The reloc_type1_lcrel method is used to add another address to
      * the Type 2 (.ref, .const) relocation word data.
      * Passed to the segment builder #context.
      *
      * @param address
      *     The addres sof the word within the segment.
      */
    void reloc_type2_ref(unsigned address);

    /**
      * The reloc_type1_lcrel method is used to add another address to
      * the Type 3 (.public, .private) relocation word data.
      * Passed to the segment builder #context.
      *
      * @param address
      *     The addres sof the word within the segment.
      */
    void reloc_type3_pubpriv(unsigned address);

private:
    /**
      * The context instance variable is used to remember the assembler
      * context we are working within.
      */
    assembler *context;

    /**
      * The default constructor.  Do not use.
      */
    opcode();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    opcode(const opcode &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    opcode &operator=(const opcode &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_OPCODE_H
// vim: set ts=8 sw=4 et :
