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

#ifndef UCSDPSYS_VOLUME_H
#define UCSDPSYS_VOLUME_H

#include <lib/config.h>
#include <boost/shared_ptr.hpp>

#include <lib/rcstring.h>
#include <lib/rcstring/list.h>

/**
  * The volume abstract base class is used to represent a volume to be
  * mounted when ucsdpsys runs.
  */
class volume
{
public:
    typedef boost::shared_ptr<volume> pointer;

    /**
      * The destructor.
      */
    virtual ~volume();

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    volume();

public:
    /**
      * The slurp method is used to prepare the volume for use by the
      * ucsdpsys_vm command.
      */
    virtual void slurp(void) = 0;

    /**
      * The unslurp method is used to debrief the volume after use by
      * the ucsdpsys_vm command.
      */
    virtual void unslurp(void) = 0;

    /**
      * The command_line_argument method is used to obtain a suitable
      * ucsdpsys_vm command line option to use this volume.
      */
    virtual rcstring command_line_argument(void) = 0;

    /**
      * The contains_system_pascal method is used to determine whether
      * the given volume contains a "system.pascal" file, necessary to
      * boot a system.
      *
      * @returns
      *     true of the volume contains a "system.pascal" file, false if
      *     it does not.
      */
    virtual bool contains_system_pascal(void) const = 0;

    /**
      * The contains_system_files method may be used to determine whether or
      * not a disk volume contains the essential system files.  This can be
      * used to determine whether or not an implicit system disk image needs to
      * be created.
      */
    virtual bool contains_system_files(void) = 0;

private:
    /**
      * The copy constructor.  Do not use.
      */
    volume(const volume &);

    /**
      * The assignment operator.  Do not use.
      */
    volume &operator=(const volume &);
};

#endif // UCSDPSYS_VOLUME_H
// vim: set ts=8 sw=4 et :
