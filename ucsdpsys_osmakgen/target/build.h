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

#ifndef UCSDPSYS_OSMAKGEN_TARGET_BUILD_H
#define UCSDPSYS_OSMAKGEN_TARGET_BUILD_H

#include <lib/output.h>

#include <ucsdpsys_osmakgen/target.h>

/**
  * The target_build class is used to represent writing the input to
  * some kind of build tool, such as make(1) or cook(1), etc.
  */
class target_build:
    public target
{
public:
    typedef boost::shared_ptr<target_build> pointer;

    /**
      * The destructor.
      */
    virtual ~target_build();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    target_build(const rcstring &filename);

    virtual void wrap_comment(const rcstring &text) = 0;

    virtual void wrap_assignment(const rcstring &name,
        const rcstring_list &value, bool immed = false) = 0;

    void wrap_assignment(const rcstring &name, const rcstring &value,
        bool immed = false);

    // See base class for documentation.
    void generate(const rcstring_list &manifest);

    // See base class for documentation.
    void no_blurb(void);

    // See base class for documentation.
    void set_default_host(const rcstring &text);

    // See base class for documentation.
    void set_default_arch(const rcstring &text);

    // See base class for documentation.
    void set_notice(const rcstring &text);

    /**
      * The write_rule virtual method is used to write a rule.  Its form
      * will differ for make(1) and cook(1), etc.
      *
      * @param lhs
      *     The left hand side of the rule, the rule target.
      * @param rhs
      *     The right hand side of the rule, the rule ingredients.
      *     Possibly empty.
      * @param body
      *     The commands to execute when the rule triggers.
      *     Possibly empty.
      */
    virtual void write_rule(const rcstring &lhs, const rcstring_list &rhs,
        const rcstring_list &body) = 0;

    /**
      * The write_clean_files_rule virtual method is used to write a
      * rule to delete files.  Its form will differ for make(1) and
      * cook(1), etc.
      *
      * @param lhs
      *     The left hand side of the rule, the rule target.
      * @param rhs
      *     The right hand side of the rule, the rule ingredients.
      *     Possibly empty.
      * @param files
      *     The files to be delected by the rule body when the rule triggers.
      */
    virtual void write_clean_files_rule(const rcstring &lhs,
        const rcstring_list &rhs, const rcstring_list &files) = 0;

    /**
      * The write_clean_dirs_rule virtual method is used to write a rule
      * to delete directories.  Its form will differ for make(1) and
      * cook(1), etc.
      *
      * @param lhs
      *     The left hand side of the rule, the rule target.
      * @param rhs
      *     The right hand side of the rule, the rule ingredients.
      *     Possibly empty.
      * @param dirs
      *     The directories to be deleted by the rule body when the
      *     rule triggers.
      */
    virtual void write_clean_dirs_rule(const rcstring &lhs,
        const rcstring_list &rhs, const rcstring_list &dirs) = 0;

    /**
      * The remember_dist_clean_file is used to add a file to the list
      * of files to be removed my the "make distclean" target.
      */
    void remember_dist_clean_file(const rcstring &path);

    /**
      * The remember_dist_clean_dir is used to add a directory to the list
      * of directory to be removed my the "make distclean" target.
      */
    void remember_dist_clean_dir(const rcstring &path);

private:
    bool blurb;

    rcstring default_host;

    rcstring default_arch;

    rcstring notice;

    /**
      * The clean_files instance variable is used to remember the list
      * of files to be cleaned, when the "clean" target is built.
      */
    rcstring_list clean_files;

    /**
      * The clean_dirs instance variable is used to remember the list of
      * directories to be cleaned, when the "clean" target is built.
      */
    rcstring_list clean_dirs;

    /**
      * The dist_clean_files instance variable is used to remember the
      * list of files to be cleaned, when the "distclean" target is
      * built.
      */
    rcstring_list dist_clean_files;

    /**
      * The dist_clean_dirs instance variable is used to remember the
      * list of directories to be cleaned, when the "distclean" target
      * is built.
      */
    rcstring_list dist_clean_dirs;

    rcstring_list remove_system_segments;

    rcstring_list system_candidates;

    rcstring_list library_candidates;

    rcstring_list no_make_dir;

    rcstring_list uninstall_files;

    int max_stage;

    void clean_file(const rcstring &path);

    void clean_dir(const rcstring &path);

    bool mkdir_p(const rcstring &path);

    void stage1_compile(const rcstring &prog, bool maybe_globals, bool has_asm);

    void stage1_assemble(const rcstring &prog);

    void simple_copy_rule(const rcstring &src, const rcstring &dst);

    void simple_install_rule(const rcstring &src, const rcstring &dst);

    virtual rcstring load(const rcstring &variable_name) const = 0;

    virtual rcstring resolve(const rcstring &filename) const;

    virtual rcstring shell(const rcstring &command) const = 0;

    virtual void cond_if_exists(const rcstring &filename) = 0;

    virtual void cond_if_not_exists(const rcstring &filename) = 0;

    virtual void cond_if_eq(const rcstring &lhs, const rcstring &rhs) = 0;

    virtual void cond_else(void) = 0;

    virtual void cond_endif(void) = 0;

    virtual rcstring extra_include_flags(void) const;

    virtual void extra_file_header(void);

    /**
      * The default constructor.  Do no tuse.
      */
    target_build();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    target_build(const target_build &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    target_build &operator=(const target_build &rhs);
};

#endif // UCSDPSYS_OSMAKGEN_TARGET_BUILD_H
// vim: set ts=8 sw=4 et :
