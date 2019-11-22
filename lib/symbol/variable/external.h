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

#ifndef LIB_SYMBOL_VARIABLE_EXTERNAL_H
#define LIB_SYMBOL_VARIABLE_EXTERNAL_H

#include <lib/symbol/variable.h>

/**
  * The symbol_variable_external class is used to represent a variable
  * that exists in a separate data segment, an external variable.
  */
class symbol_variable_external:
    public symbol_variable
{
public:
    typedef boost::shared_ptr<symbol_variable_external> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_variable_external();

protected:
    /**
      * The constructor.
      * For ue by derived classes only.
      *
      * @param name
      *     The name of the variable
      * @param tp
      *     The type of the variable
      * @param segnum
      *     The number of the segment containing the variable.
      */
    symbol_variable_external(const variable_name &name,
        const type::pointer &tp, int segnum);

    /**
      * The get_segnum method may be used to obtain the number of the
      * segment containing the variable.
      */
    int get_segnum(void) const { return segnum; }

private:
    /**
      * The segnum instance variable is used to remember the number of
      * the segment containing the variable.
      */
    int segnum;

    /**
      * The default constructor.  Do not use.
      */
    symbol_variable_external();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_variable_external(const symbol_variable_external &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_variable_external &operator=(const symbol_variable_external &rhs);
};

#endif // LIB_SYMBOL_VARIABLE_EXTERNAL_H
// vim: set ts=8 sw=4 et :
