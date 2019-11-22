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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_NAME_ASM_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_NAME_ASM_H

#include <lib/expression/name.h>

/**
  * The expression_name_asm class is used to represent a name (possibly
  * a forward reference) see in assembler source code.
  */
class expression_name_asm:
    public expression_name
{
public:
    typedef boost::shared_ptr<expression_name_asm> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_name_asm();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The location of this symbol reference.
      * @param sp
      *     The symbol being referenced.
      */
    static pointer create(const location &locn, const symbol::pointer &sp);

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param locn
      *     The location of this symbol reference.
      * @param sp
      *     The symbol being referenced.
      */
    expression_name_asm(const location &locn, const symbol::pointer &sp);

    /**
      * The default constructor.  Do not use.
      */
    expression_name_asm();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_name_asm(const expression_name_asm &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_name_asm &operator=(const expression_name_asm &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_NAME_ASM_H
// vim: set ts=8 sw=4 et :
