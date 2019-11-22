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

#ifndef LIB_STATEMENT_INFINITE_LOOP_H
#define LIB_STATEMENT_INFINITE_LOOP_H

#include <lib/statement.h>

class translator; // forward

/**
  * The statement_infinite_loop class is used to represent and abstract
  * infinite loop.
  */
class statement_infinite_loop:
    public statement
{
public:
    /**
      * The destructor.
      */
    virtual ~statement_infinite_loop();

    /**
      * The constructor.
      */
    statement_infinite_loop(const statement::pointer &body, translator *cntxt);

    // See base class for documentation.
    void traversal() const;

    // See base class for documentation.
    bool execution_flows_out() const;

protected:
    /**
      * The pre_order_traversal method is called by the traversal method
      * before the body is traversed.
      */
    virtual void pre_order_traversal() const;

    /**
      * The post_order_traversal method is called by the traversal
      * method after the body is traversed.
      */
    virtual void post_order_traversal() const;

    statement::pointer get_body() const { return body; }

private:
    /**
      * The xlat instance variable is used to remember where to get
      * labels from.
      */
    translator &xlat;

    /**
      * The body instance variable is used to remember the statement to
      * repeatedly execute.
      */
    statement::pointer body;

    /**
      * The default constructor.  Do not use.
      */
    statement_infinite_loop();

    /**
      * The copy constructor.  Do not use.
      */
    statement_infinite_loop(const statement_infinite_loop &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_infinite_loop &operator=(const statement_infinite_loop &);
};

#endif // LIB_STATEMENT_INFINITE_LOOP_H
// vim: set ts=8 sw=4 et :
