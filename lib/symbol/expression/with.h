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

#ifndef LIB_SYMBOL_EXPRESSION_WITH_H
#define LIB_SYMBOL_EXPRESSION_WITH_H

#include <lib/symbol/expression.h>

/**
  * The symbol_expression_with class is used to represent a named
  * non-constant expression, used to implement the "with" shortcut
  * method of accessing record fields by name.
  */
class symbol_expression_with:
    public symbol_expression
{
public:
    typedef boost::shared_ptr<symbol_expression_with> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_expression_with();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    symbol_expression_with(const variable_name &name, int lex_level,
        const expression::pointer &value);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const variable_name &name, int lex_level,
        const expression::pointer &value);

protected:
    // See base class for documentation.
    const char *get_kind() const;

private:
    /**
      * The default constructor.  Do not use.
      */
    symbol_expression_with();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_expression_with(const symbol_expression_with &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_expression_with &operator=(const symbol_expression_with &rhs);
};

#endif // LIB_SYMBOL_EXPRESSION_WITH_H
// vim: set ts=8 sw=4 et :
