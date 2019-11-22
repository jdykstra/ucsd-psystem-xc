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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_ADDRESS_OF_H
#define UCSDPSYS_PRETTY_EXPRESSION_ADDRESS_OF_H

#include <lib/expression.h>

class translator_pretty; // forward

/**
  * The expression_address_of class is used to represent a "at" (address
  * of) expression, for pretty printing.
  */
class expression_address_of:
    public expression
{
public:
    typedef boost::shared_ptr<expression_address_of> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_address_of();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param rhs
      *     The operand.
      * @param context
      *     The translator context we are working within.
      */
    expression_address_of(const expression::pointer &rhs,
        translator_pretty *context);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param rhs
      *     The operand.
      * @param context
      *     The translator context we are working within.
      */
    static pointer create(const expression::pointer &rhs,
        translator_pretty *context);

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
      * The rhs instance variable is used to remember the operand.
      */
    expression::pointer rhs;

    /**
      * The pretty instance variable is used to remember the translator
      * context we are working within.
      */
    translator_pretty &pretty;

    /**
      * The default constructor.  Do not use.
      */
    expression_address_of();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_address_of(const expression_address_of &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_address_of &operator=(const expression_address_of &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_ADDRESS_OF_H
// vim: set ts=8 sw=4 et :
