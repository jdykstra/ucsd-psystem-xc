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

#ifndef LIB_SCOPE_UNIT_H
#define LIB_SCOPE_UNIT_H

#include <lib/scope.h>

/**
  * The scope_unit class is used to represent a simple outer-most-
  * level unit scope, where global symbols are declared.
  */
class scope_unit:
    public scope
{
public:
    typedef boost::shared_ptr<scope_unit> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_unit();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param name
      *     The name of the unit being delcared.
      * @param code_seg
      *     the INTRINSIC CODE segment number specified in the unit
      *     declaration, or -1 if not an intrinsic unit.
      * @param data_seg
      *     the INTRINSIC DATA segment number specified in the unit
      *     declaration, or -1 if not an intrinsic unit.
      * @param llht
      *     The lex level height of the unit.  (This is an ugly hack:
      *     the lex level height of a unit is whatever it takes to be at
      *     lex level 0.)
      */
    scope_unit(const rcstring &name, int code_seg, int data_seg, int llht);

    // See base class for documentation.
    void notify_function_parameters(const name_type_list &ntl);

    // See base class for documentation.
    int get_lex_level_height(void) const;

    // See base class for documentation.
    void label_declaration_notify(const variable_name_list &names);

    // See base class for documentation.
    bool is_a_unit(void) const;

protected:
    int get_code_seg(void) const { return code_seg; }
    int get_data_seg(void) const { return data_seg; }

private:
    /**
      * The code_seg instance variable is used to remember the INTRINSIC
      * CODE segment number specified in the unit declaration.
      */
    int code_seg;

    /**
      * The data_seg instance variable is used to remember the INTRINSIC
      * DATA segment number specified in the unit declaration.
      */
    int data_seg;

    int llht;

    /**
      * The default constructor.  Do not use.
      */
    scope_unit();

    /**
      * The copy constructor.  Do not use.
      */
    scope_unit(const scope_unit &);

    /**
      * The assignment operator.  Do not use.
      */
    scope_unit &operator=(const scope_unit &);
};

#endif // LIB_SCOPE_UNIT_H
// vim: set ts=8 sw=4 et :
