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

#ifndef UCSDPSYS_PRETTY_STATEMENT_FOR_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_FOR_PRETTY_H

#include <lib/statement/for.h>

class translator_pretty; // forward

/**
  * The statement_for_pretty class is used to represent the processing
  * necessary to pretty-print a FOR statement.
  */
class statement_for_pretty:
    public statement_for
{
public:
    typedef boost::shared_ptr<statement_for_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_for_pretty();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    statement_for_pretty(const expression::pointer &var,
        const expression::pointer &limit1, bool descending,
        const expression::pointer &limit2, const statement::pointer &body,
        translator_pretty *pretty);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &var,
        const expression::pointer &limit1, bool descending,
        const expression::pointer &limit2, const statement::pointer &body,
        translator_pretty *pretty);

protected:
    // See base class for documentation.
    void traversal(void) const;

private:
    translator_pretty &pretty;

    /**
      * The default constructor.  Do not use.
      */
    statement_for_pretty();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_for_pretty(const statement_for_pretty &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_for_pretty &operator=(const statement_for_pretty &rhs);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_FOR_PRETTY_H
// vim: set ts=8 sw=4 et :
