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

#ifndef LIB_EXPRESSION_LINT_FROM_INTEGER_H
#define LIB_EXPRESSION_LINT_FROM_INTEGER_H

#include <lib/expression.h>

/**
  * The expression_lint_from_integer class is used to represent
  * the conversion from an INTEGER to an INTEGER[].
  */
class expression_lint_from_integer:
    public expression
{
public:
    typedef boost::shared_ptr<expression_lint_from_integer> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_lint_from_integer();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param operand
      *     The value to be cast.
      */
    expression_lint_from_integer(const expression::pointer &operand);

    expression::pointer get_operand(void) const { return operand; }

    // See base class for documentation.
    void traversal(int pprec) const;

    virtual void pre_order_traversal(void) const;

    virtual void post_order_traversal(void) const;

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

private:
    /**
      * The operand instance variable is used to remember the operand to
      * this conversion.
      */
    expression::pointer operand;

    /**
      * The default constructor.  Do not use.
      */
    expression_lint_from_integer();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_lint_from_integer(const expression_lint_from_integer &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_lint_from_integer &operator=(
        const expression_lint_from_integer &rhs);
};

#endif // LIB_EXPRESSION_LINT_FROM_INTEGER_H
// vim: set ts=8 sw=4 et :
