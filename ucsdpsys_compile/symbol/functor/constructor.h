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

#ifndef UCSDPSYS_COMPILE_SYMBOL_FUNCTOR_CONSTRUCTOR_H
#define UCSDPSYS_COMPILE_SYMBOL_FUNCTOR_CONSTRUCTOR_H

#include <lib/symbol/functor.h>

class scope; // forward
class translator_compile; // forward

/**
  * The symbol_functor_constructor class is used to represent the code
  * generation necessary to initialize a variable.
  */
class symbol_functor_constructor:
    public symbol_functor
{
public:
    /**
      * The destructor.
      */
    virtual ~symbol_functor_constructor();

    /**
      * The constructor.
      */
    symbol_functor_constructor(translator_compile &code, const scope &fscope);

protected:
    // See base class for documentation.
    void operator()(const symbol::pointer &sp);

private:
    /**
      * The code instance variable is used to reemember where to send
      * our generated code.
      */
    translator_compile &code;

    /**
      * The fscope instance variable is used to remember the code the
      * variables are in, in case you need to lookup anything.
      */
    const scope &fscope;

    /**
      * The default constructor.  Do not use.
      */
    symbol_functor_constructor();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_functor_constructor(const symbol_functor_constructor &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_functor_constructor &operator=(const symbol_functor_constructor &);
};

#endif // UCSDPSYS_COMPILE_SYMBOL_FUNCTOR_CONSTRUCTOR_H
// vim: set ts=8 sw=4 et :
