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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LONG_INTEGER_H
#define UCSDPSYS_COMPILE_EXPRESSION_LONG_INTEGER_H

#include <lib/expression/long_integer.h>

class translator_compile; // forward

/**
  * The expression_long_integer_compile class is used to represent the
  * code generation required for a constant expression node with an
  * INTEGER[n] value.
  */
class expression_long_integer_compile:
    public expression_long_integer
{
public:
    typedef boost::shared_ptr<expression_long_integer_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_long_integer_compile();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The location within the sourec code at which the expression occurs.
      * @param tp
      *     The type of the expression.
      * @param value
      *     The value of the expression.
      * @param code
      *     The translation context we are working within.
      */
    static expression::pointer create(const location &locn,
        const type::pointer &tp, const long_integer &value,
        translator_compile *code);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The location within the sourec code at which the expression occurs.
      * @param value
      *     The value of the expression.
      * @param code
      *     The translation context we are working within.
      */
    static expression::pointer create(const location &locn,
        const rcstring &value, translator_compile *code);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param locn
      *     The location within the sourec code at which the expression occurs.
      * @param tp
      *     The type of the expression.
      * @param value
      *     The value of the expression.
      * @param code
      *     The translation context we are working within.
      */
    expression_long_integer_compile(const location &locn,
        const type::pointer &tp, const long_integer &value,
        translator_compile *code);

    /**
      * The code instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_long_integer_compile();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_long_integer_compile(const expression_long_integer_compile &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_long_integer_compile &operator=(
        const expression_long_integer_compile &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LONG_INTEGER_H
// vim: set ts=8 sw=4 et :
