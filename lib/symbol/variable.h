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

#ifndef LIB_SYMBOL_VARIABLE_H
#define LIB_SYMBOL_VARIABLE_H

#include <lib/symbol.h>
#include <lib/variable/name.h>
#include <lib/variable/name/list.h>

/**
  * The symbol_variable class is used to represent a user-defined
  * variable.
  */
class symbol_variable:
    public symbol
{
public:
    typedef boost::shared_ptr<symbol_variable> pointer;

    enum kind_t
    {
        kind_variable,
        kind_parameter,
        kind_record_field
    };

    /**
      * The destructor.
      */
    virtual ~symbol_variable();

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
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
    symbol_variable(const variable_name &name, const type::pointer &tp,
        int lex_level, kind_t kind);

    // See base class for documentation.
    const char *get_kind(void) const;

private:
    /**
      * The definition instance variable is used to remember where this
      * symbol was defined.
      */
    variable_name definition;

    /**
      * The references instance variable is used to remember where this
      * symbol was referred to in the code.
      */
    variable_name_list references;

    kind_t kind;

    /**
      * The default constructor.
      */
    symbol_variable();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_variable(const symbol_variable &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_variable &operator=(const symbol_variable &);
};

#endif // LIB_SYMBOL_VARIABLE_H
// vim: set ts=8 sw=4 et :
