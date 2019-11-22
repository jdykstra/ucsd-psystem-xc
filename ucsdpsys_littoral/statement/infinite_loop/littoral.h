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

#ifndef UCSDPSYS_PRETTY_STATEMENT_INFINITE_LOOP_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_INFINITE_LOOP_PRETTY_H

#include <lib/statement/infinite_loop.h>

class translator_littoral; // forward

/**
  * The statement_infinite_loop_littoral class is used to represent littoral
  * printing an infinite loop.
  */
class statement_infinite_loop_littoral:
    public statement_infinite_loop
{
public:
    typedef boost::shared_ptr<statement_infinite_loop_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_infinite_loop_littoral();

private:
    /**
      * The constructor.
      */
    statement_infinite_loop_littoral(const statement::pointer &body,
        translator_littoral *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const statement::pointer &body,
        translator_littoral *cntxt);

protected:
    // See base class for docuemntation.
    void pre_order_traversal() const;

    // See base class for docuemntation.
    void post_order_traversal() const;

private:
    /**
      * The littoral instance variable is used to remember where to send
      * our beautified output.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_infinite_loop_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    statement_infinite_loop_littoral(const statement_infinite_loop_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_infinite_loop_littoral &operator=(
        const statement_infinite_loop_littoral &);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_INFINITE_LOOP_PRETTY_H
// vim: set ts=8 sw=4 et :
