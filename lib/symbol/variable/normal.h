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

#ifndef LIB_SYMBOL_VARIABLE_NORMAL_H
#define LIB_SYMBOL_VARIABLE_NORMAL_H

#include <lib/symbol/variable.h>

/**
  * The symbol_variable_normal class is used to represent a simple
  * user-defined variable, one that exists within a scope and requires
  * no linking or special opcodes to access.
  */
class symbol_variable_normal:
    public symbol_variable
{
public:
    typedef boost::shared_ptr<symbol_variable_normal> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_variable_normal();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param name
      *     The name of the variable
      * @param tp
      *     The type of the variable
      * @param lex_level
      *     The lexical level of the variable
      * @param kind
      *     The kind of the variable
      */
    symbol_variable_normal(const variable_name &name, const type::pointer &tp,
        int lex_level, kind_t kind);

private:
    /**
      * The default constructor.  Do not use.
      */
    symbol_variable_normal();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_variable_normal(const symbol_variable_normal &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_variable_normal &operator=(const symbol_variable_normal &rhs);
};

#endif // LIB_SYMBOL_VARIABLE_NORMAL_H
// vim: set ts=8 sw=4 et :
