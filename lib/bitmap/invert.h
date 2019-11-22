//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#ifndef LIB_BITMAP_INVERT_H
#define LIB_BITMAP_INVERT_H

#include <lib/bitmap.h>

/**
  * The bitmap_invert class is used to represent the processing required
  * to invert a bitmap image.
  */
class bitmap_invert:
    public bitmap
{
public:
    /**
      * The destructor.
      */
    virtual ~bitmap_invert();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const pointer &deeper);

protected:
    // See base class for documentation.
    void read(unsigned &width, unsigned &height, unsigned char *&data);

    // See base class for documentation.
    void write(unsigned width, unsigned height, const unsigned char *data);

    // See base class for documentation.
    void set_size_hint(unsigned width, unsigned height);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    bitmap_invert(const pointer &deeper);

    pointer deeper;

    /**
      * The default constructor.
      * Do not use.
      */
    bitmap_invert();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    bitmap_invert(const bitmap_invert &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    bitmap_invert &operator=(const bitmap_invert &rhs);
};

#endif // LIB_BITMAP_INVERT_H
// vim: set ts=8 sw=4 et :
