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

#ifndef LIB_LIBRARY_MAP_COLUMNS_H
#define LIB_LIBRARY_MAP_COLUMNS_H

#include <lib/columnar.h>

/**
  * The library_map_columns class is used to represent
  * the columns of a library map report.
  */
class library_map_columns
{
public:
    /**
      * The destructor.
      * It is not virtual, thou shalt not derive from this class.
      */
    ~library_map_columns();

    /**
      * The constructor.
      */
    library_map_columns(output::pointer &os);

    /**
      * The eoln method is used to emit the current line.
      */
    void eoln(void);

    /**
      * The headings method is used to print the column headings.
      */
    void headings(void);

    output::pointer codeaddr;
    output::pointer codeleng;
    output::pointer name;
    output::pointer segkind;
    output::pointer segnum;
    output::pointer mtype;
    output::pointer version;
    output::pointer comments;

private:
    columnar cols;

    /**
      * The default constructor.  Do not use.
      */
    library_map_columns();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    library_map_columns(const library_map_columns &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    library_map_columns &operator=(const library_map_columns &rhs);
};

#endif // LIB_LIBRARY_MAP_COLUMNS_H
// vim: set ts=8 sw=4 et :
