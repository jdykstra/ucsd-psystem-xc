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

#ifndef LIB_SYMBOL_ENUMERATED_H
#define LIB_SYMBOL_ENUMERATED_H

#include <lib/symbol.h>

/**
  * The symbol_enumerated class is used to represent a member of an
  * enumerated set of values.
  */
class symbol_enumerated:
    public symbol
{
public:
    typedef boost::shared_ptr<symbol_enumerated> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_enumerated();

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
      *
      * @param name
      *     The name of this enumeration tag
      * @param tp
      *     The type of this enumeration tag
      * @param lex_level
      *     The scope nesting level of the symbol
      * @param value
      *     The value of this enumeration tag
      */
    symbol_enumerated(const rcstring &name, const type::pointer &tp,
        int lex_level, int value);

    /**
      * The constructor.
      *
      * @param name
      *     The name of this enumeration tag
      * @param tp
      *     The type of this enumeration tag
      * @param lex_level
      *     The scope nesting level of the symbol
      * @param value
      *     The value of this enumeration tag
      */
    symbol_enumerated(const variable_name &name, const type::pointer &tp,
        int lex_level, int value);

public:
    /**
      * The get_value method is used to abtain the value of this
      * enumeration tag.
      */
    int get_value() const { return value; }

protected:
    // See base class for documentation.
    const char *get_kind() const;

private:
    /**
      * The value instance variable is used to remember the value of
      * this enumeration tag.
      */
    int value;

    /**
      * The default constructor.
      */
    symbol_enumerated();

    /**
      * The copy constructor.
      */
    symbol_enumerated(const symbol_enumerated &);

    /**
      * The assignment operator.
      */
    symbol_enumerated &operator=(const symbol_enumerated &);
};

#endif // LIB_SYMBOL_ENUMERATED_H
// vim: set ts=8 sw=4 et :
