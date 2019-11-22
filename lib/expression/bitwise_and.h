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

#ifndef LIB_EXPRESSION_BITWISE_AND_H
#define LIB_EXPRESSION_BITWISE_AND_H

#include <lib/expression.h>

/**
  * The expression_bitwise_and class is used to represent
  * a bit-wise AND expression.
  */
class expression_bitwise_and:
    public expression
{
public:
    typedef boost::shared_ptr<expression_bitwise_and> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_bitwise_and();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param lhs
      *     The left hand side of the expression.
      * @param rhs
      *     The right hand side of the expression.
      */
    expression_bitwise_and(const expression::pointer &lhs,
        const expression::pointer &rhs);

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    void traversal(int pprec) const;

    /**
      * The pre_order_traversal method is used to perform actions
      * required before the operand expressions are traversed.
      *
      * @param pprec
      *     The parent expressions precedence.  Not much use to code
      *     generators, but essential for pretty printers to get the
      *     parentheses correct.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The infix_order_traversal method is used to perform actions
      * required between traversing the operand expressions.
      *
      * @param pprec
      *     The parent expressions precedence.  Not much use to code
      *     generators, but essential for pretty printers to get the
      *     parentheses correct.
      */
    virtual void infix_order_traversal(int pprec) const;

    /**
      * The pre_order_traversal method is used to perform actions
      * required after the operand expressions are traversed.
      *
      * @param pprec
      *     The parent expressions precedence.  Not much use to code
      *     generators, but essential for pretty printers to get the
      *     parentheses correct.
      */
    virtual void post_order_traversal(int pprec) const;

    expression::pointer get_lhs(void) const { return lhs; }
    expression::pointer get_rhs(void) const { return rhs; }

private:
    /**
      * The lhs instance variable is used to remember the left hand side
      * of the expression.
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the right hand
      * side of the expression.
      */
    expression::pointer rhs;

    /**
      * The default constructor.  Do not use.
      */
    expression_bitwise_and();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_bitwise_and(const expression_bitwise_and &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_bitwise_and &operator=(const expression_bitwise_and &rhs);
};

#endif // LIB_EXPRESSION_BITWISE_AND_H
// vim: set ts=8 sw=4 et :
