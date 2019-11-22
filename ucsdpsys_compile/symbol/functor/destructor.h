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

#ifndef UCSDPSYS_COMPILE_SYMBOL_FUNCTOR_DESTRUCTOR_H
#define UCSDPSYS_COMPILE_SYMBOL_FUNCTOR_DESTRUCTOR_H

#include <lib/symbol/functor.h>

class translator_compile; // forward

/**
  * The symbol_functor_destructor class is used to represent the code
  * necessary to tear down a symbol.
  */
class symbol_functor_destructor:
    public symbol_functor
{
public:
    /**
      * The destructor.
      */
    virtual ~symbol_functor_destructor();

    /**
      * The constructor.
      */
    symbol_functor_destructor(const location &locn, translator_compile &code);

protected:
    // See base class for documentation
    void operator()(const symbol::pointer &sp);

private:
    /**
      * The locn instance variable is used to remember where we are in
      * the source code.  Used to anchor the code in the listing.
      */
    location locn;

    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    symbol_functor_destructor();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_functor_destructor(const symbol_functor_destructor &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_functor_destructor &operator=(const symbol_functor_destructor &);
};

#endif // UCSDPSYS_COMPILE_SYMBOL_FUNCTOR_DESTRUCTOR_H
// vim: set ts=8 sw=4 et :
