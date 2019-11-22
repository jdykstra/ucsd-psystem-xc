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

#ifndef UCSDPSYS_LITTORAL_STATEMENT_EMPTY_LITTORAL_H
#define UCSDPSYS_LITTORAL_STATEMENT_EMPTY_LITTORAL_H

#include <lib/statement/empty.h>

class translator_littoral; // forward

/**
  * The statement_empty_littoral class is used to represent
  * the empty C++ statement.
  */
class statement_empty_littoral:
    public statement_empty
{
public:
    typedef boost::shared_ptr<statement_empty_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_empty_littoral();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param littoral
      *     The translation context we are working in.
      */
    static pointer create(translator_littoral *littoral);

protected:
    // See base class for documentation.
    void traversal(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param littoral
      *     The translation context we are working in.
      */
    statement_empty_littoral(translator_littoral *littoral);

    /**
      * The littoral instance variable is used to remember the
      * translation context we are working in.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_empty_littoral();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_empty_littoral(const statement_empty_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_empty_littoral &operator=(const statement_empty_littoral &rhs);
};

#endif // UCSDPSYS_LITTORAL_STATEMENT_EMPTY_LITTORAL_H
// vim: set ts=8 sw=4 et :
