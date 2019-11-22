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

#ifndef LIB_SCOPE_TEMPORARY_COMPOUND_H
#define LIB_SCOPE_TEMPORARY_COMPOUND_H

#include <lib/scope/temporary.h>

/**
  * The scope_temporary_compound class is used to represent the scope of
  * variables declared in-line within a begin-end compound statement.
  */
class scope_temporary_compound:
    public scope_temporary
{
public:
    typedef boost::shared_ptr<scope_temporary_compound> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_temporary_compound();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param ss
      *     The scope stack used to allocate and de-allocate.
      */
    scope_temporary_compound(scope_stack &ss);

private:
    /**
      * The default constructor.  Do not use.
      */
    scope_temporary_compound();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_temporary_compound(const scope_temporary_compound &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_temporary_compound &operator=(const scope_temporary_compound &rhs);
};

#endif // LIB_SCOPE_TEMPORARY_COMPOUND_H
// vim: set ts=8 sw=4 et :
