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

#ifndef LIB_TYPE_H
#define LIB_TYPE_H

#include <boost/shared_ptr.hpp>

#include <lib/rcstring.h>

/**
  * The abstract type base class is used to represent any type of
  * type.
  */
class type
{
public:
    /**
      * The pointer type is to be used for all pointers to statements.
      */
    typedef boost::shared_ptr<type> pointer;

    /**
      * The destructor.
      */
    virtual ~type();

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    type();

public:
    /**
      * The get_name method is used to obtain the type's name.
      * This is a human-friendly string.
      */
    virtual rcstring get_name(void) const = 0;

    /**
      * The get_pascal_name method is used to obtain the type's name.
      * This string is supposed to look like a type declaration.
      */
    virtual rcstring get_pascal_name(void) const = 0;

    /**
      * The get_c_declarator method is used to write a C declaration
      * equivalent to the Pascal type.
      *
      * @param name
      *     The name of the variable (or type, or parameter) being declared.
      */
    virtual rcstring get_c_declaration(const rcstring &name) const = 0;

    /**
      * The is_func method is used to determine whether this type is
      * that of a function returning something.
      *
      * @returns
      *     bool; true if it is a function, false if it is not.
      */
    virtual bool is_func(void) const;

    /**
      * The get_subtype method is used to get the subtype for
      * those types for which there is an additional qualifier.
      *
      * @returns
      *     a type, the return type if this is indeed a type with a
      *     subtype, or the error type if its anything else.
      */
    virtual type::pointer get_subtype(void) const = 0;

    /**
      * The get_size_in_bytes method is used to obtain the storage size
      * (in multiples of 8-bit bytes) of a variable or parameter of this
      * type.
      */
    unsigned get_size_in_bytes(void) const;

    /**
      * The get_size_in_words method is used to obtain the storage size
      * (in multiple of 16-bit words) of a variable or parameter of this
      * type.
      */
    virtual unsigned get_size_in_words(void) const;

    /**
      * The get_finit_recwords method is used to calculate the
      * appropriate FINIT RECWORDS argument, when generating the code
      * for FILE constructors.
      *
      * All of the sane cases simply return #get_size_in_words, which
      * is what the default implementation does.
      */
    virtual int get_finit_recwords(void) const;

    /**
      * The get_size_in_bits method is used to obtain the storage size
      * in bits of a variable or parameter of this type.
      */
    virtual unsigned get_size_in_bits(void) const = 0;

    /**
      * The is_an_implicit_reference method is used to determine whether
      * a type is an implicit reference type.
      */
    virtual bool is_an_implicit_reference(void) const;

    /**
      * The equality operator is used to see if two types are equal.
      *
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     bool; true if the types are equivalent, false if they are not.
      */
    virtual bool same_type_as(const type::pointer &rhs) const = 0;

    /**
      * The name_hint method is used to suggest a name to a type, it it
      * does not currently have a name.  This is useful for records and
      * enumerations, which are initially anonymous.
      *
      * The default implementation does nothing.
      *
      * @param name
      *     The suggested name for the type.
      */
    virtual void name_hint(const rcstring &name);

    /**
      * The is_a_variable method is used to determine whether or not a
      * symbol describes a variable (or parameter or record field).
      */
    virtual bool is_a_variable(void) const = 0;

    /**
      * The get_minimum_value method may be used to obtain the least
      * value a variable of this type may assume.
      *
      * @note
      *     This method is not meaningful for types who's underlying
      *     implementation is not an integer of some sort.  An assert
      *     will be thrown in those cases.
      */
    virtual int get_minimum_value(void) const;

    /**
      * The get_maximum_value method may be used to obtain the greatest
      * value a variable of this type may assume.
      *
      * @note
      *     This method is not meaningful for types who's underlying
      *     implementation is not an integer of some sort.  An assert
      *     will be thrown in those cases.
      */
    virtual int get_maximum_value(void) const;

    /**
      * The may_be_used_as_array_index method is used to determine
      * whether or not this type may be used as an array index.  For
      * most types, this is false (in which case, do NOT ise the
      * get_minimum or get_maximum methods).
      */
    virtual bool may_be_used_as_array_index(void) const;

    /**
      * The late_binding class method is used by the grammar to resolve types
      * at the point where they become fixed: variable declarations,
      * usually.  It is an error if a forward reference cannot be
      * resolved.
      *
      * @param tp
      *    The type to be bound.
      * @returns
      *    The type itself if it didn't need binding, or a brand new
      *    type if it did need late binding.
      */
    static type::pointer late_binding(const type::pointer &tp);

    /**
      * The need_late_binding method may be used to determine whether or
      * not a type requires additional late binding.
      */
    virtual bool need_late_binding(void) const = 0;

    /**
      * The get_human_readable_representation_of_value method is used to
      * get a human readable representation of a value of this type.
      *
      * @param n
      *     The value to be represented.
      */
    virtual rcstring get_human_readable_representation_of_value(int n)
        const = 0;

protected:
    /**
      * The late_binding_or_null method is used by the grammar to
      * resolve types at the point where they become fixed: variable
      * declarations, usually.  It is an error if a forward reference
      * cannot be resolved.
      *
      * If no work is necessary, return the NULL pointer.
      * This is what the default implementation does.
      *
      * This method is used by the late_binding class method.
      */
    virtual type::pointer late_binding_or_null(void) const;

private:
    /**
      * The copy constructor.  Do not use.
      */
    type(const type &);

    /**
      * The assignment operator.  Do not use.
      */
    type &operator=(const type &);
};

inline bool
operator==(const type::pointer &lhs, const type::pointer &rhs)
{
    // Try it both ways, so that "anything" can appear
    // both on the left and on the right.
    return (lhs->same_type_as(rhs) || rhs->same_type_as(lhs));
}

inline bool
operator!=(const type::pointer &lhs, const type::pointer &rhs)
{
    return !operator==(lhs, rhs);
}

#endif // LIB_TYPE_H
// vim: set ts=8 sw=4 et :
