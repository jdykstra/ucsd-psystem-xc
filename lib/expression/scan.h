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

#ifndef LIB_EXPRESSION_SCAN_H
#define LIB_EXPRESSION_SCAN_H

#include <lib/expression.h>

/**
  * The expression_scan class is used to represent a weird SCAN function
  * call with the embedded non-syntactic "=" or "<>" comparison.
  */
class expression_scan:
    public expression
{
public:
    typedef boost::shared_ptr<expression_scan> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_scan();

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      *
      * @param maxdisp
      *     How far to scan; backwards if negative, forwards if positive.
      * @param mask
      *     Scan for equality if 0, inequality if 1.
      * @param value
      *     The character value to scan for.
      * @param start
      *     Expression evaluating to a byte pointer (reference), scan
      *     starting position.
      */
    expression_scan(const expression::pointer &maxdisp, int mask,
        const expression::pointer &value, const expression::pointer &start);

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    expression::pointer get_maxdisp(void) const { return maxdisp; }
    int get_mask(void) const { return mask; }
    expression::pointer get_value(void) const { return value; }
    expression::pointer get_start(void) const { return start; }

private:
    /**
      * The maxdisp instance variable is used to remember how far to
      * scan; backwards if negative, forwards if positive.
      */
    expression::pointer maxdisp;

    /**
      * The mask instance variable is used to remember whether to can
      * for equality (0) or inequality (1).
      */
    int mask;

    /**
      * The value instance variable is used to remember the character
      * value to scan for.
      */
    expression::pointer value;

    /**
      * The start instance variable is used to remember expression
      * evaluating to a byte pointer (reference), scan starting
      * position.
      */
    expression::pointer start;

    /**
      * The default constructor.  Do not use.
      */
    expression_scan();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_scan(const expression_scan &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_scan &operator=(const expression_scan &rhs);
};

#endif // LIB_EXPRESSION_SCAN_H
// vim: set ts=8 sw=4 et :
