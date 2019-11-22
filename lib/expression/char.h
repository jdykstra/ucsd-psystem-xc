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

#ifndef LIB_EXPRESSION_CHAR_H
#define LIB_EXPRESSION_CHAR_H

#include <lib/expression.h>

/**
  * The expression_char class is used to represent an abstract
  * character-valued expression node.
  */
class expression_char:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_char();

    /**
      * The constructor.
      *
      * @param locn
      *     the source code location of the expression.
      * @param value
      *     the value of the character constant
      */
    expression_char(const location &locn, unsigned char value);

protected:
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
    bool is_constant() const;

    // See base class for documentation.
    long get_integer_value() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The value instance variable is used to remember the value of the
      * character constant
      */
    unsigned char value;

    /**
      * The default constructor.  Do not use.
      */
    expression_char();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_char(const expression_char &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_char &operator=(const expression_char &);
};

#endif // LIB_EXPRESSION_CHAR_H
// vim: set ts=8 sw=4 et :
