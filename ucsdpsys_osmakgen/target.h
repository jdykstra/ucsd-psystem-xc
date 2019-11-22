//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#ifndef UCSDPSYS_OSMAKGEN_TARGET_H
#define UCSDPSYS_OSMAKGEN_TARGET_H

#include <lib/output.h>
#include <lib/rcstring/list.h>

/**
  * The target class is used to represent an abstract interface to
  * generating files from a file manifest, such as a Makefile file,
  * or a Debian rules file, etc.
  */
class target
{
public:
    typedef boost::shared_ptr<target> pointer;

    /**
      * The destructor.
      */
    virtual ~target();

    /**
      * The factory class method si sued to create an new target
      * instance, choosing the class based on the output file bame.
      *
      * @param filename
      *     The name of the file to be written.
      */
    static pointer factory(const rcstring &filename);

    /**
      * The generate method is used to generate the output file, given a
      * list of file names.
      *
      * @param manifest
      *     The list of file names to be processed.
      */
    virtual void generate(const rcstring_list &manifest) = 0;

    virtual void set_default_host(const rcstring &name);

    virtual void set_default_arch(const rcstring &name);

    virtual void no_blurb(void);

    virtual void set_notice(const rcstring &text);

    virtual void set_version(const rcstring &text);

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param filename
      *     The name of the file to be written, or "-" for stdout.
      */
    target(const rcstring &filename);

    /**
      * The op instance variable is used to remember the where to send
      * the output.
      */
    output::pointer op;

private:
    /**
      * The default constructor.  Do not use.
      */
    target();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    target(const target &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    target &operator=(const target &rhs);
};

#endif // UCSDPSYS_OSMAKGEN_TARGET_H
// vim: set ts=8 sw=4 et :
