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

#ifndef LIB_SYMBOL_PROGRAM_H
#define LIB_SYMBOL_PROGRAM_H

#include <lib/symbol.h>
#include <lib/scope.h>

/**
  * The symbol_program class is used to represent a symbol refering to
  * the whole program being compiled.
  */
class symbol_program:
    public symbol
{
public:
    typedef boost::shared_ptr<symbol_program> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_program();

private:
    /**
      * The constructor.
      */
    symbol_program(const rcstring &name, const scope::pointer &sp,
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
    const char *get_kind() const;

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The sp instance variable is used to remember the program
      * instance being compiled.
      */
    scope::pointer sp;

    /**
      * The default constructor.  Do not use.
      */
    symbol_program();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_program(const symbol_program &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_program &operator=(const symbol_program &);
};

#endif // LIB_SYMBOL_PROGRAM_H
// vim: set ts=8 sw=4 et :
