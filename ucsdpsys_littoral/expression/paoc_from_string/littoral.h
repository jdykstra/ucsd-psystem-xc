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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_PAOC_FROM_STRING_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_PAOC_FROM_STRING_PRETTY_H

#include <lib/expression/paoc_from_string.h>

/**
  * The expression_paoc_from_string_littoral class is used to represent
  * the processing necessary to littoral print a cast from string to
  * packed array of char.
  */
class expression_paoc_from_string_littoral:
    public expression_paoc_from_string
{
public:
    typedef boost::shared_ptr<expression_paoc_from_string_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_paoc_from_string_littoral();

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param rhs
      *     The value to cast.
      * @param tp
      *     The exact type of the result.
      */
    expression_paoc_from_string_littoral(const expression::pointer &rhs,
        const type::pointer &tp);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param rhs
      *     The value to cast.
      * @param tp
      *     The exact type of the result.
      */
    static pointer create(const expression::pointer &rhs,
        const type::pointer &tp);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    bool is_constant(void) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    expression_paoc_from_string_littoral();

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

private:
    /**
      * The copy constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_paoc_from_string_littoral(
        const expression_paoc_from_string_littoral &rhs);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    static pointer create(const expression_paoc_from_string_littoral &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_paoc_from_string_littoral &operator=(
        const expression_paoc_from_string_littoral &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_PAOC_FROM_STRING_PRETTY_H
// vim: set ts=8 sw=4 et :
