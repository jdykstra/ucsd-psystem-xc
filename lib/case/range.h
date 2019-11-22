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

#ifndef LIB_CASE_RANGE_H
#define LIB_CASE_RANGE_H

#include <lib/ac/map>

#include <lib/expression.h>
#include <lib/statement.h>

class case_list; // forward

/**
  * The case_range class is used to represent a range of values to the
  * left of the colon in a case statement item.
  */
class case_range
{
public:
    typedef signed short word_t;

    /**
      * The destructor.
      */
    virtual ~case_range();

    /**
      * The constructor.
      */
    case_range(const type::pointer &base);

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    case_range(const case_range &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    case_range &operator=(const case_range &rhs);

    /**
      * The push_back method is used to add another constant expression
      * to the set of case values.
      * It will cause errors if duplicates are found.
      *
      * @param ep
      *     The constant expression of interest.
      */
    void push_back(const expression::pointer &ep);

    /**
      * The push_back method is used to add another constant expression
      * to the set of case values.  An error message will be issued if
      * duplicates are found.
      *
      * @param lo_ep
      *     The start of the constant expression of interest.
      * @param hi_ep
      *     The start of the constant expression of interest.
      */
    void push_back(const expression::pointer &lo_ep,
        const expression::pointer &hi_ep);

    void push_into_list(case_list &dest, const statement::pointer &sp) const;

    /**
      * The addition-in-place operator may be used to merge two
      * case_range values.  It will cause errors if duplicates are
      * found.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    void operator+=(const case_range &rhs);

    /**
      * The addition operator may be used to merge two case_range
      * values.  It will cause errors if duplicates are found.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    case_range operator+(const case_range &rhs) const;

    word_t get_minimum(void) const { return minimum; }
    word_t get_maximum(void) const { return maximum; }
    bool empty(void) const { return values.empty(); }

private:
    /**
      * The base instance variable is used to remember the type of these
      * values.  It is used to produce better error messages.
      */
    type::pointer base;

    /**
      * The minimum instance variable is used to remember the lowest
      * case value observed.
      */
    word_t minimum;

    /**
      * The maximum instance variable is used to remember the highest
      * case value observed.
      */
    word_t maximum;

    typedef std::map<word_t, expression::pointer> values_t;

    /**
      * The value instance variable is used to remember the mapping
      * between values and the expressions that provided them.
      */
    values_t values;

    /**
      * The push_back method is used to insert a specific value and
      * expression into the mapping.  This is intentionally not part of
      * the public interface.
      *
      * @param value
      *     The value to be indexed
      * @param ep
      *     The constant expression that (possibly very loosely) defined
      *     the value.
      */
    void push_back(word_t value, const expression::pointer &ep);

    /**
      * The default constructor.  Do not use.
      */
    case_range();
};

#endif // LIB_CASE_RANGE_H
// vim: set ts=8 sw=4 et :
