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

#ifndef UCSDPSYS_ASSEMBLE_OPCODE_ERROR_H
#define UCSDPSYS_ASSEMBLE_OPCODE_ERROR_H

#include <ucsdpsys_assemble/opcode.h>

/**
  * The opcode_error class is used to represent a generic opcode lookup
  * failure.  Created as transient objects during the parse.
  */
class opcode_error:
    public opcode
{
public:
    typedef boost::shared_ptr<opcode_error> pointer;

    /**
      * The destructor.
      */
    virtual ~opcode_error();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param context
      *     The assembler context we are working within.
      * @param name
      *     The name of the non-existent opcode.
      */
    static pointer create(assembler *context, const rcstring &name);

protected:
    // See base class for documentation.
    void interpret(const expression_list &args);

    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    kind_t get_kind(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param context
      *     The assembler context we are working within.
      * @param name
      *     The name of the non-existent opcode.
      */
    opcode_error(assembler *context, const rcstring &name);

    /**
      * The name instance variable is used to remember the name of the
      * non-existent opcode.
      */
    rcstring name;

    /**
      * The default constructor.  Do not use.
      */
    opcode_error();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    opcode_error(const opcode_error &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    opcode_error &operator=(const opcode_error &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_OPCODE_ERROR_H
// vim: set ts=8 sw=4 et :
