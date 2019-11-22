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

#ifndef LIB_EXPRESSION_PAOC_FROM_STRING_H
#define LIB_EXPRESSION_PAOC_FROM_STRING_H

#include <lib/expression.h>

/**
  * The expression_paoc_from_string class is used to represent a cast of
  * a string constant to a packed array of char constant.
  */
class expression_paoc_from_string:
    public expression
{
public:
    typedef boost::shared_ptr<expression_paoc_from_string> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_paoc_from_string();

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      *
      * @param rhs
      *     The value to cast.
      * @param tp
      *     The exact type of the result.
      */
    expression_paoc_from_string(const expression::pointer &rhs,
        const type::pointer &tp);

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    bool is_constant(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    /**
      * The get_rhs method is used to obtain the right hand side of the
      * expression.
      */
    expression::pointer get_rhs(void) const { return rhs; }

    /**
      * The get_tp method is used to obtain the type of the expression.
      */
    type::pointer get_tp(void) const { return tp; }

private:
    /**
      * The rhs instance variable is used to remember the value to cast.
      */
    expression::pointer rhs;

    /**
      * The tp instance variable is used to remember the exact type of
      * the result.
      */
    type::pointer tp;

    /**
      * The default constructor.  Do not use.
      */
    expression_paoc_from_string();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_paoc_from_string(const expression_paoc_from_string &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_paoc_from_string &operator=(
        const expression_paoc_from_string &rhs);
};

#endif // LIB_EXPRESSION_PAOC_FROM_STRING_H
// vim: set ts=8 sw=4 et :
