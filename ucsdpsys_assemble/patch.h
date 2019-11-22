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

#ifndef UCSDPSYS_ASSEMBLE_PATCH_H
#define UCSDPSYS_ASSEMBLE_PATCH_H

#include <lib/expression.h>
#include <lib/segment/builder.h>

/**
  * The patch class is used to represent a location in the code that is
  * to be updated ("patched") when the associated expression tree can be
  * resolved.
  *
  * At the end of the assembly, expression trees that are still
  * unresolved are either errors, or turn into linker information
  * records, for the liker to resolve.
  */
class patch
{
public:
    typedef boost::shared_ptr<patch> pointer;

    /**
      * The destructor.
      */
    virtual ~patch();

    enum kind_t
    {
        kind_byte,
        kind_big,
        kind_word_be,
        kind_word_le,
    };

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param address
      *     The code address, relative to the start of the segment, that
      *     is to be patched.
      * @param kind
      *     The kind of patch to be applied.
      * @param value
      *     The expression tree describing the value of the patch, if
      *     all the symbols were actually defined.
      */
    static pointer create(unsigned address, kind_t kind,
        const expression::pointer &value);

    /**
      * The apply method is used to attempt to apply the patch to the
      * given segment.
      *
      * @param sbp
      *     The segment to apply it to
      * @returns
      *     true if applied successfully, false if one or more symbols
      *     remain unsatisfied.
      */
    bool apply(const segment_builder::pointer &sbp) const;

    /**
      * The error_too_complex method is used to report an error
      * for expressions too complex for the linker to be able to link.
      */
    void error_too_complex(void) const;

    rcstring representation(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param address
      *     The code address, relative to the start of the segment, that
      *     is to be patched.
      * @param kind
      *     The kind of patch to be applied.
      * @param value
      *     The expression tree describing the value of the patch, if
      *     all the symbols were actually defined.
      */
    patch(unsigned address, kind_t kind, const expression::pointer &value);

    /**
      * The address instance variable is used to remember the code
      * address, relative to the start of the segment, that is to be
      * patched.
      */
    unsigned address;

    /**
      * The kind instance variable is used to remember the kind (size
      * and encoding) of the patch to be applied.
      */
    kind_t kind;

    /**
      * The value instance variable is used to remember the expression
      * tree describing the value of the patch, if all the symbols were
      * actually defined.
      */
    expression::pointer value;

    /**
      * The default constructor.  Do not use.
      */
    patch();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    patch(const patch &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    patch &operator=(const patch &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_PATCH_H
// vim: set ts=8 sw=4 et :
