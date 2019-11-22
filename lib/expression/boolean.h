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

#ifndef LIB_EXPRESSION_BOOLEAN_H
#define LIB_EXPRESSION_BOOLEAN_H

#include <lib/expression.h>

/**
  * The expression_boolean class is used to represent an abstract
  * boolean constant valued expression.
  */
class expression_boolean:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_boolean();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param where
      *     The source code location of the expression.
      * @param value
      *     The value of this constant boolean expression.
      */
    expression_boolean(const location &where, bool value);

    /**
      * The get_value method is used to obtrain the value of this
      * constant expression.
      */
    bool get_value(void) const { return value; }

    // See base class for documentation.
    bool is_constant(void) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    bool get_boolean_value(void) const;

    // See base class for documentation.
    long get_integer_value(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    rcstring get_printable_value(void) const;

private:
    /**
      * The value instance variable is used to remember the value of
      * this constant boolean expression.
      */
    bool value;

    /**
      * The default constructor.  Do not use.
      */
    expression_boolean();

protected:
    /**
      * The copy constructor.  Do not use.
      */
    expression_boolean(const expression_boolean &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_boolean &operator=(const expression_boolean &);
};

#endif // LIB_EXPRESSION_BOOLEAN_H
// vim: set ts=8 sw=4 et :
