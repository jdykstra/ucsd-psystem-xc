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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_SIZEOF_EXPRESSION_H
#define UCSDPSYS_PRETTY_EXPRESSION_SIZEOF_EXPRESSION_H

#include <lib/expression.h>

class translator_littoral; // forward

/**
  * The expression_sizeof_expression class is used to represent littoral
  * printing the size of an expression.
  */
class expression_sizeof_expression:
    public expression
{
public:
    typedef boost::shared_ptr<expression_sizeof_expression> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_sizeof_expression();

private:
    /**
      * The constructor.
      */
    expression_sizeof_expression(const expression::pointer &subject,
        translator_littoral *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &subject,
        translator_littoral *cntxt);

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
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The littoral instance variable is used to remember where to send
      * our littoral printed source code.
      */
    translator_littoral &littoral;

    /**
      * The subject instance variable is used to remember the expression
      * we are getting the size of.
      */
    expression::pointer subject;

    /**
      * The default constructor.
      */
    expression_sizeof_expression();

    /**
      * The copy constructor.
      */
    expression_sizeof_expression(const expression_sizeof_expression &);

    /**
      * The assignment operator.
      */
    expression_sizeof_expression &operator=(
        const expression_sizeof_expression &);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_SIZEOF_EXPRESSION_H
// vim: set ts=8 sw=4 et :
