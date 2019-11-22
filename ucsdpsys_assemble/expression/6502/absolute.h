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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_6502_ABSOLUTE_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_6502_ABSOLUTE_H

#include <ucsdpsys_assemble/expression/6502.h>

/**
  * The expression_6502_absolute class is used to represent
  * the 6502 absolute addressing mode.
  */
class expression_6502_absolute:
    public expression_6502
{
public:
    typedef boost::shared_ptr<expression_6502_absolute> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_6502_absolute();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param address
      *     The address to be accessed.
      */
    static pointer create(const expression::pointer &address);

    /**
      * The get_address method may be used to obtain the address to be
      * accessed.
      */
    expression::pointer get_address(void) const { return address; }

    // See base class for documentation.
    rcstring lisp_representation(void) const;

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param address
      *     The address to be accessed.
      */
    expression_6502_absolute(const expression::pointer &address);

private:
    /**
      * The address instance variable is used to remember the address to
      * be accessed.
      */
    expression::pointer address;

    /**
      * The default constructor.  Do not use.
      */
    expression_6502_absolute();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_6502_absolute(const expression_6502_absolute &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_6502_absolute &operator=(const expression_6502_absolute &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_6502_ABSOLUTE_H
// vim: set ts=8 sw=4 et :
