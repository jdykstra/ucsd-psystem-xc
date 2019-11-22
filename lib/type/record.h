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

#ifndef LIB_TYPE_RECORD_H
#define LIB_TYPE_RECORD_H

#include <lib/expression.h>
#include <lib/scope.h>
#include <lib/type.h>
#include <lib/variant.h>

class symbol_functor; // forward

/**
  * The type_record class is used to represent a composite data
  * structure, composed of fields of other types.  Recursion is
  * possible, but not self recursion.
  */
class type_record:
    public type
{
public:
    typedef boost::shared_ptr<type_record> pointer;

    /**
      * The destructor.
      */
    virtual ~type_record();

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      *
      * @param sp
      *     The scope containing this  record.
      */
    type_record(const scope::pointer &sp);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param sp
      *     The scope containing this  record.
      */
    static pointer create(const scope::pointer &sp);

    /**
      * The lookup method is used to look for a field name in the
      * record's scope.
      *
      * @param name
      *     The name of the record field to look for.
      * @returns
      *     a pointer to the correspending symbol, or NULL on error.
      */
    symbol::pointer lookup(const rcstring &name) const;

    /**
      * The lookup_fuzzy method is used to look for a field name in the
      * record's scope.
      *
      * @param name
      *     The name of the symbol to look for.
      * @param best
      *     The fstrcmp level minimum before a symbol is "better",
      *     updated on return
      * @returns
      *     the name of the least fuzzy symbol, or the empty string if
      *     nothing is better
      */
    rcstring lookup_fuzzy(const rcstring &name, double &best) const;

    /**
      * The lookup_fuzzy method is used to look for a field name in the
      * record's scope.  This is used after lookup fails.
      *
      * @param name
      *     The name of the record field to look for.
      * @returns
      *     a pointer to a the most similar symbol by name, or NULL if
      *     nothing is especially similar.  If you want the name of the
      *     similar sumbol, use the get_name() method.
      */
    symbol::pointer lookup_fuzzy(const rcstring &name) const;

    /**
      * The is_a method is used to determine if the given type is
      * of record type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is an record type, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * of record type.
      *
      * @param arg
      *     The expression wheo's type is to be tested
      * @returns
      *     bool; true if it is an record type, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The get_variant method is used to get a pointer to the variant
      * tree, if this record has variant types.
      *
      * @returns
      *     pointer to variant tree, or NULL if no variant.
      */
    variant::pointer get_variant(void) const { return recvar; }

    /**
      * The attach_variant method is used by the parser to attache a
      * variant specification to a record.
      *
      * @param vp
      *     The variant spec to be attached.
      */
    void attach_variant(const variant::pointer &vp);

    /**
      * The symbol_table_walk method may be used to apply a functor to
      * each symbol in the symbol table.
      *
      * @param f
      *     The functor to apply to each symbol.
      */
    void symbol_table_walk(symbol_functor &f);

protected:
    // See base class for documentation.
    rcstring get_name() const;

    // See base class for documentation.
    rcstring get_pascal_name() const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    unsigned get_size_in_bits() const;

    // See base class for documentation.
    void name_hint(const rcstring &name);

    // See base class for documentation.
    bool is_a_variable() const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    type::pointer get_subtype() const;

    // See base class for documentation.
    type::pointer late_binding_or_null() const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The sp instance variable is used to remember the scope (that's
      * what has the symbol table) related to this type.
      */
    scope::pointer sp;

    /**
      * The name instance variable is used to remember the name of the
      * record type.  Initially this will be unhelpful (and have a colon
      * in it) but the grammar hints us for type names, from time to
      * time.
      */
    rcstring name;

    /**
      * The variant instance variable is used to remember a pointer to
      * the variant tree, if this record has variant types.  Will be
      * NULL for non-variant records.
      */
    variant::pointer recvar;

    /**
      * The default constructor.  Do not use.
      */
    type_record();

    /**
      * The copy constructor.  Do not use.
      */
    type_record(const type_record &);

    /**
      * The assignment operator.  Do not use.
      */
    type_record &operator=(const type_record &);
};

#endif // LIB_TYPE_RECORD_H
// vim: set ts=8 sw=4 et :
