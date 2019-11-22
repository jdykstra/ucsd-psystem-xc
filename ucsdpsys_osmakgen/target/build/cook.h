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

#ifndef UCSDPSYS_OSMAKGEN_TARGET_BUILD_COOK_H
#define UCSDPSYS_OSMAKGEN_TARGET_BUILD_COOK_H

#include <ucsdpsys_osmakgen/target/build.h>

/**
  * The target_build_cook class is used to represent the processing needed
  * to write a cookbook, for cook(1), for the ucsd-psystem-os project.
  */
class target_build_cook:
    public target_build
{
public:
    typedef boost::shared_ptr<target_build_cook> pointer;

    /**
      * The destructor.
      */
    virtual ~target_build_cook();

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
    void wrap_comment(const rcstring &text);

    // See base class for documentation.
    void wrap_assignment(const rcstring &name, const rcstring_list &value,
        bool immed);

    // See base class for documentation.
    void write_rule(const rcstring &rtarget, const rcstring_list &ingredients,
        const rcstring_list &body);

    // See base class for documentation.
    void write_clean_files_rule(const rcstring &lhs, const rcstring_list &rhs,
        const rcstring_list &files);

    // See base class for documentation.
    void write_clean_dirs_rule(const rcstring &lhs, const rcstring_list &rhs,
        const rcstring_list &dirs);

    // See base class for documentation.
    rcstring load(const rcstring &variable_name) const;

    // See base class for documentation.
    rcstring resolve(const rcstring &filename) const;

    // See base class for documentation.
    rcstring shell(const rcstring &command) const;

    // See base class for documentation.
    void cond_if_exists(const rcstring &filename);

    // See base class for documentation.
    void cond_if_not_exists(const rcstring &filename);

    // See base class for documentation.
    void cond_if_eq(const rcstring &lhs, const rcstring &rhs);

    // See base class for documentation.
    void cond_else(void);

    // See base class for documentation.
    void cond_endif(void);

    // See base class for documentation.
    rcstring extra_include_flags(void) const;

    // See base class for documentation.
    void extra_file_header(void);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param filename
      *     The name of the file to be written.
      */
    target_build_cook(const rcstring &filename);

    /**
      * The default constructor.  Do not use.
      */
    target_build_cook();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    target_build_cook(const target_build_cook &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    target_build_cook &operator=(const target_build_cook &rhs);
};

#endif // UCSDPSYS_OSMAKGEN_TARGET_BUILD_COOK_H
// vim: set ts=8 sw=4 et :
