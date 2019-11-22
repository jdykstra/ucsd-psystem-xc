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

#ifndef LIB_EXPRESSION_TERNARY_H
#define LIB_EXPRESSION_TERNARY_H

#include <lib/expression.h>

class translator; // forward

/**
  * The expression_ternary class is used to represent the ternary
  * (conditional) expression.  The is a very useful feature, borrowed
  * from C.  It makes some code generation easier, too.
  */
class expression_ternary:
    public expression
{
public:
    typedef boost::shared_ptr<expression_ternary> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_ternary();

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
      *
      * (cond ? lhs : rhs)
      *
      * @param cond
      *     The control expression.  It must be a boolean value.
      * @param lhs
      *     The value to use if cond is true.
      * @param rhs
      *     The value to use if cond is false.
      * @param xlat
      *     The translation context we are working within.
      */
    expression_ternary(const expression::pointer &cond,
        const expression::pointer &lhs, const expression::pointer &rhs,
        translator *xlat);

    /**
      * The get_cond method may be used to obtain the control
      * expression.
      */
    expression::pointer get_cond(void) const { return cond; }

    /**
      * The get_lhs method may be used to obtain the value to use if
      * cond is true.
      */
    expression::pointer get_lhs(void) const { return lhs; }

    /**
      * The get_rhs method may be used to obtain the value to use if
      * cond is false.
      */
    expression::pointer get_rhs(void) const { return rhs; }

    // See base class for documentation.
    void traversal(void) const;

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
    rcstring lisp_representation(void) const;

    /**
      * The pre_order_traversal method is called before the cond is
      * traversed, by the traversal method.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The quest_order_traversal method is called after the cond is
      * traversed, but before the lhs is traversed, by the traversal
      * method.
      */
    virtual void quest_order_traversal(int pprec) const;

    /**
      * The colon_order_traversal method is called after the lhs is
      * traversed, but before the rhs is traversed, by the traversal
      * method.
      */
    virtual void colon_order_traversal(int pprec) const;

    /**
      * The pre_order_traversal method is called after the rhs is
      * traversed, by the traversal method.
      */
    virtual void post_order_traversal(int pprec) const;

private:
    /**
      * The cond instance variable is used to remember the control
      * expression.  It must be a boolean value.
      */
    expression::pointer cond;

    /**
      * The cond instance variable is used to remember the value to use
      * if cond is true.
      */
    expression::pointer lhs;

    /**
      * The cond instance variable is used to remember the value to use
      * if cond is false.
      */
    expression::pointer rhs;

    /**
      * The xlat instance variable is used to remember the translation
      * context we are working within.
      */
    translator &xlat;

    /**
      * The default constructor.  Do not use.
      */
    expression_ternary();

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_ternary(const expression_ternary &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_ternary &operator=(const expression_ternary &rhs);
};

#endif // LIB_EXPRESSION_TERNARY_H
// vim: set ts=8 sw=4 et :
