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

#ifndef LIB_STATEMENT_FOR_H
#define LIB_STATEMENT_FOR_H

#include <lib/expression.h>
#include <lib/statement.h>

/**
  * The statement_for class is used to represent the elements that make
  * up a FOR statement.
  */
class statement_for:
    public statement
{
public:
    typedef boost::shared_ptr<statement_for> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_for();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    statement_for(const expression::pointer &var,
        const expression::pointer &limit1, bool descending,
        const expression::pointer &limit2, const statement::pointer &body);

protected:
    // See base class for documentation.
    bool execution_flows_out() const;

    expression::pointer get_var(void) const { return var; }
    expression::pointer get_limit1(void) const { return limit1; }
    bool is_descending(void) const { return descending; }
    expression::pointer get_limit2(void) const { return limit2; }
    statement::pointer get_body(void) const { return body; }

private:
    expression::pointer var;
    expression::pointer limit1;
    bool descending;
    expression::pointer limit2;
    statement::pointer body;

    /**
      * The default constructor.  Do not use.
      */
    statement_for();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_for(const statement_for &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_for &operator=(const statement_for &rhs);
};

#endif // LIB_STATEMENT_FOR_H
// vim: set ts=8 sw=4 et :
