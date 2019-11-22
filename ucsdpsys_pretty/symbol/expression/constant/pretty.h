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

#ifndef UCSDPSYS_PRETTY_SYMBOL_EXPRESSION_CONSTANT_PRETTY_H
#define UCSDPSYS_PRETTY_SYMBOL_EXPRESSION_CONSTANT_PRETTY_H

#include <lib/symbol/expression/constant.h>

class translator_pretty; // forward

/**
  * The symbol_expression_constant_pretty class is used to represent
  * a constant symbol to be pretty printed, when referenced.
  */
class symbol_expression_constant_pretty:
    public symbol_expression_constant
{
public:
    typedef boost::shared_ptr<symbol_expression_constant_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_expression_constant_pretty();

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
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const variable_name &name, int lex_level,
        const expression::pointer &value, translator_pretty *context);

protected:
    // See base class for documentation.
    expression::pointer name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     The name of the symbol, including (implicit) declaration location.
      * @param lex_level
      *     The lexical level of the symbol.  Needed for p-code.
      * @param value
      *     The value of the symbol, or NULL if it doesn't have a value yet.
      * @param context
      *     The translation context we are working within.
      */
    symbol_expression_constant_pretty(const variable_name &name, int lex_level,
        const expression::pointer &value, translator_pretty *context);

    /**
      * The context instance variable is used to remember
      * the translation context we are working within.
      */
    translator_pretty *context;

    /**
      * The default constructor.  Do not use.
      */
    symbol_expression_constant_pretty();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_expression_constant_pretty(
        const symbol_expression_constant_pretty &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_expression_constant_pretty &operator=(
        const symbol_expression_constant_pretty &rhs);
};

#endif // UCSDPSYS_PRETTY_SYMBOL_EXPRESSION_CONSTANT_PRETTY_H
// vim: set ts=8 sw=4 et :
