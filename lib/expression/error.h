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

#ifndef LIB_EXPRESSION_ERROR_H
#define LIB_EXPRESSION_ERROR_H

#include <lib/expression.h>

/**
  * The expression_error class is used to represent a place-holder
  * expression to be used when an expression tree building error has
  * ocurred.
  */
class expression_error:
    public expression
{
public:
    typedef boost::shared_ptr<expression_error> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_error();

private:
    /**
      * The constructor.
      *
      * @param loc
      *     the location within a sourc e file.
      */
    expression_error(const location &loc);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param loc
      *     the location within a sourc e file.
      */
    static pointer create(const location &loc);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer assignment_expression_factory(
        const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer function_call_expression_factory(
        const expression_list &args);

    // See base class for documentation.
    expression::pointer dot_expression_factory(const location &locn,
        const variable_name &member_name);

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The default constructor.  Do not use.
      */
    expression_error();

    /**
      * The copy constructor.  Do not use.
      */
    expression_error(const expression_error &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_error &operator=(const expression_error &);
};

#endif // LIB_EXPRESSION_ERROR_H
// vim: set ts=8 sw=4 et :
