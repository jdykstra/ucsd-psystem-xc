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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_H
#define UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_H

#include <lib/expression.h>
#include <lib/symbol/function.h>

class translator_compile; // forward

/**
  * The expression_function class is used to represent a simple function
  * name in an expression.  It is a precursor to creating a function
  * call expression, or a function return value assignment.  Only an
  * error will see this expression attempt to be compiled.
  */
class expression_function:
    public expression
{
public:
    typedef boost::shared_ptr<expression_function> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_function();

private:
    /**
      * The constructor.
      */
    expression_function(const location &locn,
        const symbol_function::pointer &sfp, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const location &locn,
        const symbol_function::pointer &sfp, translator_compile *cntxt);

    /**
      * The get_sfp method is used to obtain access the function symbol.
      */
    symbol_function::pointer get_sfp(void) const { return sfp; }

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer assignment_expression_factory(
        const expression::pointer &rhs) const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    expression::pointer function_call_expression_factory(
        const expression_list &args) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The sfp instance variable is used to remember the function being
      * referred to.
      */
    symbol_function::pointer sfp;

    /**
      * The default constructor.  Do not use.
      */
    expression_function();

    /**
      * The copy constructor.  Do not use.
      */
    expression_function(const expression_function &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_function &operator=(const expression_function &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_H
// vim: set ts=8 sw=4 et :
