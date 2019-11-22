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

#ifndef LIB_SYMBOL_TYPEDEF_H
#define LIB_SYMBOL_TYPEDEF_H

#include <lib/symbol.h>

/**
  * The symbol_typedef class is used to represent a named type.
  */
class symbol_typedef:
    public symbol
{
public:
    typedef boost::shared_ptr<symbol_typedef> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_typedef();

private:
    /**
      * The constructor.
      */
    symbol_typedef(const variable_name &name, const type::pointer &tp,
        int lex_level);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const variable_name &name, const type::pointer &tp,
        int lex_level);

protected:
    // See base class for documentation.
    const char *get_kind() const;

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const location &locn,
        int lxlvl);

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The default constructor.  Do not use.
      */
    symbol_typedef();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_typedef(const symbol_typedef &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_typedef &operator=(const symbol_typedef &);
};

#endif // LIB_SYMBOL_TYPEDEF_H
// vim: set ts=8 sw=4 et :
