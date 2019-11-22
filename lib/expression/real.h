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

#ifndef LIB_EXPRESSION_REAL_H
#define LIB_EXPRESSION_REAL_H

#include <lib/expression.h>
#include <lib/rcstring.h>

/**
  * The expression_real class is used to represent a real-valued
  * constant expression node.
  */
class expression_real:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_real();

    /**
      * The constructor.
      *
      * @param value
      *     the content of the real constant.
      * @param locn
      *     the source location of the constant
      */
    expression_real(const location &locn, const rcstring &value);

    /**
      * The constructor.
      *
      * @param value
      *     the content of the real constant.
      * @param locn
      *     the source location of the constant
      */
    expression_real(const location &locn, double value);

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

    /**
      * The get_value method is used to obtain the value of the real
      * constant.  It is raw data, you willprobably need to escape it if
      * you are going to print it.
      */
    double get_value() const { return value; }

protected:
    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The value instance variable is used to remember the content of
      * the real constant.
      */
    double value;

    /**
      * The default constructor.  Do not use.
      */
    expression_real();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_real(const expression_real &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_real &operator=(const expression_real &);
};

#endif // LIB_EXPRESSION_REAL_H
// vim: set ts=8 sw=4 et :
