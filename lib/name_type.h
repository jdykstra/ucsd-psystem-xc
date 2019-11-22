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

#ifndef LIB_NAME_TYPE_H
#define LIB_NAME_TYPE_H

#include <lib/variable/name/list.h>
#include <lib/variable/name.h>
#include <lib/type.h>

/**
  * The name_type class is used to represent a list of names and their
  * types.
  */
class name_type
{
public:
    /**
      * The destructor.
      */
    virtual ~name_type();

    /**
      * The default constructor.  Do not use.
      */
    name_type();

    /**
      * The constructor.
      */
    name_type(const variable_name_list &name, const type::pointer &ptype);

    /**
      * The copy constructor.
      */
    name_type(const name_type &);

    /**
      * The assignment operator.
      */
    name_type &operator=(const name_type &);

    size_t size() const { return name.size(); }
    const variable_name &get_name(size_t n) const { return name[n]; }
    type::pointer get_type() const { return ptype; }

private:
    /**
      * The name instance variable is used to remember the names of the
      * symbols being declared.
      */
    variable_name_list name;

    /**
      * The ptype instance variable is used to remember the type of the
      * symbols being declared.
      */
    type::pointer ptype;
};

#endif // LIB_NAME_TYPE_H
// vim: set ts=8 sw=4 et :
