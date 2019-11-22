//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#ifndef LIB_EXPRESSION_NEGATE_H
#define LIB_EXPRESSION_NEGATE_H

#include <lib/expression.h>

/**
  * The expression_negate class is used to represent an abstract
  * numerical negative expression.
  */
class expression_negate:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_negate();

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
      *
      * @param operand
      *     The expression to be negated.
      */
    expression_negate(const expression::pointer &operand);

    /**
      * The get_operand method is used to obtain the expression being
      * negated.
      */
    expression::pointer get_operand(void) const { return operand; }

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_constant(void) const;

protected:
    /**
      * The pre_order_traversal method is called before the operand is
      * traversed, by the traversal method.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is called after the operand is
      * traversed, by the traversal method.
      */
    virtual void post_order_traversal(int pprec) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

private:
    /**
      * The operand instance variable is used to remember
      * the expression to be negated.
      */
    expression::pointer operand;

    /**
      * The ptype instance variable is used to remember the type returned
      * by evaluating this expression node.
      */
    type::pointer ptype;

    /**
      * The default constructor.  Do not use.
      */
    expression_negate();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_negate(const expression_negate &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_negate &operator=(const expression_negate &);
};

#endif // LIB_EXPRESSION_NEGATE_H
// vim: set ts=8 sw=4 et :
