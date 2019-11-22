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

#ifndef LIB_EXPRESSION_INTEGER_H
#define LIB_EXPRESSION_INTEGER_H

#include <lib/expression.h>
#include <lib/rcstring.h>

/**
  * The expression_integer class is used to represent an integer-valued
  * constant expression node.
  */
class expression_integer:
    public expression
{
public:
    typedef boost::shared_ptr<expression_integer> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_integer();

    /**
      * The constructor.
      *
      * @param locn
      *     The source file location of this expression.
      * @param value
      *     the content of the integer constant.
      * @param locn
      *     The source location of the constant
      */
    expression_integer(const rcstring &value, const location &locn);

    /**
      * The constructor.
      *
      * @param value
      *     the content of the integer constant.
      * @param locn
      *     the source location of the constant
      */
    expression_integer(long value, const location &locn);

    // See base class for documentation.
    long get_integer_value() const;

protected:
    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    bool is_constant() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    rcstring get_printable_value(void) const;

private:
    /**
      * The value instance variable is used to remember the content of
      * the integer constant.
      */
    long value;

    /**
      * The default constructor.  Do not use.
      */
    expression_integer();

public:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_integer(const expression_integer &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_integer &operator=(const expression_integer &);
};

#endif // LIB_EXPRESSION_INTEGER_H
// vim: set ts=8 sw=4 et :
