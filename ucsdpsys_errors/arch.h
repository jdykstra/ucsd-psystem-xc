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

#ifndef UCSDPSYS_ERRORS_ARCH_H
#define UCSDPSYS_ERRORS_ARCH_H

#include <boost/shared_ptr.hpp>

#include <lib/mtype.h>
#include <lib/rcstring.h>

/**
  * The arch class is used to represent an abstract interface to the
  * encoding and decoding process, in case the different assemblers use
  * slightly different layouts for the error file.
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
      * The factory_by_arch class method is used to create a new encocder, based
      * on the architecture type given.
      *
      * @param arch_name
      *     The name if the architecture, e.g. "pdp11", "z80", "6502", etc.
      * @returns
      *     pointer to new instance in dynamic memory
      */
    static pointer factory_by_arch(const rcstring &arch_name);

    /**
      * The factory_by_host class method is used to create a new encocder, based
      * on the host type given.
      *
      * @param host_name
      *     The name if the architecture, e.g. "terak", "trs80", "apple", etc.
      * @returns
      *     pointer to new instance in dynamic memory
      */
    static pointer factory_by_host(const rcstring &host_name);

    static void list(void);

    /**
      * The encode method is used to read a text file representation of
      * an error file, and write a binary representation of the error
      * file.
      *
      * @param txt_file
      *     The text file to be read and encoded.
      * @param bin_file
      *     The binary file to be written.
      */
    void encode(const rcstring &txt_file, const rcstring &bin_file);

    /**
      * The decode method is used to read a binary error file,
      * and write a text representation of the error file.
      *
      * @param bin_file
      *     The binary file to be read and decoded.
      * @param txt_file
      *     The text file to be written.
      */
    void decode(const rcstring &bin_file, const rcstring &txt_file);

    void no_function_specified(const rcstring &fn1, const rcstring &fn2);

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param mtype
      *     The machine architecture of the assembler target.
      */
    arch(mtype_t mtype);

    virtual void read_binary(const rcstring &filename) = 0;

    virtual void write_binary(const rcstring &filename) const = 0;

    virtual void read_text(const rcstring &filename) = 0;

    virtual void write_text(const rcstring &filename) const = 0;

private:
    /**
      * The machine architecture of the host system.
      */
    mtype_t mtype;

    /**
      * The factory_by_mtype class method is used to create a new
      * encocder, based on the machine type given.
      *
      * @param mtype
      *     The machine type of interest.
      * @returns
      *     pointer to new instance in dynamic memory
      */
    static pointer factory_by_mtype(mtype_t mtype);

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

#endif // UCSDPSYS_ERRORS_ARCH_H
// vim: set ts=8 sw=4 et :
