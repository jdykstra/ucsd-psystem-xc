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

#ifndef LIB_TYPE_FILE_H
#define LIB_TYPE_FILE_H

#include <lib/expression.h>
#include <lib/type.h>

/**
  * The type_file class is used to represent a system file object.
  */
class type_file:
    public type
{
public:
    typedef boost::shared_ptr<type_file> pointer;

    // See compiler/compglbls.text in II.0 sources
    //     compiler/decpart.a.text
    //
    // The actual size is smaller than this,
    // see system/globals.txt, sizeof(fib)
    enum
    {
        file_size_in_words = 300,
        nil_file_size_in_words = 40
    };

    /**
      * The destructor.
      */
    virtual ~type_file();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const type::pointer &subtype);

    /**
      * The is_a method is used to determine if the given type is
      * a file type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a file type, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * a file type.
      *
      * @param arg
      *     The expression who's type to be tested
      * @returns
      *     bool; true if it is a file type, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * a file type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a file type, false if not
      */
    static bool is_a(const type *arg);

    // See base class for documentation.
    int get_finit_recwords(void) const;

protected:
    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    rcstring get_pascal_name(void) const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    void name_hint(const rcstring &name);

    // See base class for documentation.
    type::pointer get_subtype(void) const;

    // See base class for documentation.
    unsigned get_size_in_bits(void) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable(void) const;

    // See base class for documentation.
    type::pointer late_binding_or_null(void) const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      */
    type_file(const type::pointer &subtype);

    /**
      * The subtype instance variable is used to remember what this file
      * contains.
      */
    type::pointer subtype;

    /**
      * The hint instance variable is used to remember the type name
      * hint, if any.
      */
    rcstring hint;

    /**
      * The default constructor.
      */
    type_file();

    /**
      * The copy constructor.  Do not use.
      */
    type_file(const type_file &);

    /**
      * The assignment operator.  Do not use.
      */
    type_file &operator=(const type_file &);
};

#endif // LIB_TYPE_FILE_H
// vim: set ts=8 sw=4 et :
