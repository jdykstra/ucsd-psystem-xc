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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_STR_H
#define UCSDPSYS_COMPILE_EXPRESSION_STR_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_str class is used to represent an STR (store
  * intermediate word) opcode.
  */
class expression_str:
    public expression
{
public:
    typedef boost::shared_ptr<expression_str> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_str();

private:
    /**
      * The constructor.
      *
      * @param locn
      *     source location of this code
      * @param down
      *     how far to go down the static chain
      * @param offset
      *     the stack frame offset argument of this opcode
      * @param rhs
      *     the value to be stored
      * @param cntxt
      *     where to send our generated code
      */
    expression_str(const location &locn, unsigned down, int offset,
        const expression::pointer &rhs, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     source location of this code
      * @param down
      *     how far to go down the static chain
      * @param offset
      *     the stack frame offset argument of this opcode
      * @param rhs
      *     the value to be stored
      * @param cntxt
      *     where to send our generated code
      */
    static pointer create(const location &locn, unsigned down, int offset,
        const expression::pointer &rhs, translator_compile *cntxt);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

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
    rcstring lisp_representation(void) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The down instance variable is used to remember how far down the
      * static chain the stack frame of interest lies.
      */
    unsigned down;

    /**
      * The offset instance variable is used to remember the argument to
      * this opcode.
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
    expression_str();

    /**
      * The copy constructor.  Do not use.
      */
    expression_str(const expression_str &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_str &operator=(const expression_str &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_STR_H
// vim: set ts=8 sw=4 et :
