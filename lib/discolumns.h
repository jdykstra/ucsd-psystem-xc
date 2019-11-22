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

#ifndef LIB_DISCOLUMNS_H
#define LIB_DISCOLUMNS_H

#include <lib/columnar.h>

/**
  * The discolumns class is used to represent the columns of a
  * disassembly listing.
  */
class discolumns
{
public:
    /**
      * The destructor.
      */
    virtual ~discolumns();

    /**
      * The constructor.
      */
    discolumns(const output::pointer &os);

    /**
      * The eoln method is used to indicate the end of a line, causing
      * all of the columns, for this line, to be printed.
      */
    void eoln(void);

    output::pointer full;
    output::pointer source_column;
    output::pointer address_column;
    output::pointer data_column;
    output::pointer label_column;
    output::pointer opcode_column;
    output::pointer argument_column;
    output::pointer comment_column;

    output::pointer sym_location_column;
    output::pointer sym_address_column;
    output::pointer sym_size_column;
    output::pointer sym_kind_column;
    output::pointer sym_name_column;
    output::pointer sym_type_column;

    static bool long_address_format;

private:
    columnar col;

    /**
      * The default constructor.  Do not use.
      */
    discolumns();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    discolumns(const discolumns &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    discolumns &operator=(const discolumns &rhs);
};

#endif // LIB_DISCOLUMNS_H
// vim: set ts=8 sw=4 et :
