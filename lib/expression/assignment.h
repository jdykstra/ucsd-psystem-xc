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

#ifndef LIB_EXPRESSION_ASSIGNMENT_H
#define LIB_EXPRESSION_ASSIGNMENT_H


#include <lib/expression.h>

/**
  * The expression_assignment class is used to represent an abstract
  * assignment expression node.
  */
class expression_assignment:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_assignment();

    /**
      * The default constructor.
      *
      * @param lhs
      *     the left hand side of the assignment.
      * @param rhs
      *     the right hand side of the assignment.
      */
    expression_assignment(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence() const;

    /**
      * The pre_order_traversal method is called by the traversal method
      * before the left hand expression is traversed.  The default
      * implementation does nothing.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The infix_order_traversal method is called by the traversal
      * method after the left hand expression is traversed, and
      * before the right hand expression is traversed.  The default
      * implementation does nothing.
      */
    virtual void infix_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is called by the traversal
      * method after the right hand expression is traversed.  The
      * default implementation does nothing.
      */
    virtual void post_order_traversal(int pprec) const;

    /**
      * The get_lhs method is used ot get the left hand operand of the
      * assignment node.
      */
    expression::pointer get_lhs() const { return lhs; }

    /**
      * The get_rhs method is used ot get the right hand operand of the
      * assignment node.
      */
    expression::pointer get_rhs() const { return rhs; }

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The lhs instance variable is used to remember the left hand side
      * of the assignment.
      */
    expression::pointer lhs;

    /**
      * The lhs instance variable is used to remember the right hand
      * side of the assignment.
      */
    expression::pointer rhs;

    /**
      * The default constructor.  Do not use.
      */
    expression_assignment();

protected:
    /**
      * The copy constructor.
      * Foir use by derived classes only.
      */
    expression_assignment(const expression_assignment &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_assignment &operator=(const expression_assignment &);
};

#endif // LIB_EXPRESSION_ASSIGNMENT_H
// vim: set ts=8 sw=4 et :
