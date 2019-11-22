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

#ifndef LIB_STATEMENT_H
#define LIB_STATEMENT_H

#include <boost/shared_ptr.hpp>

#include <lib/location.h>

/**
  * The abstract statement base class is used to represent an arbitrary
  * statement.  Instances of this class are usually managed by
  * statement::pointer instances.
  */
class statement
{
public:
    /**
      * The pointer type is to be used for all pointers to statements.
      */
    typedef boost::shared_ptr<statement> pointer;

    /**
      * The destructor.
      */
    virtual ~statement();

public:
    /**
      * The traversal method may be used to traverse each of the
      * statements in the list.
      */
    virtual void traversal(void) const = 0;

    /**
      * The empty() method is used to test for the empty statement.  The
      * default implementation returns false (obviously).
      */
    virtual bool empty(void) const;

    /**
      * The is_compound() method is used to test for the compund statement.
      * The default implementation returns false (obviously).
      */
    virtual bool is_compound(void) const;

    /**
      * The get_location method may be used to get the location of this
      * statement.
      */
    const location &get_location(void) const { return locn; }

    /**
      * The execution_flows_out method may be used to determine whether
      * the flow of execution, having entered this statement, can ever
      * flow out again.
      *
      * @returns
      *     true if exection flows out, false if it does not.
      * @note
      *     Where the answer is indeterminate, true is returned.
      */
    virtual bool execution_flows_out(void) const = 0;

    /**
      * The execution_flows_in method may be used to determine whether
      * the flow of execution is able to enter at this point,
      * for example at a label.
      *
      * @returns
      *     true if exection flows in, false if it simply flows in from
      *     the statement before.
      */
    virtual bool execution_flows_in(void) const;

protected:
    /**
      * The default constructor.
      * Only derived classes may use this constructor.
      */
    statement();

    /**
      * The copy constructor.  Do not use.
      */
    statement(const statement &);

    /**
      * The assignment operator.  Do not use.
      */
    statement &operator=(const statement &);

    /**
      * The set_location method may be used by derived classes to
      * set a more accurate (usually the beginning) location for the
      * statement.
      */
    void set_location(const location &arg);

private:
    /**
      * The locn instance variable is used to remember where this
      * statement is located within the source code.  This makes for
      * better error messages.
      *
      * The default location is the location of the most recent lex
      * token, which is usually the end of the steatement.  Derived
      * classes are encouraged to set the location to something better
      * in their constructors.
      */
    location locn;
};

#endif // LIB_STATEMENT_H
// vim: set ts=8 sw=4 et :
