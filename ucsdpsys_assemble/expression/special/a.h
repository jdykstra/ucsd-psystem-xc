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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_SPECIAL_A_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_SPECIAL_A_H

#include <ucsdpsys_assemble/expression/special.h>

/**
  * The expression_special_a class is used to represent
  * the A registor of a CPU.
  */
class expression_special_a:
    public expression_special
{
public:
    typedef boost::shared_ptr<expression_special_a> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_special_a();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The location of the symbol in the source.
      */
    static pointer create(const location &locn);

protected:
    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param locn
      *     The location of the symbol in the source.
      */
    expression_special_a(const location &locn);

    /**
      * The default constructor.  Do not use.
      */
    expression_special_a();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_special_a(const expression_special_a &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_special_a &operator=(const expression_special_a &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_SPECIAL_A_H
// vim: set ts=8 sw=4 et :
