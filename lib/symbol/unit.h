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

#ifndef LIB_SYMBOL_UNIT_H
#define LIB_SYMBOL_UNIT_H

#include <lib/symbol.h>
#include <lib/scope.h>

/**
  * The symbol_unit class is used to represent a symbol refering to
  * the whole unit being compiled.
  */
class symbol_unit:
    public symbol
{
public:
    typedef boost::shared_ptr<symbol_unit> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_unit();

private:
    /**
      * The constructor.
      */
    symbol_unit(const rcstring &name, const scope::pointer &sp,
        int lex_level);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const rcstring &name, const scope::pointer &sp,
        int lex_level);

protected:
    // See base class for documentation.
    const char *get_kind(void) const;

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The sp instance variable is used to remember the unit
      * instance being compiled.
      */
    scope::pointer sp;

    /**
      * The default constructor.  Do not use.
      */
    symbol_unit();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_unit(const symbol_unit &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_unit &operator=(const symbol_unit &);
};

#endif // LIB_SYMBOL_UNIT_H
// vim: set ts=8 sw=4 et :
