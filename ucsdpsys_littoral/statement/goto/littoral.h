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

#ifndef UCSDPSYS_PRETTY_STATEMENT_GOTO_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_GOTO_PRETTY_H

#include <lib/statement/goto.h>

class translator_littoral; // forward

/**
  * The statement_goto_littoral class is used to represent the prcessing
  * necessary to littoral print a GOTO statement.
  */
class statement_goto_littoral:
    public statement_goto
{
public:
    typedef boost::shared_ptr<statement_goto_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_goto_littoral();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param sp
      *     The label symbol to jump to.
      * @param cntxt
      *     The translator context we are working within.
      */
    statement_goto_littoral(
        const symbol::pointer &sp, translator_littoral *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param sp
      *     The label symbol to jump to.
      * @param cntxt
      *     The translator context we are working within.
      */
    static pointer create(const symbol::pointer &sp,
        translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void post_order_traversal(const symbol::pointer &sp) const;

private:
    /**
      * The littoral instance variable is used to remember the translator
      * context we are working within.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_goto_littoral();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_goto_littoral(const statement_goto_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_goto_littoral &operator=(const statement_goto_littoral &rhs);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_GOTO_PRETTY_H
// vim: set ts=8 sw=4 et :
