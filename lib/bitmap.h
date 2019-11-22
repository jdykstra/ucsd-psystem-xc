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

#ifndef LIB_BITMAP_H
#define LIB_BITMAP_H

#include <lib/config.h>
#include <boost/shared_ptr.hpp>

#include <lib/rcstring.h>

/**
  * The bitmap class is used to represent the procesing required to read
  * and write files containing bitmapped images.
  *
  * Data is stored 8 pixels per byte, horizontally.  Least
  * significant bit in the byte is the left-most pixel in the byte.
  * A bit value of zero means a black pixel, otherwise white.
  */
class bitmap
{
public:
    typedef boost::shared_ptr<bitmap> pointer;

    /**
      * The destructor.
      */
    virtual ~bitmap();

    static pointer factory(const rcstring &filename);

    /**
      * The read method is used to open the named file and read an image
      * from that file.  Format unspecified, that's up to the derived class.
      *
      * @param width
      *     This is where the image width, in pixels, is returned.
      * @param height
      *     This is where the image height, in pixels, is returned.
      * @param data
      *     This is where the data is returned.  It will be allocated
      *     for you, and you must delete[]data when you are done with
      *     it.  ownership of the pointer is given to the caller.
      */
    virtual void read(unsigned &width, unsigned &height,
        unsigned char *&data) = 0;

    virtual void write(unsigned width, unsigned height,
        const unsigned char *data) = 0;

    virtual void set_size_hint(unsigned width, unsigned height) = 0;

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    bitmap();

private:
    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    bitmap(const bitmap &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    bitmap &operator=(const bitmap &rhs);
};

#endif // LIB_BITMAP_H
// vim: set ts=8 sw=4 et :
