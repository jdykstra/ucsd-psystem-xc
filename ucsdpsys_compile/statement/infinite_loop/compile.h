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

#ifndef UCSDPSYS_COMPILE_STATEMENT_INFINITE_LOOP_COMPILE_H
#define UCSDPSYS_COMPILE_STATEMENT_INFINITE_LOOP_COMPILE_H

#include <lib/statement/infinite_loop.h>

class translator_compile; // forward

/**
  * The statement_infinite_loop_compile class is used to represent
  * generate the code for infinite loops.
  */
class statement_infinite_loop_compile:
    public statement_infinite_loop
{
public:
    typedef boost::shared_ptr<statement_infinite_loop_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_infinite_loop_compile();

private:
    /**
      * The constructor.
      */
    statement_infinite_loop_compile(const statement::pointer &body,
        translator_compile *cntxt);

public:
    static pointer create(const statement::pointer &body,
        translator_compile *cntxt);

private:
    /**
      * The default constructor.  Do not use.
      */
    statement_infinite_loop_compile();

    /**
      * The copy constructor.  Do not use.
      */
    statement_infinite_loop_compile(const statement_infinite_loop_compile &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_infinite_loop_compile &operator=(
        const statement_infinite_loop_compile &);
};

#endif // UCSDPSYS_COMPILE_STATEMENT_INFINITE_LOOP_COMPILE_H
// vim: set ts=8 sw=4 et :
