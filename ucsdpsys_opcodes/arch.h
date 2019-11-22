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

#ifndef UCSDPSYS_OPCODES_ARCH_H
#define UCSDPSYS_OPCODES_ARCH_H

#include <boost/shared_ptr.hpp>

#include <lib/byte_sex.h>
#include <lib/mtype.h>
#include <lib/rcstring.h>

/**
  * The arch class is used to represent an abstract interface to
  * architecture opcode information.
  */
class arch
{
public:
    typedef boost::shared_ptr<arch> pointer;

    /**
      * The destructor.
      */
    virtual ~arch();

    /**
      * The factory_by_arch method is used to construct a new opcode
      * architecture instance, based on the name of the architecture.
      *
      * @param arch_name
      *     The name of the architecture, e.g. "pdp11", "z80", "6502"
      * @returns
      *     new opcode arch instance in dynamic memory.
      */
    static pointer factory_by_arch(const rcstring &arch_name);

    /**
      * The factory_by_host method is used to construct a new opcode
      * architecture instance, based on the name of the host.
      *
      * @param host_name
      *     The name of the host, e.g. "terak", "trs80", "apple"
      * @returns
      *     new opcode arch instance in dynamic memory.
      */
    static pointer factory_by_host(const rcstring &host_name);

    /**
      * The list class method is used to print a list of the known
      * architectures on the standard output.
      */
    static void list(void);

    /**
      * The encode function is used to read a text opcodes file and
      * generate a binary equivalent - because text is much easier to edit.
      *
      * @param infile
      *     The text file to be read.  The string "-" means the standard
      *     input is to be used.
      * @param outfile
      *     The binary file to be written.  The string "-" means the standard
      *     output is to be used.
      */
    void encode(const rcstring &infile, const rcstring &outfile);

    /**
      * The decode function is used to read a binary opcodes file and
      * generate a text equivalent - because text is much easier to edit.
      *
      * @param infile
      *     The binary file to be read.  The string "-" means the standard
      *     input is to be used.
      * @param outfile
      *     The text file to be written.  The string "-" means the standard
      *     output is to be used.
      */
    void decode(const rcstring &infile, const rcstring &outfile);

    void random_test_data(const rcstring &infile, const rcstring &outfile);

    void no_function_specified(const rcstring &infile, const rcstring &outfile);

protected:
    /**
      * The constructor.
      * For use of derived classes only.
      *
      * @param mtype
      *     The machine type of this architecture.
      */
    arch(mtype_t mtype);

    virtual void read_binary(const rcstring &filename) = 0;

    virtual void write_binary(const rcstring &filename) const = 0;

    virtual void read_text(const rcstring &filename) = 0;

    virtual void write_text(const rcstring &filename) const = 0;

    unsigned get_word(const unsigned char *data) const;

    void put_word(unsigned char *data, unsigned value) const;

    virtual void random_test_data(void) = 0;

    void flip_byte_sex(void);

    static rcstring rname(void);

private:
    /**
      * The factory_by_mtype method is used to construct a new opcode
      * architecture instance, based on the machine type.
      *
      * @param mtype
      *     The machine type.
      * @returns
      *     new opcode arch instance in dynamic memory.
      */
    static pointer factory_by_mtype(mtype_t mtype);

    /**
      * The default constructor.  Do not use.
      */
    arch();

    mtype_t mtype;
    byte_sex_t byte_sex;

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    arch(const arch &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    arch &operator=(const arch &rhs);
};

#endif // UCSDPSYS_OPCODES_ARCH_H
// vim: set ts=8 sw=4 et :
