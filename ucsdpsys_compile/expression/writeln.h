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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_WRITELN_H
#define UCSDPSYS_COMPILE_EXPRESSION_WRITELN_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_writeln class is used to represent an parameter to a
  * write() or writeln() call.  It has an optional width and an optional
  * precision, depending on the type.
  *
  * This exprtession doesn't actually generate any code.  It is
  * a place-holder until the complete parameter list is known, at
  * which time the parameter list will be transformed into a compound
  * statement with one procedure call per value parameter.
  *
  * There is also an implied FILE first parameter if none is specified.
  */
class expression_writeln:
    public expression
{
public:
    typedef boost::shared_ptr<expression_writeln> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_writeln();

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      *
      * @param value
      *     The value to be printed.
      * @param width
      *     The field width, or zero if not specified.
      */
    expression_writeln(const expression::pointer &value,
        const expression::pointer &width);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param value
      *     The value to be printed.
      * @param width
      *     The field width, or zero if not specified.
      * @param code
      *     The translator we are working with.
      * @note
      *     Could report an error for unprintable parameters, or dumb
      *     combinations of parameters.
      */
    static expression::pointer create(const expression::pointer &value,
        const expression::pointer &width, translator_compile *code);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param value
      *     The value to be printed.
      * @param width
      *     The field width, or zero if not specified.
      * @param precision
      *     The number of digits to the rigth of the decimal point, or
      *     zero if not specified.
      * @param code
      *     The translator we are working with.
      * @note
      *     Could report an error for unprintable parameters, or dumb
      *     combinations of parameters.
      */
    static expression::pointer create(const expression::pointer &value,
        const expression::pointer &width, const expression::pointer &precision,
        translator_compile *code);

    /**
      * The function_call_expression_factory is used to turn the
      * expression into a call of a system function to implement the
      * formatting.
      *
      * @param file
      *     Where to print the data.
      * @returns
      *     a new expression node.
      */
    virtual expression::pointer function_call_expression_factory(
        const expression::pointer &file) const = 0;

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

    expression::pointer get_value() const { return value; }
    expression::pointer get_width() const { return width; }

private:
    expression::pointer value;
    expression::pointer width;

    /**
      * The default constructor.  Do not use.
      */
    expression_writeln();

    /**
      * The copy constructor.  Do no tuse.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_writeln(const expression_writeln &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_writeln &operator=(const expression_writeln &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_WRITELN_H
// vim: set ts=8 sw=4 et :
