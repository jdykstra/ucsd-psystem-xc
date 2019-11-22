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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_6502_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_6502_H

#include <lib/expression.h>

/**
  * The expression_6502 class is used to represent an abstract
  * expression of a 6502 addressing mode.  Such expressions are never
  * traversed, they exist ssoley to be dismantled by the opcodes
  * when they are picing out the relevant expression trees for code
  * generation.
  */
class expression_6502:
    public expression
{
public:
    typedef boost::shared_ptr<expression_6502> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_6502();

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
      */
    expression_6502(const location &locn);

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

private:
    /**
      * The default constructor.  Do not use.
      */
    expression_6502();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_6502(const expression_6502 &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_6502 &operator=(const expression_6502 &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_6502_H
// vim: set ts=8 sw=4 et :
