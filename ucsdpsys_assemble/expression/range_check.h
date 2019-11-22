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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_RANGE_CHECK_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_RANGE_CHECK_H

#include <lib/expression.h>

/**
  * The expression_range_check class is used to represent an identity
  * transform, but it checks the value for a specific range.
  */
class expression_range_check:
    public expression
{
public:
    typedef boost::shared_ptr<expression_range_check> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_range_check();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param parameter_number
      *     The parameter number of this expression.
      * @param operand
      *     The value to be checked.
      * @param minimum
      *     The minimum permitted value.
      * @param maximum
      *     The maximum permitted value.
      */
    static expression::pointer create(int parameter_number,
        const expression::pointer &operand, long minimum, long maximum);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.  (The parameter number
      * defaults to one.)
      *
      * @param operand
      *     The value to be checked.
      * @param minimum
      *     The minimum permitted value.
      * @param maximum
      *     The maximum permitted value.
      */
    static expression::pointer create(const expression::pointer &operand,
        long minimum, long maximum);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

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
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param parameter_number
      *     The parameter number of this expression.
      * @param operand
      *     The value to be checked.
      * @param minimum
      *     The minimum permitted value.
      * @param maximum
      *     The maximum permitted value.
      */
    expression_range_check(int parameter_number,
        const expression::pointer &operand, long minimum, long maximum);

    /**
      * The parameter_number instance variable is used to remember
      * the parameter number of this check.
      */
    int parameter_number;

    /**
      * The operand instance variable is used to remember the expression
      * tree of the value to be checked.
      */
    expression::pointer operand;

    /**
      * The minimum instance variable is used to remember the minimum
      * permitted value.
      */
    long minimum;

    /**
      * The maximum instance variable is used to remember the maximum
      * permitted value.
      */
    long maximum;

    /**
      * The default constructor.  Do not use.
      */
    expression_range_check();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_range_check(const expression_range_check &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_range_check &operator=(const expression_range_check &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_RANGE_CHECK_H
// vim: set ts=8 sw=4 et :
