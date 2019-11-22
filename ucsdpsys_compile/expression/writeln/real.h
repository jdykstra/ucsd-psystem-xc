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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_WRITELN_REAL_H
#define UCSDPSYS_COMPILE_EXPRESSION_WRITELN_REAL_H

#include <ucsdpsys_compile/expression/writeln.h>

/**
  * The expression_writeln_real class is used to represent the value
  * and formatting specifiers for a REAL typed value.
  */
class expression_writeln_real:
    public expression_writeln
{
public:
    typedef boost::shared_ptr<expression_writeln_real> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_writeln_real();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param value
      *     The value to be printed.
      * @param width
      *     The field width to print it in.
      * @param precision
      *     The number of decimal places.
      * @param context
      *     The context we are working in.
      */
    expression_writeln_real(const expression::pointer &value,
        const expression::pointer &width, const expression::pointer &precision,
        translator_compile *context);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param value
      *     The value to be printed.
      * @param width
      *     The field width to print it in.
      * @param precision
      *     The number of decimal places.
      * @param code
      *     The context we are working in.
      */
    static expression::pointer create(const expression::pointer &value,
        const expression::pointer &width, const expression::pointer &precision,
        translator_compile *code);

protected:
    // See base class for documentation.
    expression::pointer function_call_expression_factory(
        const expression::pointer &file) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    translator_compile *context;
    expression::pointer precision;

    /**
      * The default constructor.  Do not use.
      */
    expression_writeln_real();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_writeln_real(const expression_writeln_real &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_writeln_real &operator=(const expression_writeln_real &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_WRITELN_REAL_H
// vim: set ts=8 sw=4 et :
