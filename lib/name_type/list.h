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

#ifndef LIB_NAME_TYPE_LIST_H
#define LIB_NAME_TYPE_LIST_H

#include <lib/name_type.h>

/**
  * The name_type_list class is used to represent an ordered list of
  * name_type objects.
  */
class name_type_list
{
public:
    /**
      * The destructor.
      */
    virtual ~name_type_list();

    /**
      * The default constructor.
      */
    name_type_list();

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialization.
      */
    name_type_list(const name_type_list &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    name_type_list &operator=(const name_type_list &rhs);

    /**
      * The empty method is used to determine whether or not the list is
      * empty (has no elements).
      */
    bool empty() const { return (length == 0); }

    /**
      * The size method is used to obtain the number of elements in the
      * list.
      */
    size_t size() const { return length; }

    /**
      * The push_back method is used to add aother list element to the
      * end of the list.
      */
    void push_back(const name_type &arg);

    /**
      * The get method is used to access an element of the list.
      *
      * @param n
      *     The list element to get.  Zero based.
      */
    const name_type &get(size_t n) const { return list[n]; }

    /**
      * The array index operator is used to access an element of the list.
      *
      * @param n
      *     The list element to get.  Zero based.
      */
    const name_type &operator[](size_t n) const { return get(n); }

    /**
      * The flatten_into method is used to convert a possibly nested
      * list of names and types into a linear (depth 1) list of names
      * and types.
      *
      * @param output
      *     Where to put the flattened list.
      */
    void flatten_into(name_type_list &output) const;

private:
    /**
      * The length instance variable is used to remember how long the
      * list is.
      */
    size_t length;

    /**
      * The maximum instance variable is used to remember how much space
      * has been allocated for the list.
      */
    size_t maximum;

    /**
      * The list instance variable is used to remember the base address
      * of the dynamically allocated array of list elements.
      */
    name_type *list;
};

#endif // LIB_NAME_TYPE_LIST_H
// vim: set ts=8 sw=4 et :
