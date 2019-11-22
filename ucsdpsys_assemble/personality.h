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

#ifndef UCSDPSYS_ASSEMBLE_PERSONALITY_H
#define UCSDPSYS_ASSEMBLE_PERSONALITY_H

#include <lib/byte_sex.h>
#include <lib/expression.h>
#include <lib/mtype.h>
#include <lib/symtab/template.h>

#include <ucsdpsys_assemble/expression/special.h>
#include <ucsdpsys_assemble/opcode.h>

class assembler; // forward

/**
  * The personality class is used to represent a collection of opcodes
  * that are the "personality" of a particular machine (e.g. p-machine,
  * PDP-11, Z80, 6502, etc).
  */
class personality
{
public:
    typedef boost::shared_ptr<personality> pointer;

    /**
      * The destructor.
      */
    virtual ~personality();

    /**
      * The lookup method is used to look-up a name in the opcode
      * symbol table, to see if the name given is the name of an
      * opcode.
      *
      * @param name
      *     The name of the opcode, to be tested.
      * @returns
      *     Pointer to an opcode if successful, or NULL if not an opcode.
      */
    opcode::pointer lookup_opcode(const rcstring &name) const;

    /**
      * The lookup_special method is used to look-up a name in the "special"
      * symbol table.  This is typically used for machine registers.
      *
      * @param name
      *     The name of the special symbol to be tested.
      * @returns
      *     Pointer to an expression if successful, or NULL if not a
      *     special symbol.
      */
    expression::pointer lookup_special(const rcstring &name) const;

    /**
      * The get_byte_sex method may be used to determine the byte sex of
      * the macine architecture this personality describes.
      */
    virtual byte_sex_t get_byte_sex(void) const;

    /**
      * The factory_by_arch method is used to create new personality
      * instances, based on the name of the personality.
      *
      * @param arch_name
      *     The name of the architecture, e.g. "6502"
      * @param context
      *     The assembly context we are working within.
      * @returns
      *     a pointer to a new instance in dynamic memory, or NULL if
      *     the name is unknown (it will have already printed the error).
      */
    static pointer factory_by_arch(const rcstring &arch_name,
        assembler *context);

    /**
      * The factory_by_host method is used to create new personality
      * instances, based on the name of the host.
      *
      * @param host_name
      *     The name of the host, e.g. "apple"
      * @param context
      *     The assembly context we are working within.
      * @returns
      *     a pointer to a new instance in dynamic memory, or NULL if
      *     the name is unknown (it will have already printed the error).
      */
    static pointer factory_by_host(const rcstring &host_name,
        assembler *context);

    /**
      * The factory_by_mtype method is used to create new personality
      * instances, based on the machine type.
      *
      * @param mtype
      *     The machine type.
      * @param context
      *     The assembly context we are working within.
      * @returns
      *     a pointer to a new instance in dynamic memory, or NULL if
      *     the name is unknown (it will have already printed the error).
      */
    static pointer factory_by_mtype(mtype_t mtype, assembler *context);

    /**
      * The get_mtype method is used to determine the machine type for
      * the code being assembled.
      */
    virtual mtype_t get_mtype(void) const = 0;

    /**
      * The get_default_radix method is used to obtain the default lexer
      * number radix preferred by this personality.  For historical
      * reasons, the default implementation returns 16.
      */
    virtual int get_default_radix(void) const;

    /**
      * The get_one_byte_nop method is used to obtain the byte value of
      * a one byte NOP opcode, to be used as filler.
      */
    virtual unsigned char get_one_byte_nop(void) const = 0;

    /**
      * The register_opcode method is used to add another opcode to
      * the personality.  Usually this is done in the derived class
      * constructor.
      *
      * @param op
      *     The opcode to register.
      *     The name will be taken from here, too.
      */
    void register_opcode(const opcode::pointer &op);

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    personality();

    /**
      * The copy constructor.
      * For use by derived classes only.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    personality(const personality &rhs);

    /**
      * The register_opcode method is used to add another opcode to
      * the personality.  Usually this is done in the derived class
      * constructor.
      *
      * @param name
      *     The name to register the opcode as.
      * @param op
      *     The opcode to register.
      */
    void register_opcode(const rcstring &name, const opcode::pointer &op);

    /**
      * The register_special method is used to add another special
      * symbol to the personality.  Usually this is done in the derived
      * class constructor.
      *
      * @param op
      *     The opcode to register.
      *     The name will be taken from here, too.
      */
    void register_special(const expression_special::pointer &op);

private:
    /**
      * The factory_by_mtype method is used to create new personality
      * instances, based on the machine type.
      *
      * @param mtype
      *     The machine type of interest.
      * @param context
      *     The assembly context we are working within.
      * @returns
      *     a pointer to a new instance in dynamic memory, or NULL if
      *     the name is unknown (it will have already printed the error).
      */
    static pointer factory_by_host(mtype_t mtype, assembler *context);

    typedef symtab<opcode::pointer> opcodes_t;

    /**
      * The opcodes instance variable is used to remember the mapping
      * from name to opcode.
      */
    opcodes_t opcodes;

    typedef symtab<expression::pointer> specials_t;

    /**
      * The special instance variable is used to remember the mapping
      * from name to special symbol.
      */
    specials_t specials;

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    personality &operator=(const personality &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_PERSONALITY_H
// vim: set ts=8 sw=4 et :
