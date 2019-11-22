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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_ADDITION_ADDRESS_H
#define UCSDPSYS_COMPILE_EXPRESSION_ADDITION_ADDRESS_H

#include <lib/expression/addition.h>

class translator_compile; // forward

/**
  * The expression_addition_address class is used to represent adding
  * an address and a word offset together.  The type of the result is
  * specified explicitly, as opposed to the expression_addition_pointer
  * class (which optimises to this class eventually).
  */
class expression_addition_address:
    public expression_addition
{
public:
    typedef boost::shared_ptr<expression_addition_address> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_addition_address();

private:
    /**
      * The default constructor.
      *
      * @param tp
      *     The type of this expression node
      * @param lhs
      *     The base address
      * @param rhs
      *     The word offset to be applied.
      * @param hint
      *     The symbol most recently related to this expression, or NULL
      *     for no hint.
      * @param cntxt
      *     where to send our generated code.
      */
    expression_addition_address(const type::pointer &tp,
        const expression::pointer &lhs, const expression::pointer &rhs,
        const symbol::pointer &hint, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamicallay
      * allocated instances of this class.
      *
      * @param tp
      *     The type of this expression node
      * @param lhs
      *     The base address
      * @param rhs
      *     The word offset to be applied.
      * @param hint
      *     The symbol most recently related to this expression, or NULL
      *     for no hint.
      * @param cntxt
      *     where to send our generated code.
      */
    static pointer create(const type::pointer &tp,
        const expression::pointer &lhs, const expression::pointer &rhs,
        const symbol::pointer &hint, translator_compile *cntxt);

protected:
    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    void post_order_traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    symbol::pointer get_symbol_hint() const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The tp instance variable is used to remember the type of this
      * expression node
      */
    type::pointer tp;

    /**
      * The hint instance variable is used to remember the symbol most
      * recently related to this expression, or NULL for no hint.
      */
    symbol::pointer hint;

    /**
      * The default constructor.  Do not use.
      */
    expression_addition_address();

    /**
      * The copy constructor.  Do not use.
      */
    expression_addition_address(const expression_addition_address &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_addition_address &operator=(const expression_addition_address &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_ADDITION_ADDRESS_H
// vim: set ts=8 sw=4 et :
