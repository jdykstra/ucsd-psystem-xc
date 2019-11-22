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

#ifndef LIB_CASE_LIST_H
#define LIB_CASE_LIST_H

#include <lib/ac/map>

#include <lib/expression.h>
#include <lib/statement.h>

/**
  * The case_list class is used to represent the case values and
  * statements within a case statement.
  */
class case_list
{
public:
    typedef signed short word_t;

    /**
      * The destructor.
      */
    virtual ~case_list();

    /**
      * The constructor.
      */
    case_list(const type::pointer &base);

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    case_list(const case_list &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    case_list &operator=(const case_list &rhs);

    void push_back(word_t value, const expression::pointer &ep,
        const statement::pointer &sp);

    void push_back(const case_list &rhs);

    bool execution_flows_out(void) const;

    word_t get_minimum(void) const { return minimum; }
    word_t get_maximum(void) const { return maximum; }
    bool empty(void) const { return values.empty(); }

    statement::pointer get_case(word_t value) const;

private:
    type::pointer base;
    word_t minimum;
    word_t maximum;

    struct mapping
    {
        mapping() { }

        mapping(
            const expression::pointer &a_ep,
            const statement::pointer &a_sp
        ) :
            ep(a_ep),
            sp(a_sp)
        {
        }

        mapping(const mapping &rhs) :
            ep(rhs.ep),
            sp(rhs.sp)
        {
        }

        mapping &
        operator=(const mapping &rhs)
        {
            if (this != &rhs)
            {
                ep = rhs.ep;
                sp = rhs.sp;
            }
            return *this;
        }

        expression::pointer ep;
        statement::pointer sp;
    };

    typedef std::map<word_t, mapping> values_t;

    /**
      * The values instance variable is used to remember the mapping
      * from a constant expression (value) to a statement to be executed
      * when that value is seen.
      */
    values_t values;

    /**
      * The default constructor.  Do not use.
      */
    case_list();
};

#endif // LIB_CASE_LIST_H
// vim: set ts=8 sw=4 et :
