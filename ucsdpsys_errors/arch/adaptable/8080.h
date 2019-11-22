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

#ifndef UCSDPSYS_ERRORS_ARCH_ADAPTABLE_8080_H
#define UCSDPSYS_ERRORS_ARCH_ADAPTABLE_8080_H

#include <ucsdpsys_errors/arch/adaptable.h>

/**
  * The arch_adaptable_8080 class is used to represent the 8080 form of
  * the adaptable assembler's error data file.
  */
class arch_adaptable_8080:
    public arch_adaptable
{
public:
    typedef boost::shared_ptr<arch_adaptable_8080> pointer;

    /**
      * The destructor.
      */
    virtual ~arch_adaptable_8080();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static arch::pointer create(void);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    arch_adaptable_8080();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    arch_adaptable_8080(const arch_adaptable_8080 &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    arch_adaptable_8080 &operator=(const arch_adaptable_8080 &rhs);
};

#endif // UCSDPSYS_ERRORS_ARCH_ADAPTABLE_8080_H
// vim: set ts=8 sw=4 et :
