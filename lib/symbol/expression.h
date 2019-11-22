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

#ifndef LIB_SYMBOL_EXPRESSION_H
#define LIB_SYMBOL_EXPRESSION_H

#include <lib/expression.h>
#include <lib/symbol.h>

/**
  * The symbol_expression class is used to represent a symbol who's
  * value is an expression; a named expression.  The expression may be a
  * constant, or it could be come kind of macro, etc.
  */
class symbol_expression:
    public symbol
{
public:
    typedef boost::shared_ptr<symbol_expression> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_expression();

    /**
      * The get_value method is used to obtain the value of the contant.
      */
    expression::pointer get_value(void) const { return value; }

protected:
    /**
      * A constructor.
      * For use by derived classes only.
      *
      * @param name
      *     The name of the symbol, including (implicit) declaration location.
      * @param lex_level
      *     The lexical level of the symbol.  Needed for p-code.
      * @param value
      *     The value of the symbol, or NULL if it doesn't have a value yet.
      */
    symbol_expression(const variable_name &name, int lex_level,
        const expression::pointer &value);

    /**
      * A constructor.
      * For use by derived classes only.
      *
      * @param name
      *     The name of the symbol, including (implicit) declaration location.
      * @param type
      *     The type of the expression, particularly useful for NULL values.
      * @param lex_level
      *     The lexical level of the symbol.  Needed for p-code.
      * @param value
      *     The value of the symbol (in which case the type must match),
      *     or NULL if it doesn't have a value yet.
      */
    symbol_expression(const variable_name &name, const type::pointer &type,
        int lex_level, const expression::pointer &value);

    /**
      * The set_value method may be used to alter the value attached to
      * the symbol.
      *
      * @param value
      *     The new value to be attached (type must match).
      */
    void set_value(const expression::pointer &value);

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The value instance variable is used to remember the value of the
      * named expression.  It is not necessarily a constant.
      */
    expression::pointer value;

    /**
      * The default constructor.  Do not use.
      */
    symbol_expression();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_expression(const symbol_expression &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_expression &operator=(const symbol_expression &rhs);
};

#endif // LIB_SYMBOL_EXPRESSION_H
// vim: set ts=8 sw=4 et :
