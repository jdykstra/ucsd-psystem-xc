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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_BYTE_POINTER_H
#define UCSDPSYS_COMPILE_EXPRESSION_BYTE_POINTER_H

#include <lib/expression.h>

/**
  * The expression_byte_pointer class is used to represent
  * the constructor for a byte pointer.
  *
  * A byte pointer is a two-word value on the stack.  The first is a
  * word pointer to the base of an array of bytes, the second is an
  * index into the byte array (it could be a packed array of char, it
  * could be a string, etc).
  */
class expression_byte_pointer:
    public expression
{
public:
    typedef boost::shared_ptr<expression_byte_pointer> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_byte_pointer();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The left hand side (the word pointer).
      * @param rhs
      *     The right hand side (the array index).
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
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

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param lhs
      *     The left hand side (the word pointer).
      * @param rhs
      *     The right hand side (the array index).
      */
    expression_byte_pointer(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The lhs instance variable is used to remember the left hand side
      * (the word pointer).
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the right hand
      * side (the array index).
      */
    expression::pointer rhs;

    /**
      * The default constructor.  Do not use.
      */
    expression_byte_pointer();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_byte_pointer(const expression_byte_pointer &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_byte_pointer &operator=(const expression_byte_pointer &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_BYTE_POINTER_H
// vim: set ts=8 sw=4 et :
