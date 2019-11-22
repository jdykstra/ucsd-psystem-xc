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

#ifndef LIB_LABEL_H
#define LIB_LABEL_H

#include <boost/shared_ptr.hpp>

/**
  * The label abstract base class is used to represent a generic
  * implementation of an assembler branch label.  It allows labels to be
  * define before or after the are used as a branch destination.  Note
  * that this is an abstraction, and does not actually know anything
  * about addresses or opcodes.
  *
  * Label instances are usually managed using a label::pointer object.
  */
class label
{
public:
    /**
      * The pointer type is to be used for all pointers to labels.
      */
    typedef boost::shared_ptr<label> pointer;

    /**
      * The destructor.
      */
    virtual ~label();

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    label();

public:
    /**
      * The define_here method is used to set the address of the label.
      * This may occur before or after the label has been used as a jump
      * destination.
      */
    virtual void define_here(void) = 0;

    /**
      * The goto_from_here method is used to emit opcodes to perform an
      * unconditional jump from the current code address to the label.
      */
    virtual void goto_from_here(void) = 0;

    /**
      * The branch_from_here_if_false method is used to emit opcodes to
      * perform a conditional jump (if the top-of-stack is false) from
      * the current code address to the label.
      */
    virtual void branch_from_here_if_false(void) = 0;

    /**
      * The branch_from_here_if_true method is used to emit opcodes to
      * perform a conditional jump (if the top-of-stack is true) from
      * the current code address to the label.
      *
      * Only use this method if you have absolutely no other way to
      * perform the jump.  Building a logical negation expression is
      * preferable, because that gives more of the compiler ways to
      * optimize what is happening.  Usually this generates less code
      * than using an LNOT opcode.
      */
    virtual void branch_from_here_if_true(void) = 0;

    /**
      * The branch_from_here_if_equal method is used to emit opcodes to
      * perform a conditional jump (if words tos and tos-1 are equal)
      * from the current code address to the label.
      */
    virtual void branch_from_here_if_equal(void) = 0;

    /**
      * The branch_from_here_if_not_equal method is used to emit opcodes
      * to perform a conditional jump (if words tos and tos-1 are not
      * equal) from the current code address to the label.
      */
    virtual void branch_from_here_if_not_equal(void) = 0;

    /**
      * The self_relative_from_here method is used to insert a two-byte
      * value into the code stream.  This value if relative to the
      * address (of the first of the two bytes) by subtraction.
      * That is
      *     destination=address-*address;
      * conversely
      *     *address=address-destination
      */
    virtual void self_relative_from_here(void) = 0;

    /**
      * The is-defined method may be used to determine whether or not a
      * label has been defined.
      */
    virtual bool is_defined(void) const = 0;

private:
    /**
      * The copy constructor.  Do not use.
      */
    label(const label &);

    /**
      * The assignment operator.  Do not use.
      */
    label &operator=(const label &);
};

#endif // LIB_LABEL_H
// vim: set ts=8 sw=4 et :
