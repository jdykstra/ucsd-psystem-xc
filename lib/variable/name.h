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

#ifndef LIB_VARIABLE_NAME_H
#define LIB_VARIABLE_NAME_H

#include <lib/location.h>

/**
  * The variable_name class is used to represent a variable name with an
  * attached location.
  */
class variable_name
{
public:
    /**
      * The destructor.
      */
    virtual ~variable_name();

    /**
      * The default constructor.
      */
    variable_name();

    /**
      * The constructor.
      */
    variable_name(const rcstring &name);

    /**
      * The constructor.
      */
    variable_name(const rcstring &name, const location &locn);

    /**
      * The copy constructor.
      */
    variable_name(const variable_name &);

    /**
      * The assignment operator.
      */
    variable_name &operator=(const variable_name &);

    /**
      * The get_name method is used to obtain the name of this variable.
      */
    rcstring get_name() const { return name; }

    /**
      * The get_name method is used to obtain the name of this variable.
      */
    const location &get_location() const { return locn; }

    bool empty(void) const { return name.empty(); }

private:
    /**
      * The name instance variable is used to remember name of the
      * variable in question.
      */
    rcstring name;

    /**
      * The locn instance variable is used to remember where in the
      * source code the variable was mentioned.
      */
    location locn;
};

#endif // LIB_VARIABLE_NAME_H
// vim: set ts=8 sw=4 et :
