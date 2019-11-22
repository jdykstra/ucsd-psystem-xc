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

#ifndef UCSDPSYS_PRETTY_STATEMENT_WITH_H
#define UCSDPSYS_PRETTY_STATEMENT_WITH_H

#include <lib/expression.h>
#include <lib/statement.h>

class translator_littoral; // forward

/**
  * The statement_with class is used to represent a WITH statement to be
  * littoral printed.
  */
class statement_with:
    public statement
{
public:
    typedef boost::shared_ptr<statement_with> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_with();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    statement_with(const expression::pointer &ep, const statement::pointer &sp,
        translator_littoral *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &ep,
        const statement::pointer &sp, translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void traversal() const;

    // See base class for documentation.
    bool execution_flows_out() const;

private:
    expression::pointer ep;
    statement::pointer sp;
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_with();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_with(const statement_with &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_with &operator=(const statement_with &rhs);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_WITH_H
// vim: set ts=8 sw=4 et :
