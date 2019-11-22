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

#ifndef LIB_STATEMENT_LIST_H
#define LIB_STATEMENT_LIST_H

#include <lib/ac/stddef.h>

#include <lib/statement.h>

/**
  * The statement_list class is used to represent an ordered collection
  * of statements.
  */
class statement_list
{
public:
    /**
      * The destructor.
      */
    virtual ~statement_list();

    /**
      * The default constructor.
      */
    statement_list();

    /**
      * The copy constructor.
      */
    statement_list(const statement_list &);

    /**
      * The assignment operator.
      */
    statement_list &operator=(const statement_list &);

    /**
      * The push_back method is used to add another statement to the
      * end of the list.
      */
    void push_back(const statement::pointer &ssp);

    /**
      * The traversal method may be used to traverse each of the
      * declaractions in the list.
      */
    void traversal(void) const;

    /**
      * The trim method is used to remove redundant empty statements
      * from the list.  We don't need them for code generation, end we
      * don't need them for pretty printing.
      */
    void trim(void);

    /**
      * The get_location method is used to get the location of the of
      * the statement list.  Note that it will be empty if the statement
      * list is empty.
      */
    location get_location(void) const;

    /**
      * The execution_flows_out method may be used to determine whether
      * the flow of execution, having entered this statement, can ever
      * flow out again.
      *
      * @returns
      *     true if exection flows out, false if it does not.
      * @note
      *     Where the answer is indeterminate, true is returned.
      */
    bool execution_flows_out(void) const;

    /**
      * The execution_flows_in method may be used to determine whether
      * the flow of execution, enteres this stament from an unusual
      * place (e.g. a label).
      *
      * @returns
      *     true if exection flows in, false if it simplyt enters from
      *     the preceeding statement.
      */
    bool execution_flows_in(void) const;

    /**
      * The size method may be used to obtain the number of stetemnts in
      * this list.
      */
    size_t size(void) const { return length; }

    /**
      * The empty method may be used to
      * determine whether or no there are items in this list.
      *
      * @returns
      *     boolean; true if there are no statements in the list, false
      *     if there is at least opne statement in the list.
      */
    bool empty(void) const { return (length == 0); }

    /**
      * The get method may be used to obtain a particular item from the list.
      *
      * @param n
      *     The indes of the item to get, zero based.
      */
    statement::pointer get(size_t n) const;

    /**
      * The array index operator may be used to obtain a particular item
      * from the list.
      *
      * @param n
      *     The indes of the item to get, zero based.
      */
    statement::pointer operator[](size_t n) const { return get(n); }

private:
    /**
      * The length instance variable is used to remember how many
      * elements in the \a item array have been used to date.
      */
    size_t length;

    /**
      * The maximum instance variable is used to remember how many
      * elements were allocated in the \a item array.
      */
    size_t maximum;

    /**
      * The item instance variable is used to remember the address of
      * the base of an array of pointers to statemenmt objects.
      */
    statement::pointer *item;
};

#endif // LIB_STATEMENT_LIST_H
// vim: set ts=8 sw=4 et :
