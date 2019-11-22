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

#ifndef LIB_EXPRESSION_BITWISE_NOT_H
#define LIB_EXPRESSION_BITWISE_NOT_H

#include <lib/expression.h>

/**
  * The expression_bitwise_not class is used to represent a bit-wise not
  * expression.
  */
class expression_bitwise_not:
    public expression
{
public:
    typedef boost::shared_ptr<expression_bitwise_not> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_bitwise_not();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param operand
      *     The operand to the expression.
      */
    expression_bitwise_not(const expression::pointer &operand);

    // See base for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base for documentation.
    int get_precedence(void) const;

    // See base for documentation.
    type::pointer get_type(void) const;

    // See base for documentation.
    bool is_lvalue(void) const;

    // See base for documentation.
    side_effect_t has_side_effect(void) const;

    // See base for documentation.
    rcstring lisp_representation(void) const;

    // See base for documentation.
    void traversal(int pprec) const;

    /**
      * The pre_order_traversal method is used to perform actions
      * required before the operand expression is traversed.
      *
      * @param pprec
      *     The parent expressions precedence.  Not much use to code
      *     generators, but essential for pretty printers to get the
      *     parentheses correct.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is used to perform actions
      * required after the operand expression has been traversed.
      *
      * @param pprec
      *     The parent expressions precedence.  Not much use to code
      *     generators, but essential for pretty printers to get the
      *     parentheses correct.
      */
    virtual void post_order_traversal(int pprec) const;

    /**
      * The get_operand method may be used to obtain the operand of this
      * expression.  For derived classes only.
      */
    expression::pointer get_operand(void) const { return operand; }

private:
    /**
      * The operand instance variable is used to remember the operand to
      * the expression.
      */
    expression::pointer operand;

    /**
      * The default constructor.  Do not use.
      */
    expression_bitwise_not();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_bitwise_not(const expression_bitwise_not &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_bitwise_not &operator=(const expression_bitwise_not &rhs);
};

#endif // LIB_EXPRESSION_BITWISE_NOT_H
// vim: set ts=8 sw=4 et :
