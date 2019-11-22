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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_ARRAY_INDEX_H
#define UCSDPSYS_PRETTY_EXPRESSION_ARRAY_INDEX_H

#include <lib/expression.h>

class translator_pretty; // forward

/**
  * The expression_array_index class is used to represent pretty
  * printing of an array index expression.
  */
class expression_array_index:
    public expression
{
public:
    typedef boost::shared_ptr<expression_array_index> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_array_index();

private:
    /**
      * The constructor.
      */
    expression_array_index(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_pretty *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_pretty *cntxt);

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
    expression::pointer optimize() const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    expression::pointer clone(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer assignment_expression_factory(
        const expression::pointer &rhs) const;

private:
    /**
      * The pretty instance variable is used to remember where to send
      * our pretty source code.
      */
    translator_pretty &pretty;

    /**
      * The lhs instance variable is used to remember the array to be
      * indexed.
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the element index
      * expression.
      */
    expression::pointer rhs;

    /**
      * The default constructor.  Do not use.
      */
    expression_array_index();

    /**
      * The copy constructor.  Do not use.
      */
    expression_array_index(const expression_array_index &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_array_index &operator=(const expression_array_index &);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_ARRAY_INDEX_H
// vim: set ts=8 sw=4 et :
