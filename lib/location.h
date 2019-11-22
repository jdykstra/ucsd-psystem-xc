//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#ifndef LIB_LOCATION_H
#define LIB_LOCATION_H


#include <lib/rcstring.h>

/**
  * The location class is used to represent a position within a source
  * file.
  */
class location
{
public:
    /**
      * The destructor.
      */
    virtual ~location();

    /**
      * The constructor.
      */
    location(const rcstring &file_name, long line_number);

    /**
      * The default constructor.
      */
    location();

    /**
      * The copy constructor.
      */
    location(const location &);

    /**
      * The assignment operator.
      */
    location &operator=(const location &);

    /**
      * The representation method may be used to obtain a string
      * representation of the file position.
      */
    rcstring representation(void) const;

    void operator+=(const location &rhs);

    location operator+(const location &rhs) const;

    /**
      * The empty method is used to determine whether or not the
      * location is empty (no file name, no line number).
      */
    bool empty(void) const;

    /**
      * The clear method is used to remove all location data from a
      * location.  This is sometimes needed for expressions that are
      * re-used many times.
      */
    void clear(void);

    /**
      * The get_debughook method is used to get a string which can be
      * used in the generated output as a call to the totally inadequate
      * debug hook function.
      */
    rcstring get_debughook(int arg = 0) const;

    /**
      * The get_hash_line method is used to get a string which can be
      * used in the generated output as a "#line" directive.  It includes
      * the newline.
      */
    rcstring get_hash_line(void) const;

    /**
      * The get_file_name method is used to determine the name of the
      * file the location is within.
      */
    const rcstring &get_file_name(void) const { return file_name; }

    /**
      * The get_line_number method is used to determine the line of the
      * file the location is within.
      */
    int get_line_number(void) const { return line_number; }

    /**
      * The incriment_line_number method is used to advance the line
      * number of the location.
      */
    void
    incriment_line_number(int nlines = 1)
    {
        line_number += nlines;
    }

    /**
      * The equality operation may be used to test whether another
      * location is equal to this location.
      */
    bool operator==(const location &rhs) const;

    /**
      * The inequality operation may be used to test whether another
      * location is different than this location.
      */
    bool operator!=(const location &rhs) const { return !operator==(rhs); }

    /**
      * The builtin class method is used to create a location that
      * claims to be built-in.
      */
    static location builtin(void);

private:
    /**
      * The file_name instance variable is used to remember the name of
      * the file.
      */
    rcstring file_name;

    /**
      * The line number instance variable is used to remember the line
      * number within the file.
      */
    long line_number;
};

#endif // LIB_LOCATION_H
// vim: set ts=8 sw=4 et :
