//
// UCSD p-System operating system
// Copyright (C) 2006, 2010-2012 Peter Miller
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

#ifndef UCSDPSYS_VOLUME_LIST_H
#define UCSDPSYS_VOLUME_LIST_H

#include <ucsdpsys/volume.h>

/**
  * The volume_list class is used to represent an ordered list of
  * volumes.
  */
class volume_list:
    public volume
{
public:
    typedef boost::shared_ptr<volume_list> pointer;

    /**
      * The destructor.
      */
    virtual ~volume_list();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

    bool empty(void) const { return (length == 0); }

    void push_back(const volume::pointer &vp);

    void insert_second(const volume::pointer &vp);

    // See base class for documentation.
    rcstring command_line_argument(void);

    // See base class for documentation.
    void slurp(void);

    // See base class for documentation.
    void unslurp(void);

    // See base class for documentation.
    bool contains_system_pascal(void) const;

    // See base class for documentation.
    bool contains_system_files(void);

private:
    /**
      * The default constructor.
      * It is private on purpose, use the #create class method instead.
      */
    volume_list();

    size_t length;
    size_t maximum;
    volume::pointer *item;

    /**
      * The copy constructor.  Do not use.
      */
    volume_list(const volume_list &);

    /**
      * The assignment operator.  Do not use.
      */
    volume_list &operator=(const volume_list &);
};

#endif // UCSDPSYS_VOLUME_LIST_H
// vim: set ts=8 sw=4 et :
