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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_H
#define UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_H

#include <lib/expression.h>

/**
  * The expression_address class is used to represent an abstract
  * address constant.
  */
class expression_address:
    public expression
{
public:
    typedef boost::shared_ptr<expression_address> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_address();

public:
    /**
      * The fast_load method is used by the
      * expression_load_indirect::optimize method to produce smaller
      * code, if possible.
      *
      * @returns
      *     A load expression, or the NULL pointer of no fast load
      *     expression tree (opcode) is available.
      *
      * @note
      *     It is tempting to use a default implementation which returns
      *     NULL, however this prevents the compiler from telling us
      *     we missed something.
      */
    virtual expression::pointer fast_load(void) const = 0;

    /**
      * The fast_store method is used by the
      * expression_store_indirect::optimize method to produce smaller
      * code, if possible.
      *
      * @param rhs
      *     The value to be stored.
      * @returns
      *     A load expression, or the NULL pointer of no fast load
      *     expression tree (opcode) is available.
      *
      * @note
      *     It is tempting to use a default implementation which returns
      *     NULL, however this prevents the compiler from telling us
      *     we missed something.
      */
    virtual expression::pointer fast_store(
        const expression::pointer &rhs) const = 0;

    /**
      * The advance_by_words method is used to create a new pointer
      * constant from an old one, with the offset advanced by the given
      * number of words.
      *
      * @param nwords
      *     The number of words to advance.
      * @param new_type
      *     the new type of pointer, because usually the original is a
      *     record pointer, and the new one is a member pointer (i.e.
      *     not the same type).
      * @param new_hint
      *     New hit to attach to the new pointer node
      * @returns
      *     a new expression node, or NULL if the operation would be invalid
      */
    virtual pointer advance_by_words(long nwords,
        const type::pointer &new_type, const symbol::pointer &new_hint)
        const = 0;

    /**
      * The cast method is used to cast an address (a pointer to
      * something) to be a pointer to something else.  The value is
      * unchanged, only the type is altered.
      *
      * @param tp
      *     The new type for the new expression.
      * @returns
      *     a new expression with exactly the same value, but a different type
      */
    virtual expression::pointer cast(const type::pointer &tp) const = 0;

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param locn
      *     The source code location of the expression.
      * @param offset
      *     The offset of the variable.
      * @param ptype
      *     The type of this address (pointer-to whatever the variable is).
      */
    expression_address(const location &locn, int offset,
        const type::pointer &ptype);

    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_address(const expression_address &);

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    /**
      * The get_offset method is used to obtain the offset (in 16-bit
      * words, zero based) from the base address represented by this
      * expression node.
      */
    unsigned get_offset(void) const { return offset; }

private:
    /**
      * The offset instance variable is used to remember the offset from
      * the base, in words.  Zero based.
      */
    int offset;

    /**
      * The ptype instance variable is used to remember the type of this
      * pointer expression node.
      */
    type::pointer ptype;

    /**
      * The default constructor.  Do not use.
      */
    expression_address();

    /**
      * The assignment operator.  Do not use.
      */
    expression_address &operator=(const expression_address &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_H
// vim: set ts=8 sw=4 et :
