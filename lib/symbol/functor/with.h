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

#ifndef LIB_SYMBOL_FUNCTOR_WITH_H
#define LIB_SYMBOL_FUNCTOR_WITH_H

#include <lib/symbol/functor.h>

class scope_stack; // forward
class location; // forward

/**
  * The symbol_functor_with class is used to represent the symbol table
  * operations necessary to check record fields in a WITH scope, making
  * sure that earlier symbols are not shadowed.
  */
class symbol_functor_with:
    public symbol_functor
{
public:
    typedef boost::shared_ptr<symbol_functor_with> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_functor_with();

    /**
      * The constructor.
      *
      * @param scopes
      *     where the scope stack is located.
      * @param locn
      *     where the WITH base expression appears in the source
      */
    symbol_functor_with(scope_stack &scopes, const location &locn);

    // See base class for documentation.
    void operator()(const symbol::pointer &sp);

private:
    /**
      * The scopes instance variable is used to remember where the scope
      * stack is located.
      */
    scope_stack &scopes;

    /**
      * The locn instance variable is used to remember where the WITH
      * base expression appears in the source.
      */
    const location &locn;

    /**
      * The default constructor.  Do not use.
      */
    symbol_functor_with();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_functor_with(const symbol_functor_with &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_functor_with &operator=(const symbol_functor_with &rhs);
};

#endif // LIB_SYMBOL_FUNCTOR_WITH_H
// vim: set ts=8 sw=4 et :
