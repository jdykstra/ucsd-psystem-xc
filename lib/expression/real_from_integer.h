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

#ifndef LIB_EXPRESSION_REAL_FROM_INTEGER_H
#define LIB_EXPRESSION_REAL_FROM_INTEGER_H

#include <lib/expression.h>

/**
  * The expression_real_from_integer class is used to represent a
  * conversion of an integer value to a real value.
  */
class expression_real_from_integer:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_real_from_integer();

    /**
      * The constructor.
      */
    expression_real_from_integer(const expression::pointer &arg);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    bool is_constant() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

protected:
    /**
      * The pre_order_traversal method is called by the traversal method
      * before the operand expression is traversed.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is called by the traversal
      * method after the operand expression is traversed.
      */
    virtual void post_order_traversal(int pprec) const;

    /**
      * The get_operand method is used to obtain the operand
      * expression of this node.
      */
    expression::pointer get_operand(void) const { return operand; }

private:
    /**
      * The arg instance variable is used to remember the expression to
      * be cast.
      */
    expression::pointer operand;

    /**
      * The default constructor.  Do not use.
      */
    expression_real_from_integer();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_real_from_integer(const expression_real_from_integer &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_real_from_integer &operator=(
        const expression_real_from_integer &);
};

#endif // LIB_EXPRESSION_REAL_FROM_INTEGER_H
// vim: set ts=8 sw=4 et :
