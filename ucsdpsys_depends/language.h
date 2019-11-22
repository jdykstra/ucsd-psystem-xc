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

#ifndef UCSDPSYS_DEPENDS_LANGUAGE_H
#define UCSDPSYS_DEPENDS_LANGUAGE_H

#include <lib/ac/stdio.h>
#include <lib/ac/list>
#include <boost/shared_ptr.hpp>

#include <lib/location.h>
#include <lib/rcstring/list.h>

#include <ucsdpsys_depends/context.h>

/**
  * The language class is used to represent the processing required to
  * calculate include dependencies.
  */
class language
{
public:
    typedef boost::shared_ptr<language> pointer;

    /**
      * The destructor.
      */
    virtual ~language();

    /**
      * The depends method is used to scan the given file for include
      * dependencies.
      *
      * @param infile
      *     The name of the input file.
      */
    virtual void depends(const rcstring &infile) = 0;

    /**
      * The print_results method is used to print the results of the
      * include file scan.
      *
      * @param outfile
      *     The name of the output file.
      */
    void print_results(const rcstring &outfile) const;

    /**
      * The depends_include_directory class method is used to append
      * another directory to the include search path.
      *
      * @param path
      *     the path of a directory to look in
      */
    static void depends_include_directory(const char *path);

    /**
      * The depends_view_directory class method is used to append
      * another directory to the view path.
      *
      * @param path
      *     the path of a directory to look in
      */
    static void depends_view_directory(const char *path);

    /**
      * The depends_recursive_set class method is used to request a
      * recursive search into each of the included files.
      */
    static void depends_recursive_set();

    /**
      * The depends_prefix class method is used to specific what to put
      * at the start of the output line listing the dependencies.
      *
      * @param str
      *     The string to print.
      */
    static void depends_prefix(const char *str);

    /**
      * The depends_suffix class method is used to specific what to put
      * at the end of the output line listing the dependencies.
      *
      * @param str
      *     The string to print.
      */
    static void depends_suffix(const char *str);

    /**
      * The depends_one_per_line class method is used to request that
      * each dependency (if any) should be output on a separate line.
      */
    static void depends_one_per_line(void);

protected:
    /**
      * The default constructor.
      * For use by derived classess only.
      */
    language();

    void lex_open(const rcstring &filename);

    bool lex_open_include(const rcstring &filename);

    int lex_getc(void);

    void lex_ungetc(int c);

    bool lex_read_one_line(rcstring &line);

    location get_lex_location(void) const;

private:
    rcstring_list include_files;

    int view_access(const rcstring &filename, int mode);

    FILE *view_fopen(const rcstring &filename, const char *flags) const;

    static bool recursive;
    static rcstring_list include_path;
    static rcstring_list view_path;
    static rcstring prefix;
    static rcstring suffix;
    static bool one_per_line;

    typedef std::list<context::pointer> lex_stack_t;

    lex_stack_t lex_stack;

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    language(const language &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    language &operator=(const language &rhs);
};

#endif // UCSDPSYS_DEPENDS_LANGUAGE_H
// vim: set ts=8 sw=4 et :
