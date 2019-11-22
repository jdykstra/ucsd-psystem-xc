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

#ifndef LIB_EXPRESSION_LINT2LINT_H
#define LIB_EXPRESSION_LINT2LINT_H

#include <lib/expression.h>

/**
  * The expression_lint2lint class is used to represent a cast from one
  * long integer size to another.
  */
class expression_lint2lint:
    public expression
{
public:
    typedef boost::shared_ptr<expression_lint2lint> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_lint2lint();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param to
      *     The type the expression is to be converted to.
      * @param from
      *     The expression to be converted (cast).
      */
    expression_lint2lint(const type::pointer &to,
        const expression::pointer &from);

    // See base class for docimentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for docimentation.
    int get_precedence(void) const;

    // See base class for docimentation.
    type::pointer get_type(void) const;

    // See base class for docimentation.
    void clear_location_recursive(void);

    // See base class for docimentation.
    bool is_lvalue(void) const;

    // See base class for docimentation.
    side_effect_t has_side_effect(void) const;

    // See base class for docimentation.
    rcstring lisp_representation(void) const;

    expression::pointer get_from(void) const { return from; }

private:
    /**
      * The to instance variable is used to remember the type we are
      * casting the expression to.
      */
    type::pointer to;

    /**
      * The from instance variable is used to remember the expression to
      * be cast.
      */
    expression::pointer from;

    /**
      * The default constructor.  Do not use.
      */
    expression_lint2lint();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_lint2lint(const expression_lint2lint &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_lint2lint &operator=(const expression_lint2lint &rhs);
};

#endif // LIB_EXPRESSION_LINT2LINT_H
// vim: set ts=8 sw=4 et :
