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

#ifndef LIB_SYMBOL_EXPRESSION_CONSTANT_H
#define LIB_SYMBOL_EXPRESSION_CONSTANT_H

#include <lib/symbol/expression.h>

/**
  * The symbol_expression_constant class is used to represent a named constant
  * expression.
  */
class symbol_expression_constant:
    public symbol_expression
{
public:
    typedef boost::shared_ptr<symbol_expression_constant> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_expression_constant();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the symbol, including (implicit) declaration location.
      * @param lex_level
      *     The lexical level of the symbol.  Needed for p-code.
      * @param value
      *     The value of the symbol, or NULL if it doesn't have a value yet.
      */
    static pointer create(const variable_name &name, int lex_level,
        const expression::pointer &value);

protected:
    // See base class for documentation.
    const char *get_kind() const;

    /**
      * The constructor.
      * For use of derived classes, only.
      *
      * @param name
      *     The name of the symbol, including (implicit) declaration location.
      * @param lex_level
      *     The lexical level of the symbol.  Needed for p-code.
      * @param value
      *     The value of the symbol, or NULL if it doesn't have a value yet.
      */
    symbol_expression_constant(const variable_name &name, int lex_level,
        const expression::pointer &value);

private:
    /**
      * The default constructor.  Do not use.
      */
    symbol_expression_constant();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_expression_constant(const symbol_expression_constant &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_expression_constant &operator=(const symbol_expression_constant &);
};

#endif // LIB_SYMBOL_EXPRESSION_CONSTANT_H
// vim: set ts=8 sw=4 et :
