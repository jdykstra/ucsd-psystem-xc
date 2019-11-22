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

#ifndef LIB_VARIANT_H
#define LIB_VARIANT_H

#include <lib/ac/map>
#include <boost/shared_ptr.hpp>

#include <lib/type.h>

/**
  * The variant abstract base class is used to represent a variant
  * record tree, or tree node.
  */
class variant
{
public:
    typedef boost::shared_ptr<variant> pointer;

    /**
      * The destructor.
      */
    virtual ~variant();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param nwords
      *     The size in words
      */
    variant(unsigned nwords);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param nwords
      *     The size in words
      */
    static pointer create(unsigned nwords);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param type
      *     The type of the variant selector expression.
      */
    variant(const type::pointer &type);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param type
      *     The type of the variant selector expression.
      */
    static pointer create(const type::pointer &type);

    /**
      * The get_size_in_words method may be used to obtain the size, in
      * 16-bit words, of the record at this branch in the variant tree.
      */
    unsigned get_size_in_words(void) const { return size_in_words; }

    /**
      * The terminal method may be used to determine whether or not this
      * is a terminal variant node.  A node is terminal if it has no
      * child variants.
      */
    bool terminal(void) const;

    /**
      * The get_type method is used to obtain the type of this variant
      * selector.
      *
      * @note
      *     This method will return NULL if the variant is terminal.
      */
    type::pointer get_type(void) const { return vtype; }

    /**
      * The get_child method may be used to obtain the variant branch
      * corresponding to the given value.
      *
      * @param n
      *     The value to use to decide
      * @returns
      *     pointer to nested variant, or NULL if no such branch
      */
    variant::pointer get_child(int n) const;

    /**
      * The register_child method is used by the grammar when parsing
      * variant record types, to add additional branches to a variant
      * tree.
      *
      * @param value
      *     The value to use to decode
      * @param child
      *     pointer to nested variant
      */
    void register_child(int value, const variant::pointer &child);

    /**
      * The set_size method is used to set the size of a terminal variant.
      *
      * Because we aren't wotking for a full abstract syntax tree, we
      * find out things from the parser as they appear in the source
      * file.  We don;t know that ar record is variant or not until we
      * have finished parsing it.  Thus, we can't set the size in the
      * constructor.  Sigh.
      *
      * @param nwords
      *     The size of the variant in words.
      */
    void set_size(unsigned nwords);

    /**
      * The set_otherwise_size method is used to by the parser to tell a
      * variant type what to return of none of the childeren match, when
      * calling get_child.
      *
      * @param nwords
      *     The size of the variant in words.
      */
    void set_otherwise_size(unsigned nwords);

private:
    /**
      * The size_in_words instance variable is used to remember the
      * size, in 16-bit words, of this variant.
      */
    unsigned size_in_words;

    /**
      * The vtype instance variable is used to remember the type of the
      * selector, even if no actual selector member variable is present.
      */
    type::pointer vtype;

    typedef std::map<int, variant::pointer> children_t;

    /**
      * The children instance variable is used to remember the child
      * variants of this variant.
      */
    children_t children;

    /**
      * The otherwise instance variable is used to remember what to
      * return from get_child when the value matches nothing.
      */
    variant::pointer otherwise;

    /**
      * The default constructor.  Do not use.
      */
    variant();

    /**
      * The copy constructor.  Do no tuse.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    variant(const variant &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    variant &operator=(const variant &rhs);
};

#endif // LIB_VARIANT_H
// vim: set ts=8 sw=4 et :
