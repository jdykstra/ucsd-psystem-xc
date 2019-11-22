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

#ifndef UCSDPSYS_ASSEMBLE_LISTING_H
#define UCSDPSYS_ASSEMBLE_LISTING_H

#include <lib/columnar.h>

/**
  * The listing class is used to represent the processing required to
  * produce the assembler listing.
  */
class listing
{
public:
    /**
      * The destructor.
      */
    virtual ~listing();

    /**
      * The constructor.
      *
      * @param os
      *     The output stream to write on.
      */
    listing(const output::pointer &os);

    /**
      * The addr_empty method is used to determine whether or not the
      * address column in presently empty.
      *
      * @returns
      *     bool; true if there is nothing is the address column for
      *     this row, false if somethign has already been printed in the
      *     address column.
      */
    bool addr_empty(void) const;

    /**
      * The addr_printf method is used to print text into the address column.
      *
      * @param fmt
      *     The format for the text, and the remaining arguments.  See
      *     printf(3) for more information.
      */
    void addr_printf(const char *fmt, ...)                  FORMAT_PRINTF(2, 3);

    /**
      * The data_printf method is used to print text into the data column.
      *
      * @param fmt
      *     The format for the text, and the remaining arguments.  See
      *     printf(3) for more information.
      */
    void data_printf(const char *fmt, ...)                  FORMAT_PRINTF(2, 3);

    /**
      * The get_source_column is used to obtain an output stream for the
      * source column of the assembler listing.
      */
    output::pointer get_source_column(void) const;

    /**
      * The eoln method is used to tell the assembler listing that one
      * source line has been completed, and to prepare for the next.
      */
    void eoln(void);

    /**
      * The flush method is used to make sure all of the output has been
      * written.
      */
    void flush(void);

    /**
      * The set_heading1 method is used to set the first line of title
      * text on the assembler listing pages.
      *
      * @param text
      *     The text of the first title line.
      */
    void set_heading1(const rcstring &text);

    /**
      * The set_heading2 method is used to set the second line of title
      * text on the assembler listing pages.
      *
      * @param text
      *     The text of the second title line.
      */
    void set_heading2(const rcstring &text);

private:
    /**
      * The orig instance variable is used to remember the output stream
      * onto which the listing columns will be written.
      */
    output::pointer orig;

    /**
      * The col instance variable is used to remember the columnar
      * output processing required to format the output as columns that
      * wrap on a source line-by-line basis.
      */
    columnar col;

    /**
      * The address_column instance variable is used to remember the
      * output stream to be used to print opcode addresses.
      */
    output::pointer address_column;

    /**
      * The data_column instance variable is used to remember the output
      * stream to be used to print opcode data bytes.
      */
    output::pointer data_column;

    /**
      * The source_column instance variable is used to remember the
      * output stream to be used to print source code lines.
      */
    output::pointer source_column;

    /**
      * The addr_empty_flag instance variable is used to remember
      * whether or not something has been printed in the address column
      * for this row of the columnar output.
      */
    bool addr_empty_flag;

    /**
      * The default constructor.  Do not use.
      */
    listing();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    listing(const listing &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    listing &operator=(const listing &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_LISTING_H
// vim: set ts=8 sw=4 et :
