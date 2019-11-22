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

#ifndef LIB_SYMBOL_LIST_H
#define LIB_SYMBOL_LIST_H

#include <lib/ac/stddef.h>

#include <lib/symbol.h>

/**
  * The symbol_list class is used to represent an ordered list of
  * pointers to symbols.
  */
class symbol_list
{
public:
    /**
      * The destructor.
      */
    virtual ~symbol_list();

    /**
      * The default constructor.
      */
    symbol_list();

    /**
      * The push_back method is used to add another symbol to the end of
      * the list.
      */
    void push_back(const symbol::pointer &arg);

    /**
      * The size method is used to obtain the number of symbols in the
      * list.
      */
    size_t size() const { return length; }

    symbol::pointer get(int n) const;

private:
    /**
      * The length instance variable is used to remember how many items
      * are in the list.
      */
    size_t length;

    /**
      * The maximum instance variable is used to remember the allocated
      * size of the array.
      */
    size_t maximum;

    /**
      * The item instance variable is used to remember the base of the
      * dynamically allocated array to hold the list of pointers to
      * symbols.
      */
    symbol::pointer *item;

    /**
      * The copy constructor.  Do not use.
      */
    symbol_list(const symbol_list &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_list &operator=(const symbol_list &);
};

#endif // LIB_SYMBOL_LIST_H
// vim: set ts=8 sw=4 et :
