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

#ifndef LIB_STATEMENT_EMPTY_H
#define LIB_STATEMENT_EMPTY_H

#include <lib/statement.h>

/**
  * The statement_empty class is used to represent a statement which
  * does absolutely nothing.
  */
class statement_empty:
    public statement
{
public:
    typedef boost::shared_ptr<statement_empty> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_empty();

protected:
    /**
      * The default constructor.
      */
    statement_empty();

public:
    static pointer create();

protected:
    // See base class for documentation.
    void traversal() const;

    // See base class for documentation.
    bool empty() const;

    // See base class for documentation.
    bool execution_flows_out() const;

private:
    /**
      * The copy constructor.  Do not use.
      */
    statement_empty(const statement_empty &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_empty &operator=(const statement_empty &);
};

#endif // LIB_STATEMENT_EMPTY_H
// vim: set ts=8 sw=4 et :
