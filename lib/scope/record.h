//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#ifndef LIB_SCOPE_RECORD_H
#define LIB_SCOPE_RECORD_H

#include <lib/scope.h>

/**
  * The scope_record class is used to represent the layout of a RECORD.
  */
class scope_record:
    public scope
{
public:
    typedef boost::shared_ptr<scope_record> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_record();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param is_packed
      *     Whether or not this record is to have its fields packed.
      */
    scope_record(bool is_packed);

protected:
    // See base class for documentation.
    void notify_function_parameters(const name_type_list &ntl);

    // See base class for documentation.
    void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value);

    // See base class for documentation.
    void type_declaration_notify(const variable_name &name,
        const type::pointer &tp);

    // See base class for documentation.
    void variable_declaration_notify(const symbol_variable_p &svp);

    // See base class for documentation.
    int get_lex_level_height(void) const;

    // See base class for documentation.
    void label_declaration_notify(const variable_name_list &names);

    // See base class for documentation.
    bool will_accept_enums(void) const;

    // See base class for documentation.
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

    // See base class for documentation.
    symbol_variable::pointer parameter_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

private:
    /**
      * The is_packed instance variable is used to remember whether or
      * not this record is a packed record.  In a packed record, fields
      * are packed into words as tightly as possible, without crossing a
      * 16-bit boundary.
      */
    bool is_packed;

    /**
      * The default constructor.  Do not use.
      */
    scope_record();

    /**
      * The copy constructor.  Do not use.
      */
    scope_record(const scope_record &);

    /**
      * The assignment operator.  Do not use.
      */
    scope_record &operator=(const scope_record &);
};

#endif // LIB_SCOPE_RECORD_H
// vim: set ts=8 sw=4 et :
