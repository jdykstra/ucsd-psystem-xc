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

#ifndef LIB_EXPRESSION_SRS_H
#define LIB_EXPRESSION_SRS_H

#include <lib/expression.h>

/**
  * The expression_srs class is used to represent an abstract expression
  * to produce a subrange set.
  */
class expression_srs:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_srs();

    /**
      * The constructor.
      *
      * @param lhs
      *     the left hand side of this expression.
      * @param rhs
      *     the right hand side of this expression.
      */
    expression_srs(const expression::pointer &lhs,
        const expression::pointer &rhs);

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
    rcstring lisp_representation(void) const;

    /**
      * The pre_order_traversal method is called by the traversal method
      * immediately before the left hand side is traversed.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The infix_order_traversal method is called by the traversal
      * method between traversing the left hand side and right hand side
      * expressions.
      */
    virtual void infix_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is called by the traversal method
      * immediately after the right hand side is traversed.
      */
    virtual void post_order_traversal(int pprec) const;

    /**
      * The get_lhs method may be used to derived classes to obtain
      * the left hand expression.
      */
    expression::pointer get_lhs() const { return lhs; }

    /**
      * The get_rhs method may be used to derived classes to obtain
      * the right hand expression.
      */
    expression::pointer get_rhs() const { return rhs; }

private:
    /**
      * The lhs instance variable is used to remember the left hand side
      * of this expression.
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the right hand
      * side of this expression.
      */
    expression::pointer rhs;

    /**
      * The tp instance variable is used to remember the type of
      * evaluating this expression node.
      */
    type::pointer tp;

    /**
      * The default constructor.  Do not use.
      */
    expression_srs();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_srs(const expression_srs &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_srs &operator=(const expression_srs &);
};

#endif // LIB_EXPRESSION_SRS_H
// vim: set ts=8 sw=4 et :
