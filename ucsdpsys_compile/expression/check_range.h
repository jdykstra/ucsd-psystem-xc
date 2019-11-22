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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_CHECK_RANGE_H
#define UCSDPSYS_COMPILE_EXPRESSION_CHECK_RANGE_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_check_range class is used to represent code to check
  * the range of an integer value before assigning it to a subrange
  * typed variable, or using it as an array index.
  */
class expression_check_range:
    public expression
{
public:
    typedef boost::shared_ptr<expression_check_range> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_check_range();

private:
    /**
      * The constructor.
      */
    expression_check_range(const expression::pointer &value,
        const expression::pointer &minimum,
        const expression::pointer &maximum, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &value,
        const expression::pointer &minimum,
        const expression::pointer &maximum, translator_compile *cntxt);

protected:
    // See baseline for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See baseline for documentation.
    void traversal(int pprec) const;

    // See baseline for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See baseline for documentation.
    int get_precedence() const;

    // See baseline for documentation.
    type::pointer get_type() const;

    // See baseline for documentation.
    bool is_lvalue() const;

    // See baseline for documentation.
    side_effect_t has_side_effect() const;

    // See baseline for documentation.
    symbol::pointer get_symbol_hint() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The code instance variable is used to remember wheer to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The value instance variable is used to remember the value of
      * this expression.
      */
    expression::pointer value;

    /**
      * The minimum instance variable is used to remember the minimum
      * accepttable value.
      */
    expression::pointer minimum;

    /**
      * The maximum instance variable is used to remember the maximum
      * acceptable value.
      */
    expression::pointer maximum;

    /**
      * The default constructor.  Do not use.
      */
    expression_check_range();

    /**
      * The copy constructor.  Do not use.
      */
    expression_check_range(const expression_check_range &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_check_range &operator=(const expression_check_range &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_CHECK_RANGE_H
// vim: set ts=8 sw=4 et :
