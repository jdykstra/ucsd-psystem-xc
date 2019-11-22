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

#ifndef LIB_SCOPE_WITH_H
#define LIB_SCOPE_WITH_H

#include <lib/bit_address.h>
#include <lib/scope/temporary.h>
#include <lib/statement.h>
#include <lib/type/record.h>

class translator; // forward

/**
  * The scope_temporary_with class is used to represent the members of a
  * record that are exposed by a WITH statement.
  */
class scope_temporary_with:
    public scope_temporary
{
public:
    typedef boost::shared_ptr<scope_temporary_with> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_temporary_with();

    /**
      * The get_base_assignment method is used to obtain the base
      * expression from the start of the WITH statement, or NULL if it
      * was not required.
      */
    statement::pointer
    get_base_assignment(void)
        const
    {
        return base_assignment;
    }

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param stk
      *     The scope stack used to allocate and de-allocate.
      * @param base
      *     The implied lhs for the direct field references
      * @param base_assignment
      *     The statement that builds the base value, or NULL if none required.
      * @param context
      *     the translator context we are working within
      */
    scope_temporary_with(scope_stack &stk, const expression::pointer &base,
        const statement::pointer &base_assignment, translator *context);

    // See base class for documentation.
    symbol::pointer lookup(const rcstring &qname) const;

    // See base class for documentation.
    rcstring lookup_fuzzy(const rcstring &qname, double &best) const;

private:
    /**
      * The base instance variable is sused to remember the implied
      * leaf-hand-side of dot (.) for direct field references.
      *
      * Note that this could also be an expression_error instance if
      * they screwed up the expression in the with statement.
      */
    expression::pointer base;

    /**
      * The trp instance vaiable is used to remember the record type of
      * this scope.  It will be NULL if base is an expression_error.
      */
    type_record::pointer trp;

    /**
      * The base_assignment instance variable is used to remember the
      * statement that builds the base value, or NULL if none required.
      */
    statement::pointer base_assignment;

    /**
      * The context instance variable is used to remember the translator
      * context we are working within.  This is used to build the
      * implicit dot (.) expressions.
      */
    translator *context;

    /**
      * The default constructor.  Do not use.
      */
    scope_temporary_with();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_temporary_with(const scope_temporary_with &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_temporary_with &operator=(const scope_temporary_with &rhs);
};

#endif // LIB_SCOPE_WITH_H
// vim: set ts=8 sw=4 et :
