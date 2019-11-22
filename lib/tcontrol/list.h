//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#ifndef LIB_OPTION_LIST_H
#define LIB_OPTION_LIST_H

#include <lib/tcontrol.h>
#include <lib/symtab/template.h>

/**
  * The tcontrol_list class is used to represent a tcontrol that
  * contains sub-tcontrols.
  *
  * It works by running down the list of known tcontrols (args[0]), and
  * dispatching to the sub-tcontrol found (args.tail()).  No sub-tcontrol of
  * that name results in an error.
  */
class tcontrol_list:
    public tcontrol
{
public:
    typedef boost::shared_ptr<tcontrol_list> pointer;

    /**
      * The destructor.
      */
    virtual ~tcontrol_list();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

    // See base class for documentation.
    void set(const rcstring_list &args);

    /**
      * The register_command method is used to register a
      * sub-tcontrol by name.
      *
      * @param name
      *     The name of the sub-tcontrol.
      * @param sub
      *     The implementation of the sub-tcontrol.
      */
    void register_command(const rcstring &name,
        const tcontrol::pointer &sub);

private:
    /**
      * The dispatch instance variable i sused to rememebr the set of
      * sub-tcontrols.
      */
    symtab<tcontrol::pointer> dispatch;

    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    tcontrol_list();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_list(const tcontrol_list &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_list &operator=(const tcontrol_list &rhs);
};

#endif // LIB_OPTION_LIST_H
// vim: set ts=8 sw=4 et :
