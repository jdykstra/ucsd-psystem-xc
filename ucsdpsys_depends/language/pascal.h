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

#ifndef UCSDPSYS_DEPENDS_LANGUAGE_PASCAL_H
#define UCSDPSYS_DEPENDS_LANGUAGE_PASCAL_H

#include <ucsdpsys_depends/language.h>

/**
  * The language_pascal class is used to represent
  */
class language_pascal:
    public language
{
public:
    typedef boost::shared_ptr<language_pascal> pointer;

    /**
      * The destructor.
      */
    virtual ~language_pascal();

    // See base class for documentation.
    void depends(const rcstring &infile);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static language::pointer create(void);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    language_pascal();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    language_pascal(const language_pascal &rhs);

    rcstring tokval;

    void comment(const rcstring &text);

    bool next_token(void);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    static pointer create(const language_pascal &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    language_pascal &operator=(const language_pascal &rhs);
};

#endif // UCSDPSYS_DEPENDS_LANGUAGE_PASCAL_H
// vim: set ts=8 sw=4 et :
