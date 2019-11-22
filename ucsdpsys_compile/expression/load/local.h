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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LOAD_LOCAL_H
#define UCSDPSYS_COMPILE_EXPRESSION_LOAD_LOCAL_H

#include <ucsdpsys_compile/expression/load.h>

class translator_compile; // forward

/**
  * The expression_load_local class is used to represent loading the
  * contents of a local variable onto the stack.
  */
class expression_load_local:
    public expression_load
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_load_local();

    /**
      * The constructor.
      */
    expression_load_local(const location &locn, const type::pointer &type,
        int offset, translator_compile *cntxt);

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The offset instance variable is used to remember the word offset
      * into the local scope.
      */
    int offset;

    /**
      * The default constructor.  Do not use.
      */
    expression_load_local();

    /**
      * The copy constructor.  Do not use.
      */
    expression_load_local(const expression_load_local &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_load_local &operator=(const expression_load_local &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LOAD_LOCAL_H
// vim: set ts=8 sw=4 et :
