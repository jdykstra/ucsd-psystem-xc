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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_SEGMENT_RELATIVE_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_SEGMENT_RELATIVE_H

#include <lib/expression.h>

/**
  * The expression_segment_relative class is used to represent an
  * address that is relative to the current segment.  (In contrast to an
  * absolute address, represented using #expression_integer_asm.)
  */
class expression_segment_relative:
    public expression
{
public:
    typedef boost::shared_ptr<expression_segment_relative> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_segment_relative();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The location of the value in the source code.
      * @param value
      *     The value of the segment relative pointer.
      */
    static pointer create(const location &locn, long value);

    // See base class for documentation.
    long get_integer_value(void) const;

protected:
    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    bool is_constant(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param locn
      *     The location of the value in the source code.
      * @param value
      *     The value of the segment relative pointer.
      */
    expression_segment_relative(const location &locn, long value);

    /**
      * The value instance variable is used to remember
      * the byte offset into the segment code.
      */
    long value;

    /**
      * The default constructor.  Do not use.
      */
    expression_segment_relative();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_segment_relative(const expression_segment_relative &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_segment_relative &operator=(
        const expression_segment_relative &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_SEGMENT_RELATIVE_H
// vim: set ts=8 sw=4 et :
