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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_COMMA_H
#define UCSDPSYS_COMPILE_EXPRESSION_COMMA_H

#include <lib/expression.h>
#include <lib/expression/list.h>

class translator_compile; // forward

/**
  * The expression_comma class is used to represent a comma expression
  * (named after the C comma expression), which is actually a series of
  * expressions.  All but the last expression is of type nothing.
  */
class expression_comma:
    public expression
{
public:
    typedef boost::shared_ptr<expression_comma> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_comma();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param rhs
      *     The parameters of the function call.
      * @param code
      *     The translation context we are working within.
      */
    static pointer create(const expression_list &rhs, translator_compile *code);

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
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param rhs
      *     The parameters of the function call.
      * @param code
      *     The translation context we are working within.
      */
    expression_comma(const expression_list &rhs, translator_compile *code);

    /**
      * The rhs instance variable is used to remember the parameters of
      * the function call.
      */
    expression_list rhs;

    /**
      * The code instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_comma();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_comma(const expression_comma &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_comma &operator=(const expression_comma &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_COMMA_H
// vim: set ts=8 sw=4 et :
