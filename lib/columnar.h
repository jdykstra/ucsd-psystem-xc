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

#ifndef LIB_COLUMNAR_H
#define LIB_COLUMNAR_H

#include <lib/output/string_list.h>

/**
  * The columnar class is used to represent an object which constructs
  * columnar output.
  */
class columnar
{
public:
    /**
      * The destructor.
      */
    virtual ~columnar();

    /**
      * The constructor.
      */
    columnar(const output::pointer &deeper);

    output::pointer create_new_column(int left, int right);

    void eoln(void);

    /**
      * The flush method is used to make sure all of the output has been
      * written.
      */
    void flush(void);

private:
    output::pointer deeper;

    struct col_t
    {
        ~col_t();
        col_t(int, int);

        int icol;
        int ocol;
        output_string_list::pointer contents;
        output::pointer external_presentation;
        rcstring_list text;
        int min;
        int max;
    };

    size_t length;
    size_t maximum;
    col_t **list;

    int in_col;
    int out_col;

    void emit_char(char c);

    /**
      * The default constructor.
      */
    columnar();

    /**
      * The copy constructor.
      */
    columnar(const columnar &);

    /**
      * The assignment operator.
      */
    columnar &operator=(const columnar &);
};

#endif // LIB_COLUMNAR_H
// vim: set ts=8 sw=4 et :
