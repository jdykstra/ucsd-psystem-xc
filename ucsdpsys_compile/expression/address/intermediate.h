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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_INTERMEDIATE_H
#define UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_INTERMEDIATE_H

#include <ucsdpsys_compile/expression/address.h>

class translator_compile; // forward

/**
  * The expression_address_intermediate class is used to represent an
  * intermediate (lower stack frame) address constant.
  */
class expression_address_intermediate:
    public expression_address
{
public:
    typedef boost::shared_ptr<expression_address_intermediate> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_address_intermediate();

private:
    /**
      * The constructor.
      *
      * @param locn
      *     The sourec file location of this expression
      * @param offset
      *     The word offset into the lower stack frame
      * @param type
      *     The type of the expression
      * @param down
      *     How many hops down the scall stack to go
      * @param hint
      *     The symbol most recently related to this expression, or NULL
      *     for no hint.
      * @param cntxt
      *     The translation context for this expression
      */
    expression_address_intermediate(const location &locn, int offset,
        const type::pointer &type, unsigned down, const symbol::pointer &hint,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The sourec file location of this expression
      * @param offset
      *     The word offset into the lower stack frame
      * @param type
      *     The type of the expression
      * @param down
      *     How many hops down the scall stack to go
      * @param hint
      *     The symbol most recently related to this expression, or NULL
      *     for no hint.
      * @param cntxt
      *     The translation context for this expression
      */
    static pointer create(const location &locn, int offset,
        const type::pointer &type, unsigned down, const symbol::pointer &hint,
        translator_compile *cntxt);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer fast_load() const;

    // See base class for documentation.
    expression::pointer fast_store(const expression::pointer &rhs)
        const;

    // See base class for documentation.
    bool is_constant() const;

    // See base class for documentation.
    expression_address::pointer advance_by_words(long nwords,
        const type::pointer &new_type, const symbol::pointer &new_hint) const;

    // See base class for documentation.
    symbol::pointer get_symbol_hint() const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer cast(const type::pointer &tp) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The down instance variable is used to remember how many stack
      * frames down the stack the offset refers to.
      */
    unsigned down;

    /**
      * The hint instance variable is used to remember the symbol most
      * recently related to this expression, or NULL for no hint.
      */
    symbol::pointer hint;

    /**
      * The default constructor.
      */
    expression_address_intermediate();

    /**
      * The copy constructor.
      */
    expression_address_intermediate(const expression_address_intermediate &);

    /**
      * The assignment operator.
      */
    expression_address_intermediate &operator=(
        const expression_address_intermediate &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_INTERMEDIATE_H
// vim: set ts=8 sw=4 et :
