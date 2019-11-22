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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_TRUNC_H
#define UCSDPSYS_COMPILE_EXPRESSION_TRUNC_H

#include <lib/expression.h>

/**
  * The expression_trunc class is used to represent a generic TRUNC
  * opcode.
  */
class expression_trunc:
    public expression
{
public:
    typedef boost::shared_ptr<expression_trunc> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_trunc();

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      *
      * @param operand
      *     The value to be truncated.
      */
    expression_trunc(const expression::pointer &operand);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    /**
      * The pre_order_traversal method is called before the operand is
      * traversed, by the #traversal method.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is called after the operand is
      * traversed, by the #traversal method.
      */
    virtual void post_order_traversal(int pprec) const;

    // See base class for documentation.
    bool is_constant(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    /**
      * The get_operand method may be used to obtain the first parameter
      * of the TRUNC function call.
      */
    expression::pointer get_operand(void) const { return operand; }

private:
    /**
      * The operand instance variable is used to remember the value to
      * be truncated.
      */
    expression::pointer operand;

    /**
      * The default constructor.  Do not use.
      */
    expression_trunc();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_trunc(const expression_trunc &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_trunc &operator=(const expression_trunc &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_TRUNC_H
// vim: set ts=8 sw=4 et :
