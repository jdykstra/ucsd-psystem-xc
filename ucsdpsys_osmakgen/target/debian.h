//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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

#ifndef UCSDPSYS_OSMAKGEN_TARGET_DEBIAN_H
#define UCSDPSYS_OSMAKGEN_TARGET_DEBIAN_H

#include <ucsdpsys_osmakgen/target.h>

/**
  * The target_debian class is used to represent writing the necessary
  * files into the debian/ directory for building debian packages of the
  * UCSD Pascal operating system.
  */
class target_debian:
    public target
{
public:
    typedef boost::shared_ptr<target_debian> pointer;

    /**
      * The destructor.
      */
    virtual ~target_debian();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The name of the file to be written.
      */
    static pointer create(const rcstring &filename);

protected:
    // See base class for documentation.
    void generate(const rcstring_list &manifest);

    // See base class for documentation.
    void set_version(const rcstring &text);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param filename
      *     The name of the file to be written.
      */
    target_debian(const rcstring &filename);

    rcstring version;

    void generate_changelog(void);
    void generate_compat(void);
    void generate_control(void);
    void generate_copyright(void);
    void generate_rules(void);
    void generate_install(void);

    /**
      * The default constructor.  Do not use.
      */
    target_debian();

    /**
      * The copy constructor.  Do no tuse.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    target_debian(const target_debian &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    target_debian &operator=(const target_debian &rhs);
};

#endif // UCSDPSYS_OSMAKGEN_TARGET_DEBIAN_H
// vim: set ts=8 sw=4 et :
