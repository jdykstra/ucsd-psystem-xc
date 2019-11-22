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

#ifndef UCSDPSYS_OPCODES_ARCH_PCODE_H
#define UCSDPSYS_OPCODES_ARCH_PCODE_H

#include <ucsdpsys_opcodes/arch.h>

/**
  * The arch_pcode class is used to represent the opcode file format for
  * the p-code disassembler.
  */
class arch_pcode:
    public arch
{
public:
    typedef boost::shared_ptr<arch_pcode> pointer;

    /**
      * The destructor.
      */
    virtual ~arch_pcode();

    /**
      * The create_le class method is used to create new dynamically
      * allocated instances of this class.
      */
    static arch::pointer create_le(void);

    /**
      * The create_be class method is used to create new dynamically
      * allocated instances of this class.
      */
    static arch::pointer create_be(void);

protected:
    // See base class for documentation.
    void read_binary(const rcstring &filename);

    // See base class for documentation.
    void write_binary(const rcstring &filename) const;

    // See base class for documentation.
    void read_text(const rcstring &filename);

    // See base class for documentation.
    void write_text(const rcstring &filename) const;

    // See base class for documentation.
    void random_test_data(void);

private:
    /**
      * The constructor.  It is private on purpose, use the #create_be
      * or #create_le class method instead.
      *
      * @param mtype
      *     The machine type, either mtype_pcode_be or mtype_pcode_le
      */
    arch_pcode(mtype_t mtype);

    rcstring opname[256];
    int optype[265];

    /**
      * The default constructor.  Do not use.
      */
    arch_pcode();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    arch_pcode(const arch_pcode &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    arch_pcode &operator=(const arch_pcode &rhs);
};

#endif // UCSDPSYS_OPCODES_ARCH_PCODE_H
// vim: set ts=8 sw=4 et :
