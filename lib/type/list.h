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

#ifndef LIB_TYPE_LIST_H
#define LIB_TYPE_LIST_H

#include <lib/ac/vector>

#include <lib/type.h>

/**
  * The type_list class is used to represent a list of types (without
  * names).  This is used to represent array dimensions during parsing.
  */
class type_list
{
public:
    typedef boost::shared_ptr<type_list> pointer;

    /**
      * The destructor.
      */
    virtual ~type_list();

    /**
      * The default constructor.
      */
    type_list();

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    type_list(const type_list &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    type_list &operator=(const type_list &rhs);

    bool empty(void) const { return content.empty(); }
    size_t size() const { return content.size(); }
    type::pointer get(size_t n) const { return content[n]; }
    type::pointer operator[](size_t n) const { return get(n); }

    void push_back(const type::pointer &tp);

private:
    typedef std::vector<type::pointer> content_t;

    /**
      * The content instance variable is used to remember the contents
      * of the list.
      */
    content_t content;
};

#endif // LIB_TYPE_LIST_H
// vim: set ts=8 sw=4 et :
