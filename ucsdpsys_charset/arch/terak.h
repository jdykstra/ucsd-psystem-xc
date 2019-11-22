//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#ifndef UCSDPSYS_CHARSET_ARCH_TERAK_H
#define UCSDPSYS_CHARSET_ARCH_TERAK_H

#include <ucsdpsys_charset/arch.h>

/**
  * The arch_terak class is used to represent the processing required to
  * build SYSTEM.CHARSET files for a Terak (PDP-11).
  */
class arch_terak:
    public arch
{
public:
    typedef boost::shared_ptr<arch_terak> pointer;

    /**
      * The destructor.
      */
    virtual ~arch_terak();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param mtype
      *     The machine type we are working within.
      */
    static pointer create(mtype_t mtype);

protected:
    // See base class for documentation.
    rcstring get_binary_format_name(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param mtype
      *     The machine type we are working within.
      */
    arch_terak(mtype_t mtype);

    /**
      * The default constructor.
      * Do not use.
      */
    arch_terak();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    arch_terak(const arch_terak &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    arch_terak &operator=(const arch_terak &rhs);
};

#endif // UCSDPSYS_CHARSET_ARCH_TERAK_H
// vim: set ts=8 sw=4 et :
