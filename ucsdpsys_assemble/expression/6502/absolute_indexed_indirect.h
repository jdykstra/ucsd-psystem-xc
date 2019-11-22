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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_6502_ABSOLUTE_INDEXED_INDIRECT_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_6502_ABSOLUTE_INDEXED_INDIRECT_H

#include <ucsdpsys_assemble/expression/6502.h>

/**
  * The expression_6502_absolute_indexed_indirect class is used to
  * represent the absolute (or zero page) indexed indirect addressing
  * mode of the 6502.
  */
class expression_6502_absolute_indexed_indirect:
    public expression_6502
{
public:
    typedef boost::shared_ptr<expression_6502_absolute_indexed_indirect>
        pointer;

    /**
      * The destructor.
      */
    virtual ~expression_6502_absolute_indexed_indirect();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param address
      *     The absolute address to access.
      * @param special
      *     The register to index by.
      */
    static pointer create(const expression::pointer &address,
        const expression::pointer &special);

    /**
      * The get_address method may be used to obtain
      * the absolute address to access.
      */
    expression::pointer get_address(void) const { return address; }

    /**
      * The get_special method may be used to obtain
      * the register to index by.
      */
    expression::pointer get_special(void) const { return special; }

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param address
      *     The absolute address to access
      * @param special
      *     The register to index by.
      */
    expression_6502_absolute_indexed_indirect(
        const expression::pointer &address, const expression::pointer &special);

    /**
      * The fubar instance variable is used to remember the absolute
      * address to access.
      */
    expression::pointer address;

    /**
      * The fubar instance variable is used to remember the register to
      * index by.
      */
    expression::pointer special;

    /**
      * The default constructor.  Do not use.
      */
    expression_6502_absolute_indexed_indirect();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_6502_absolute_indexed_indirect(
        const expression_6502_absolute_indexed_indirect &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_6502_absolute_indexed_indirect &operator=(
        const expression_6502_absolute_indexed_indirect &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_6502_ABSOLUTE_INDEXED_INDIRECT_H
// vim: set ts=8 sw=4 et :
