//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2012 Peter Miller
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

#ifndef LIB_VARIABLE_NAME_LIST_H
#define LIB_VARIABLE_NAME_LIST_H

#include <lib/variable/name.h>

/**
  * The variable_name_list class is used to represent an ordered list of
  * names with attached source code locations.
  */
class variable_name_list
{
public:
    /**
      * The destructor.
      */
    virtual ~variable_name_list();

    /**
      * The default constructor.
      */
    variable_name_list();

    /**
      * The copy constructor.
      */
    variable_name_list(const variable_name_list &);

    /**
      * The assignment operator.
      */
    variable_name_list &operator=(const variable_name_list &);

    /**
      * The push back method is used to append a variable location to
      * the list.
      *
      * @param arg
      *     The variable and location to append.
      */
    void push_back(const variable_name &arg);

    /**
      * The push back method is used to append a list of variable
      * locations to the list.
      *
      * @param arg
      *     The variables and location to append.
      */
    void push_back(const variable_name_list &arg);

    /**
      * The size method is used to obtain the number of items in the list.
      */
    size_t size() const { return length; }

    /**
      * The empty method is used to determine whether or not the list is
      * empty (has no elements).
      */
    bool empty() const { return (length == 0); }

    /**
      * The get method is used to obtain the n'th element of the list.
      *
      * @note
      *     No array bounds check is done.
      */
    const variable_name &get(size_t n) const { return list[n]; }

    /**
      * The array index operator is used to obtain the n'th element of
      * the list.
      */
    const variable_name &operator[](size_t n) const { return get(n); }

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
      * of the dynamically allocated array used to hold the list.
      */
    variable_name *list;
};

#endif // LIB_VARIABLE_NAME_LIST_H
// vim: set ts=8 sw=4 et :
