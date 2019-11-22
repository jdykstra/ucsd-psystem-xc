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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_UPTR2PPTR_H
#define UCSDPSYS_COMPILE_EXPRESSION_UPTR2PPTR_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_uptr2pptr class is used to represent a pointer cast
  * from unpacked to packed layout.
  *
  * Packed pointers appear on the stack as
  * <pre>
  *    word 2 (tos-2) word pointer to word field is in
  *    word 1 (tos-1) field width (in bits)
  *    word 0 (tos-0) right bit number of field.
  * </pre>
  * The above order of operands is used by the constructor to reinforce
  * the semantics, and use common expectations.
  *
  * The bit order in memory differs for big-endian and little-endian.
  * The word is loaded in the appropriate endian way, and then the field
  * is extracted from it.  Once loaded, we are able to ignore endian
  * issues, and the bits are interpreted as:
  * <pre>
  *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *   | F | E | D | C | B | A | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  *    MSB                                                         LSB
  * </pre>
  * The right bit number is shown in each box, and fields with extends
  * from right to left.  This is the same interpretation that C uses,
  * making implementing a p-machine in C very straight-forward.
  */
class expression_uptr2pptr:
    public expression
{
public:
    typedef boost::shared_ptr<expression_uptr2pptr> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_uptr2pptr();

private:
    /**
      * The constructor.
      *
      * @param to
      *     the type this is casting to
      * @param from
      *     the pointer expression to be cast
      * @param field_width
      *     the field width (in bits) of the field within the word
      *     pointed to by "from"
      * @param right_bit
      *     the right bit number of the field within the word pointed to
      *     by "from"
      * @param cntxt
      *     where to send our generated code.
      */
    expression_uptr2pptr(const type::pointer &to,
        const expression::pointer &from, const expression::pointer &field_width,
        const expression::pointer &right_bit, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param to
      *     the type this is casting to
      * @param from
      *     the pointer expression to be cast
      * @param field_width
      *     the field width (in bits) of the field within the word
      *     pointed to by "from"
      * @param right_bit
      *     the right bit number of the field within the word pointed to
      *     by "from"
      * @param cntxt
      *     where to send our generated code.
      */
    static pointer create(const type::pointer &to,
        const expression::pointer &from, const expression::pointer &field_width,
        const expression::pointer &right_bit, translator_compile *cntxt);

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
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The to instance variable is used to remember the type this
      * pointer is to be cast to.
      */
    type::pointer to;

    /**
      * The from instance variable is used to remember the base word
      * pointer.
      */
    expression::pointer from;

    /**
      * The field_width instance variable is used to remember the
      * bit-width of the packed field being accessed.
      */
    expression::pointer field_width;

    /**
      * The right_bit instance variable is used to remember the
      * bit-offset of the field within the field.
      */
    expression::pointer right_bit;

    /**
      * The default constructor.
      */
    expression_uptr2pptr();

    /**
      * The copy constructor.
      */
    expression_uptr2pptr(const expression_uptr2pptr &);

    /**
      * The assignment operator.
      */
    expression_uptr2pptr &operator=(const expression_uptr2pptr &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_UPTR2PPTR_H
// vim: set ts=8 sw=4 et :
