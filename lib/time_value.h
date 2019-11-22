//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2012 Peter Miller
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

#ifndef LIB_TIME_VALUE_H
#define LIB_TIME_VALUE_H

#include <sys/time.h>

/**
  * The time_value class is used to represent a system time in seconds
  * and microseconds.  It is a thin wrapper around the system's "struct
  * timeval" to allow simple arithmetic.
  */
class time_value:
    public timeval
{
public:
    /**
      * The destructor.
      */
    virtual ~time_value();

    /**
      * The default constructor.
      */
    time_value();

    /**
      * The constructor.
      */
    time_value(int seconds);

    /**
      * The constructor.
      */
    time_value(double seconds);

    /**
      * The copy constructor.
      */
    time_value(const time_value &rhs);

    /**
      * The assignment operator.
      */
    time_value &operator=(const time_value &rhs);

    /**
      * The get_time_of_day method is used to set this time value to the
      * current time of day, to a fraction of a second.
      */
    void get_time_of_day();

    time_value &operator+=(const time_value &rhs);

    time_value operator+(const time_value &rhs) const;

    time_value &operator-=(const time_value &rhs);

    time_value operator-(const time_value &rhs) const;

    operator double() const;

    /**
      * The valid method is used to confirm that this object is
      * internally consistent.
      */
    bool valid() const;

    bool operator==(const time_value &rhs) const;
    bool operator!=(const time_value &rhs) const;
    bool operator>(const time_value &rhs) const;
    bool operator>=(const time_value &rhs) const;
    bool operator<(const time_value &rhs) const;
    bool operator<=(const time_value &rhs) const;
};

#endif // LIB_TIME_VALUE_H
// vim: set ts=8 sw=4 et :
