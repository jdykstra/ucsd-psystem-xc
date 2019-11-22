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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_CHAR_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_CHAR_PRETTY_H

#include <lib/expression/char.h>

class translator_littoral; // forward

/**
  * The expression_char_littoral class is used to represent littoral
  * printing a constant charater.
  */
class expression_char_littoral:
    public expression_char
{
public:
    typedef boost::shared_ptr<expression_char_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_char_littoral();

private:
    /**
      * The constructor.
      */
    expression_char_littoral(const location &locn, unsigned char value,
        translator_littoral *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const location &locn, unsigned char value,
        translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The littoral instance variable is used to remember where to send
      * our beautiful source code.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    expression_char_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    expression_char_littoral(const expression_char_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_char_littoral &operator=(const expression_char_littoral &);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_CHAR_PRETTY_H
// vim: set ts=8 sw=4 et :
