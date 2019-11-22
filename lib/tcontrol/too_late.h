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

#ifndef LIB_TCONTROL_TOO_LATE_H
#define LIB_TCONTROL_TOO_LATE_H

#include <lib/tcontrol.h>

class translator; // forward

/**
  * The tcontrol_too_late class is used to represent a tcontrol setting
  * filter that detencts and warns about the category of control
  * comments that must appear very early in the codefile.
  */
class tcontrol_too_late:
    public tcontrol
{
public:
    typedef boost::shared_ptr<tcontrol_too_late> pointer;

    /**
      * The destructor.
      */
    virtual ~tcontrol_too_late();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param xlat
      *     The translation context we are working wothin.
      * @param deeper
      *     The tcontrol to set if all is well.
      */
    static pointer create(translator *xlat, const tcontrol::pointer &deeper);

protected:
    // See base class for documentation.
    void set(const rcstring_list &args);

private:
    /**
      * The xlat instance variable is used to remember the translation
      * context we are working wothin.
      */
    translator *xlat;

    /**
      * The deeper instance variable is used to remember the tcontrol to
      * set if all is well.
      */
    tcontrol::pointer deeper;

    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param xlat
      *     The translation context we are working wothin.
      * @param deeper
      *     The tcontrol to set if all is well.
      */
    tcontrol_too_late(translator *xlat, const tcontrol::pointer &deeper);

    /**
      * The default constructor.  Do not use.
      */
    tcontrol_too_late(translator *);

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_too_late(const tcontrol_too_late &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_too_late &operator=(const tcontrol_too_late &rhs);
};

#endif // LIB_TCONTROL_TOO_LATE_H
// vim: set ts=8 sw=4 et :
