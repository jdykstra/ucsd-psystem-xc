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

#ifndef UCSDPSYS_PRETTY_STATEMENT_UNTIL_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_UNTIL_PRETTY_H

#include <lib/statement/until.h>

class translator_littoral; // forward

/**
  * The statement_until_littoral class is used to represent littoral
  * printing a until statement.
  */
class statement_until_littoral:
    public statement_until
{
public:
    typedef boost::shared_ptr<statement_until_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_until_littoral();

private:
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
    statement_until_littoral(const statement_list &body,
        const expression::pointer &condition, translator_littoral *cntxt);

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
        const expression::pointer &condition, translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal() const;

    // See base class for documentation.
    void until_order_traversal() const;

    // See base class for documentation.
    void post_order_traversal() const;

private:
    /**
      * The littoral instance variable is used to remember where to send
      * our littoral text.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_until_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    statement_until_littoral(const statement_until_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_until_littoral &operator=(const statement_until_littoral &);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_UNTIL_PRETTY_H
// vim: set ts=8 sw=4 et :
