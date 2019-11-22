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

#ifndef LIB_STATEMENT_UNTIL_H
#define LIB_STATEMENT_UNTIL_H

#include <lib/expression.h>
#include <lib/statement.h>
#include <lib/statement/list.h>

class translator; // forward

/**
  * The statement_until class is used to represent an abstract WHILE
  * loop statement.
  */
class statement_until:
    public statement
{
public:
    /**
      * The destructor.
      */
    virtual ~statement_until();

    /**
      * The constructor.
      *
      * @param body
      *     The statements that for the loop body.
      * @param condition
      *     The condition that controls the loop exit.
      * @param cntxt
      *     The translator context we are working within.
      */
    statement_until(const statement_list &body,
        const expression::pointer &condition, translator *cntxt);

protected:
    // See base class for documentation.
    void traversal(void) const;

    // See base class for documentation.
    bool execution_flows_out(void) const;

    // See base class for documentation.
    bool execution_flows_in(void) const;

protected:
    /**
      * The pre_order_traversal method is called by the traversal method
      * immediately before the body statement is traversed.
      */
    virtual void pre_order_traversal(void) const;

    /**
      * The until_order_traversal method is called by the traversal
      * method after the body statement is traversed and before the
      * condition expression is traversed.
      */
    virtual void until_order_traversal(void) const;

    /**
      * The post_order_traversal method is called by the traversal
      * method after the condition expression is traversed.
      */
    virtual void post_order_traversal(void) const;

private:
    /**
      * The xlat instance variable is used to remember where to get
      * labels from.
      */
    translator &xlat;

    /**
      * The body instance variable is used to remember the body of the
      * loop to be executed.
      */
    statement_list body;

    /**
      * The condition instance variable is used to remember the boolean
      * condition controling the loop repetition.
      */
    expression::pointer condition;

    /**
      * The default constructor.  Do not use.
      */
    statement_until();

    /**
      * The copy constructor.  Do not use.
      */
    statement_until(const statement_until &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_until &operator=(const statement_until &);
};

#endif // LIB_STATEMENT_UNTIL_H
// vim: set ts=8 sw=4 et :
