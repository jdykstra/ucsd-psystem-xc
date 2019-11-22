//
// UCSD p-System operating system
// Copyright (C) 2006, 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
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

#ifndef UCSDPSYS_VOLUME_DIRECTORY_H
#define UCSDPSYS_VOLUME_DIRECTORY_H

#include <ucsdpsys/volume.h>

/**
  * The volume_directory class is used to represent a temporary volume
  * created from a directory (or directories) and extracted again when
  * the virtual machine terminates.
  */
class volume_directory:
    public volume
{
public:
    typedef boost::shared_ptr<volume_directory> pointer;

    /**
      * The destructor.
      */
    virtual ~volume_directory();

    /**
      * The create class method is used to creat enew dynamically
      * allocated instances of this class.
      *
      * @param path
      *     The path to the directory of contents.
      * @param mode
      *     The mode to operate the file as ('w', 'r' or 'f').
      * @param name
      *     The volume name to use.
      */
    static pointer create(const rcstring &path, char mode,
        const rcstring &name);

    /**
      * The create class method is used to creat enew dynamically
      * allocated instances of this class.
      *
      * @param paths
      *     The paths to the directories of contents.
      * @param mode
      *     The mode to operate the file as ('w', 'r' or 'f').
      * @param name
      *     The volume name to use.
      */
    static pointer create(const rcstring_list &paths, char mode,
        const rcstring &name);

protected:
    // See base class for documentation.
    void slurp(void);

    // See base class for documentation.
    void unslurp(void);

    // See base class for documentation.
    rcstring command_line_argument(void);

    /**
      * The push_back method is used to add another directory to get
      * ontents from.  (Only the direct directory is used to extract
      * contents during unslurp.)
      */
    void push_back(const rcstring &path);

    void set_volume_name(const rcstring &name);

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
      *     The path to the directory of contents.
      * @param mode
      *     The mode to operate the file as ('w', 'r' or 'f').
      * @param name
      *     The volume name to use.
      */
    volume_directory(const rcstring &path, char mode, const rcstring &name);

    /**
      * The constructor.
      *
      * @param paths
      *     The paths to the directories of contents.
      * @param mode
      *     The mode to operate the file as ('w', 'r' or 'f').
      * @param name
      *     The volume name to use.
      */
    volume_directory(const rcstring_list &paths, char mode,
        const rcstring &name);

    rcstring_list directories;
    char mode;
    rcstring temporary_file_name;
    rcstring volume_name;
    bool slurped;

    const rcstring &get_temporary_file_name(void);

    /**
      * The default constructor.  Do not use.
      */
    volume_directory();

    /**
      * The copy constructor.  Do not use.
      */
    volume_directory(const volume_directory &);

    /**
      * The assignment operator.  Do not use.
      */
    volume_directory &operator=(const volume_directory &);
};

#endif // UCSDPSYS_VOLUME_DIRECTORY_H
// vim: set ts=8 sw=4 et :
