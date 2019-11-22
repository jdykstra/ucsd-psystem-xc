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

#ifndef LIB_TYPE_REFERENCE_IMPLICIT_H
#define LIB_TYPE_REFERENCE_IMPLICIT_H

#include <lib/type/reference.h>

/**
  * The type_reference_implicit class is used to represent a variable
  * reference which is implicit (the users disn't ask for it, and
  * doesn't know it exists).
  */
class type_reference_implicit:
    public type_reference
{
public:
    typedef boost::shared_ptr<type_reference_implicit> pointer;

    /**
      * The destructor.
      */
    virtual ~type_reference_implicit();

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      */
    type_reference_implicit(const type::pointer &arg);

public:
    static pointer create(const type::pointer &arg);

protected:
    // See base class for documentation.
    bool is_an_implicit_reference() const;

    // See base class for documentation.
    rcstring get_name() const;

private:
    /**
      * The default constructor.  Do not use.
      */
    type_reference_implicit();

    /**
      * The copy constructor.  Do not use.
      */
    type_reference_implicit(const type_reference_implicit &);

    /**
      * The assignment operator.  Do not use.
      */
    type_reference_implicit &operator=(const type_reference_implicit &);
};

#endif // LIB_TYPE_REFERENCE_IMPLICIT_H
// vim: set ts=8 sw=4 et :
