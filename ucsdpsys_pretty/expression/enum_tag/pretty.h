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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_ENUM_TAG_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_ENUM_TAG_PRETTY_H

#include <lib/expression/enum_tag.h>

class translator_pretty; // forward

/**
  * The expression_enum_tag_pretty class is used to represent an
  * expression containing the value of an enumerated constant, to be
  * pretty printed.
  */
class expression_enum_tag_pretty:
    public expression_enum_tag
{
public:
    typedef boost::shared_ptr<expression_enum_tag_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_enum_tag_pretty();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the enumeration tag.
      * @param typ
      *     The type (parent enumeration) of the tag.
      * @param value
      *     The integer value of the tag.
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const variable_name &name, const type::pointer &typ,
        int value, translator_pretty *context);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     The name of the enumeration tag.
      * @param typ
      *     The type (parent enumeration) of the tag.
      * @param value
      *     The integer value of the tag.
      * @param context
      *     The translation context we are working within.
      */
    expression_enum_tag_pretty(const variable_name &name,
        const type::pointer &typ, int value, translator_pretty *context);

    /**
      * The context instance variable i sused to remember
      * the translation context we are working within.
      */
    translator_pretty *context;

    /**
      * The default constructor.  Do not use.
      */
    expression_enum_tag_pretty();

    /**
      * The copy constructor.  Do no tuse.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_enum_tag_pretty(const expression_enum_tag_pretty &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_enum_tag_pretty &operator=(
        const expression_enum_tag_pretty &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_ENUM_TAG_PRETTY_H
// vim: set ts=8 sw=4 et :
