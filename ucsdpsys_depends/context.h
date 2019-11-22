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

#ifndef UCSDPSYS_DEPENDS_CONTEXT_H
#define UCSDPSYS_DEPENDS_CONTEXT_H

#include <boost/shared_ptr.hpp>
#include <lib/ac/stdio.h>

#include <lib/location.h>

/**
  * The context class is used to represent an open file for obtaining
  * input from.
  */
class context
{
public:
    typedef boost::shared_ptr<context> pointer;

    /**
      * The destructor.
      */
    virtual ~context();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The name of the file to be opened.
      * @param fp
      *     The associated file stream.
      */
    static pointer create(const rcstring &filename, FILE *fp);

    int getch(void)
    {
        int c = getc(ifp);
        if (c == '\n')
            ++line_number;
        return c;
    }

    void ungetch(int c);

    location get_location(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param filename
      *     The name of the file to be opened.
      * @param fp
      *     The associated file stream.
      */
    context(const rcstring &filename, FILE *fp);

    rcstring ifn;

    FILE *ifp;

    int line_number;

    void fatal_error(const char *str);

    /**
      * The default constructor.  Do not use.
      */
    context();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    context(const context &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    context &operator=(const context &rhs);
};

#endif // UCSDPSYS_DEPENDS_CONTEXT_H
// vim: set ts=8 sw=4 et :
