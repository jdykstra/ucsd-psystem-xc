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

#ifndef LIB_EXPRESSION_LONG_INTEGER_H
#define LIB_EXPRESSION_LONG_INTEGER_H

#include <lib/expression.h>
#include <lib/long_integer.h>

/**
  * The expression_long_integer class is used to represent a long
  * integer constant.
  */
class expression_long_integer:
    public expression
{
public:
    typedef boost::shared_ptr<expression_long_integer> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_long_integer();

    /**
      * The get_value method may be used to obtain the value of this
      * expression node.
      */
    const long_integer &get_value(void) const { return value; }

protected:
    /**
      * A constructor.
      * for use by derived classes only.
      *
      * @param locn
      *     The location of this expression in the sourec code.
      * @param tp
      *     The type of the expression.
      * @param value
      *     The value of the constant.
      */
    expression_long_integer(const location &locn, const type::pointer &tp,
        const long_integer &value);

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_constant(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    void logical_traversal(int, label::pointer &, label::pointer &, bool) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The tp instance variable is used to remember the type of this
      * expression node.
      */
    type::pointer tp;

    /**
      * The value instance variable is used to remember the value of this
      * expression node.
      */
    long_integer value;

    /**
      * The default constructor.  Do not use.
      */
    expression_long_integer();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_long_integer(const expression_long_integer &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_long_integer &operator=(const expression_long_integer &rhs);
};

#endif // LIB_EXPRESSION_LONG_INTEGER_H
// vim: set ts=8 sw=4 et :
