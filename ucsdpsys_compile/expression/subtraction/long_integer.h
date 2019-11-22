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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_SUBTRACTION_LONG_INTEGER_H
#define UCSDPSYS_COMPILE_EXPRESSION_SUBTRACTION_LONG_INTEGER_H

#include <lib/expression/subtraction.h>

class translator_compile; // forward

/**
  * The expression_subtraction_long_integer class is used to represent
  * the code generation required to subtract long integers.
  */
class expression_subtraction_long_integer:
    public expression_subtraction
{
public:
    typedef boost::shared_ptr<expression_subtraction_long_integer> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_subtraction_long_integer();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @param code
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *code);

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    void post_order_traversal(int pprec) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @param code
      *     The translation context we are working within.
      */
    expression_subtraction_long_integer(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *code);

    /**
      * The code instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_subtraction_long_integer();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_subtraction_long_integer(
        const expression_subtraction_long_integer &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_subtraction_long_integer &operator=(
        const expression_subtraction_long_integer &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_SUBTRACTION_LONG_INTEGER_H
// vim: set ts=8 sw=4 et :
