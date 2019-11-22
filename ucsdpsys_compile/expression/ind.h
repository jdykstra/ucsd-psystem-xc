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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_IND_H
#define UCSDPSYS_COMPILE_EXPRESSION_IND_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_ind class is used to represent the set of IND 0..32767
  * opcodes (INDirect load).
  */
class expression_ind:
    public expression
{
public:
    typedef boost::shared_ptr<expression_ind> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_ind();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param lhs
      *     The integer pointer to be loaded from.
      * @param rhs
      *     The offset from the left hand pointer.
      * @param tp
      *     The type of the result left on the stack after this
      *     opteration is performed.
      * @param code
      *     The translator context we are working within.
      */
    expression_ind(const expression::pointer &lhs, int rhs,
        const type::pointer &tp, translator_compile &code);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The integer pointer to be loaded from.
      * @param rhs
      *     The offset from the left hand pointer.
      * @param tp
      *     The type of the result on the stack after this opcode completes.
      * @param code
      *     The translator context we are working within.
      */
    static pointer create(const expression::pointer &lhs, int rhs,
        const type::pointer &tp, translator_compile &code);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

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
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The lhs instance variable is used to remember the integer
      * pointer to be loaded from.
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the offset from
      * the left hand pointer.
      */
    int rhs;

    /**
      * The tp instance variable is used to remember the result type of
      * this expression.
      */
    type::pointer tp;

    /**
      * The code instance variable is used to remember the translator
      * context we are operating within.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_ind();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_ind(const expression_ind &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_ind &operator=(const expression_ind &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_IND_H
// vim: set ts=8 sw=4 et :
