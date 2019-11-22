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

#ifndef UCSDPSYS_ERRORS_ARCH_ADAPTABLE_H
#define UCSDPSYS_ERRORS_ARCH_ADAPTABLE_H

#include <lib/ac/vector>

#include <ucsdpsys_errors/arch.h>

/**
  * The arch_adaptable class is used to represent the error file format
  * of the UCSD Adaptive Assembler.
  */
class arch_adaptable:
    public arch
{
public:
    typedef boost::shared_ptr<arch_adaptable> pointer;

    /**
      * The destructor.
      */
    virtual ~arch_adaptable();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param mtype
      *     The machine architecture of the assembler target.
      */
    arch_adaptable(mtype_t mtype);

    // See base class for documentation.
    void read_binary(const rcstring &filename);

    // See base class for documentation.
    void write_binary(const rcstring &filename) const;

    // See base class for documentation.
    void read_text(const rcstring &filename);

    // See base class for documentation.
    void write_text(const rcstring &filename) const;

    virtual unsigned get_string_length(void) const;

private:
    typedef std::vector<rcstring> data_t;

    /**
      * The data instance variable is used to remember the individual
      * error messages.
      */
    data_t data;

    /**
      * The default constructor.  Do not use.
      */
    arch_adaptable();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    arch_adaptable(const arch_adaptable &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    arch_adaptable &operator=(const arch_adaptable &rhs);
};

#endif // UCSDPSYS_ERRORS_ARCH_ADAPTABLE_H
// vim: set ts=8 sw=4 et :
