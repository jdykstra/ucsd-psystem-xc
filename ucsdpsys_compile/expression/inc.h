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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_INC_H
#define UCSDPSYS_COMPILE_EXPRESSION_INC_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_inc class is used to represent an INC opcode, which
  * adds a constant number of words to a pointer.
  */
class expression_inc:
    public expression
{
public:
    typedef boost::shared_ptr<expression_inc> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_inc();

private:
    /**
      * The constructor.
      *
      * @param tp
      *     The type of the resulting pointer
      * @param lhs
      *     The pointer expression to be added to
      * @param rhs
      *     the number of words to add to the pointer value
      * @param hint
      *     symbol hint for error messages
      * @param cntxt
      *     where to send our generated code
      */
    expression_inc(const type::pointer &tp, const expression::pointer &lhs,
        int rhs, const symbol::pointer &hint, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param tp
      *     The type of the resulting pointer
      * @param lhs
      *     The pointer expression to be added to
      * @param rhs
      *     the number of words to add to the pointer value
      * @param hint
      *     symbol hint for error messages
      * @param cntxt
      *     where to send our generated code
      */
    static pointer create(const type::pointer &tp,
        const expression::pointer &lhs, int rhs, const symbol::pointer &hint,
        translator_compile *cntxt);

    /**
      * Can this expression tree be turned into an IND opcode?
      * It may be optiomized to SIND, later.
      *
      * @param tp
      *     type type of the result
      * @param offset
      *     offset in addition to the rhs offset already present
      * @returns
      *     pointer to new expression tree, or NULL if not suitable.
      */
    expression::pointer indirect_load(const type::pointer &tp,
        int offset) const;

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
    symbol::pointer get_symbol_hint() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    expression::pointer get_lhs() const { return lhs; }
    int get_rhs() const { return rhs; }

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The tp instance variable is used to remember the type of the
      * resulting pointer.
      */
    type::pointer tp;

    /**
      * The lhs instance variable is used to remember the pointer
      * expression to be added to.
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the number of
      * words to add to the pointer value
      */
    int rhs;

    /**
      * The hint instance variable is used to remember a symbol hint for
      * use in error messages.
      */
    symbol::pointer hint;

    /**
      * The default constructor.
      */
    expression_inc();

    /**
      * The copy constructor.
      */
    expression_inc(const expression_inc &);

    /**
      * The assignment operator.
      */
    expression_inc &operator=(const expression_inc &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_INC_H
// vim: set ts=8 sw=4 et :
