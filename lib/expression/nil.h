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

#ifndef LIB_EXPRESSION_NIL_H
#define LIB_EXPRESSION_NIL_H

#include <lib/expression.h>

/**
  * The expression_nil class is used to represent the value of the NIL
  * built-in named constant.  (In C that would be the NULL pointer.)
  */
class expression_nil:
    public expression
{
public:
    typedef boost::shared_ptr<expression_nil> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_nil();

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
      */
    expression_nil(const location &locn);

    // See base class for documentation.
    bool is_constant() const;

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

private:
    /**
      * The default constructor.  Do not use.
      */
    expression_nil();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_nil(const expression_nil &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_nil &operator=(const expression_nil &rhs);
};

#endif // LIB_EXPRESSION_NIL_H
// vim: set ts=8 sw=4 et :
