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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_ENUM_TAG_COMPILE_H
#define UCSDPSYS_COMPILE_EXPRESSION_ENUM_TAG_COMPILE_H

#include <lib/expression/enum_tag.h>

class translator_compile; // forward

/**
  * The expression_enum_tag_compile class is used to represent
  * en enumerated constant, to be compiled.
  */
class expression_enum_tag_compile:
    public expression_enum_tag
{
public:
    typedef boost::shared_ptr<expression_enum_tag_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_enum_tag_compile();

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
        int value, translator_compile *context);

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
    expression_enum_tag_compile(const variable_name &name,
        const type::pointer &typ, int value, translator_compile *context);

    /**
      * The context instance variable is used to remember
      * the translation context we are working within.
      */
    translator_compile *context;

    /**
      * The default constructor.  Do not use.
      */
    expression_enum_tag_compile();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_enum_tag_compile(const expression_enum_tag_compile &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_enum_tag_compile &operator=(
        const expression_enum_tag_compile &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_ENUM_TAG_COMPILE_H
// vim: set ts=8 sw=4 et :
