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

#ifndef LIB_STATEMENT_CASE_H
#define LIB_STATEMENT_CASE_H

#include <lib/case/list.h>
#include <lib/expression.h>
#include <lib/statement.h>

/**
  * The statement_case class is used to represent a case statement, and
  * all of its cases.
  */
class statement_case:
    public statement
{
public:
    typedef boost::shared_ptr<statement_case> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_case();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    statement_case(const expression::pointer &control,
        const case_list &cases, const statement::pointer &otherwise);

    int get_minimum(void) const { return cases.get_minimum(); }
    int get_maximum(void) const { return cases.get_maximum(); }
    const case_list &get_cases(void) const { return cases; }

protected:
    // See base class for documentation.
    bool execution_flows_out() const;

    expression::pointer get_control(void) const { return control; }

    statement::pointer
    get_case(int value)
        const
    {
        return cases.get_case(value);
    }

    statement::pointer get_otherwise(void) const { return otherwise; }

private:
    expression::pointer control;
    case_list cases;
    statement::pointer otherwise;

    /**
      * The default constructor.  Do not use.
      */
    statement_case();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_case(const statement_case &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_case &operator=(const statement_case &rhs);
};

#endif // LIB_STATEMENT_CASE_H
// vim: set ts=8 sw=4 et :
