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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_NIL_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_NIL_PRETTY_H

#include <lib/expression/nil.h>

class translator_littoral; // forward

/**
  * The expression_nil_littoral class is used to represent the ouptut
  * required to littoral print the NIL constant.
  */
class expression_nil_littoral:
    public expression_nil
{
public:
    typedef boost::shared_ptr<expression_nil_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_nil_littoral();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    expression_nil_littoral(const location &locn,
        translator_littoral *littoral);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const location &locn, translator_littoral *littoral);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    translator_littoral &littoral;

private:
    /**
      * The default constructor.  Do not use.
      */
    expression_nil_littoral(translator_littoral *littoral);

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_nil_littoral(const expression_nil_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_nil_littoral &operator=(const expression_nil_littoral &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_NIL_PRETTY_H
// vim: set ts=8 sw=4 et :
