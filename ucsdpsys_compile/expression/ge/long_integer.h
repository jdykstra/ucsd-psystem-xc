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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_GE_LONG_INTEGER_H
#define UCSDPSYS_COMPILE_EXPRESSION_GE_LONG_INTEGER_H

#include <lib/expression/ge.h>

class translator_compile; // forward

/**
  * The expression_ge_long_integer class is used to represent code
  * generation for greater than or equal to expressions, of long
  * integers.
  */
class expression_ge_long_integer:
    public expression_ge
{
public:
    typedef boost::shared_ptr<expression_ge_long_integer> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_ge_long_integer();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @param code
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *code);

protected:
    // See base class for documentation.
    void post_order_traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @param code
      *     The translation context we are working within.
      */
    expression_ge_long_integer(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *code);

    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_ge_long_integer();

    /**
      * The copy constructor.  Do not use.
      */
    expression_ge_long_integer(const expression_ge_long_integer &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_ge_long_integer &operator=(const expression_ge_long_integer &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_GE_LONG_INTEGER_H
// vim: set ts=8 sw=4 et :
