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

#ifndef UCSDPSYS_VOLUME_FILE_H
#define UCSDPSYS_VOLUME_FILE_H

#include <ucsdpsys/volume.h>

/**
  * The volume_file class is used to represent a non-temporary volume
  * read from a file and (optionally) updated again when the virtual
  * machine terminates.
  */
class volume_file:
    public volume
{
public:
    typedef boost::shared_ptr<volume_file> pointer;

    /**
      * The destructor.
      */
    virtual ~volume_file();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param path
      *     The unix pathname of the file containing the disk image.
      * @param mode
      *     The open mode: 'w' for read and write, 'r' for read only,
      *     and 'f' for write and forget.
      */
    static pointer create(const rcstring &path, char mode);

protected:
    // See base class for documentation.
    void slurp(void);

    // See base class for documentation.
    void unslurp(void);

    // See base class for documentation.
    rcstring command_line_argument(void);

    // See base class for documentation.
    bool contains_system_pascal(void) const;

    // See base class for documentation.
    bool contains_system_files(void);

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param path
      *     The unix pathname of the file containing the disk image.
      * @param mode
      *     The open mode: 'w' for read and write, 'r' for read only,
      *     and 'f' for write and forget.
      */
    volume_file(const rcstring &path, char mode);

    rcstring path;
    char mode;

    /**
      * The default constructor.
      */
    volume_file();

    /**
      * The copy constructor.
      */
    volume_file(const volume_file &);

    /**
      * The assignment operator.
      */
    volume_file &operator=(const volume_file &);
};

#endif // UCSDPSYS_VOLUME_FILE_H
// vim: set ts=8 sw=4 et :
