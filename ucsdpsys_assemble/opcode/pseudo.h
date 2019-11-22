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

#ifndef UCSDPSYS_ASSEMBLE_OPCODE_PSEUDO_H
#define UCSDPSYS_ASSEMBLE_OPCODE_PSEUDO_H

#include <ucsdpsys_assemble/opcode.h>

/**
  * The opcode_pseudo class is used to represent
  * a generic machine-neutral pseudo-op.
  */
class opcode_pseudo:
    public opcode
{
public:
    typedef boost::shared_ptr<opcode_pseudo> pointer;

    /**
      * The destructor.
      */
    virtual ~opcode_pseudo();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    opcode_pseudo(assembler *context);

private:
    /**
      * The default constructor.  Do not use.
      */
    opcode_pseudo();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    opcode_pseudo(const opcode_pseudo &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    opcode_pseudo &operator=(const opcode_pseudo &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_OPCODE_PSEUDO_H
// vim: set ts=8 sw=4 et :
