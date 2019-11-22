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

#ifndef LIB_STATEMENT_WHILE_H
#define LIB_STATEMENT_WHILE_H

#include <lib/expression.h>
#include <lib/statement.h>

class translator; // forward

/**
  * The statement_while class is used to represent an abstract WHILE
  * loop statement.
  */
class statement_while:
    public statement
{
public:
    /**
      * The destructor.
      */
    virtual ~statement_while();

    /**
      * The constructor.
      */
    statement_while(const expression::pointer &condition,
        const statement::pointer &body, translator *cntxt);

    // See base class for documentation.
    void traversal() const;

    // See base class for documentation.
    bool execution_flows_out() const;

protected:
    /**
      * The pre_order_traversal method is called by the traversal method
      * immediately before the control expression is traversed.
      */
    virtual void pre_order_traversal() const;

    /**
      * The do_order_traversal method is called by the traversal method
      * after the control expression is traversed and before the body is
      * traversed.
      */
    virtual void do_order_traversal() const;

    /**
      * The post_order_traversal method is called by the traversal
      * method after the body is traversed.
      */
    virtual void post_order_traversal() const;

    statement::pointer get_body(void) const { return body; }

private:
    /**
      * The xlat instance variable is used to remember where to get
      * labels from.
      */
    translator &xlat;

    /**
      * The condition instance variable is used to remember the boolean
      * condition controling the loop repetition.
      */
    expression::pointer condition;

    /**
      * The body instance variable is used to remember the body of the
      * loop to be executed.
      */
    statement::pointer body;

    /**
      * The default constructor.  Do not use.
      */
    statement_while();

    /**
      * The copy constructor.  Do not use.
      */
    statement_while(const statement_while &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_while &operator=(const statement_while &);
};

#endif // LIB_STATEMENT_WHILE_H
// vim: set ts=8 sw=4 et :
