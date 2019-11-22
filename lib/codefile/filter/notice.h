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

#ifndef LIB_CODEFILE_FILTER_NOTICE_H
#define LIB_CODEFILE_FILTER_NOTICE_H

#include <lib/codefile/filter.h>

/**
  * The codefile_filter_notice class is used to represent a codefile filter
  * that substitutes a different copyright notice.
  */
class codefile_filter_notice:
    public codefile_filter
{
public:
    typedef boost::shared_ptr<codefile_filter_notice> pointer;

    /**
      * The destructor.
      */
    virtual ~codefile_filter_notice();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The codefile to be filtered.
      * @param notice
      *     The replacement copyright notice.
      */
    static pointer create(const codefile::pointer &deeper,
        const rcstring &notice);

protected:
    // See base class for documentation.
    void set_copyright_notice(const rcstring &text);

    // See base class for documentation.
    rcstring get_copyright_notice(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param deeper
      *     The codefile to be filtered.
      * @param notice
      *     The replacement copyright notice.
      */
    codefile_filter_notice(const codefile::pointer &deeper,
        const rcstring &notice);

    /**
      * The notice instance variable is used to remember the replacement
      * copyright notice.
      */
    rcstring notice;

    /**
      * The default constructor.  Do not use.
      */
    codefile_filter_notice();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    codefile_filter_notice(const codefile_filter_notice &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    codefile_filter_notice &operator=(const codefile_filter_notice &rhs);
};

#endif // LIB_CODEFILE_FILTER_NOTICE_H
// vim: set ts=8 sw=4 et :
