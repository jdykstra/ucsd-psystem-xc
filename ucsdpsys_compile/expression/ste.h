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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_STE_H
#define UCSDPSYS_COMPILE_EXPRESSION_STE_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_ste class is used to represent an STE (STore External
  * word) opcode.
  */
class expression_ste:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_ste();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     source location of this code
      * @param segnum
      *     the segment number of the variable
      * @param offset
      *     the segment word offset of the variable
      * @param rhs
      *     the value to be stored
      * @param context
      *     The translaton context we are working within.
      */
    static pointer create(const location &locn, int segnum, int offset,
        const expression::pointer &rhs, translator_compile *context);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

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
      * The constructor.
      *
      * @param locn
      *     source location of this code
      * @param segnum
      *     the segment number of the variable
      * @param offset
      *     the segment word offset of the variable
      * @param rhs
      *     the value to be stored
      * @param context
      *     The translaton context we are working within.
      */
    expression_ste(const location &locn, int segnum, int offset,
        const expression::pointer &rhs, translator_compile *context);

    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The segnum instance variable is used to remember the segment
      * number of the variable.
      */
    int segnum;

    /**
      * The offset instance variable is used to remember the segment
      * word offset of the variable.
      */
    int offset;

    /**
      * The rhs instance variable is used to remember the value to be
      * stored.
      */
    expression::pointer rhs;

    /**
      * The default constructor.  Do not use.
      */
    expression_ste();

    /**
      * The copy constructor.  Do not use.
      */
    expression_ste(const expression_ste &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_ste &operator=(const expression_ste &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_STE_H
// vim: set ts=8 sw=4 et :
