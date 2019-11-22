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

#ifndef UCSDPSYS_COMPILE_STATEMENT_UNTIL_COMPILE_H
#define UCSDPSYS_COMPILE_STATEMENT_UNTIL_COMPILE_H

#include <lib/statement/until.h>

class translator_compile; // forward

/**
  * The statement_until_compile class is used to represent the code
  * generation necessary for a until statement.
  */
class statement_until_compile:
    public statement_until
{
public:
    typedef boost::shared_ptr<statement_until_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_until_compile();

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param body
      *     The statements that for the loop body.
      * @param condition
      *     The condition that controls the loop exit.
      * @param cntxt
      *     The translator context we are working within.
      */
    statement_until_compile(const statement_list &body,
        const expression::pointer &condition, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param body
      *     The statements that for the loop body.
      * @param condition
      *     The condition that controls the loop exit.
      * @param cntxt
      *     The translator context we are working within.
      */
    static pointer create(const statement_list &body,
        const expression::pointer &condition, translator_compile *cntxt);

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    statement_until_compile();

    /**
      * The copy constructor.  Do not use.
      */
    statement_until_compile(const statement_until_compile &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_until_compile &operator=(const statement_until_compile &);
};

#endif // UCSDPSYS_COMPILE_STATEMENT_UNTIL_COMPILE_H
// vim: set ts=8 sw=4 et :
