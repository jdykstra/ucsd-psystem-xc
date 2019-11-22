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

#include <lib/ac/errno.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/output.h>

#include <lib/path_join.h>
#include <lib/rcstring/accumulator.h>

#include <ucsdpsys_depends/language.h>


bool language::recursive;
rcstring_list language::include_path;
rcstring_list language::view_path;
rcstring language::prefix;
rcstring language::suffix;
bool language::one_per_line;


language::~language()
{
}


language::language()
{
}


void
language::depends_one_per_line(void)
{
    one_per_line = true;
}


void
language::depends_prefix(const char *s)
{
    prefix = s;
}


void
language::depends_suffix(const char *s)
{
    suffix = s;
}


void
language::depends_recursive_set()
{
    recursive = true;
}


void
language::depends_include_directory(const char *path)
{
    while (path[0] == '.' && path[1] == '/')
        path += 2;
    include_path.push_back_unique(path);
}


void
language::depends_view_directory(const char *path)
{
    if (view_path.empty())
        view_path.push_back(".");
    while (path[0] == '.' && path[1] == '/')
        path += 2;
    view_path.push_back_unique(path);
}


int
language::view_access(const rcstring &filename, int mode)
{
    if (filename[0] == '/')
        return access(filename.c_str(), mode);
    if (view_path.empty())
        view_path.push_back(".");
    for (size_t j = 0; j < view_path.size(); ++j)
    {
        rcstring dir = view_path[j];
        rcstring path = path_join(dir, filename);
        int n = access(path.c_str(), mode);
        if (n == 0)
            return 0;
        if (errno != ENOENT)
            return n;
    }
    errno = ENOENT;
    return -1;
}


FILE *
language::view_fopen(const rcstring &filename, const char *mode)
    const
{
    if (view_path.empty())
        view_path.push_back(".");
    for (size_t j = 0; ; ++j)
    {
        if (j >= view_path.size())
        {
            errno = ENOENT;
            explain_output_error_and_die
            (
                "%s",
                explain_fopen(filename.c_str(), mode)
            );
            //NOTREACHED
        }

        rcstring dir = view_path[j];
        rcstring path = path_join(dir, filename);
        FILE *fp = fopen(path.c_str(), mode);
        if (fp)
            return fp;
        if (errno != ENOENT)
        {
            explain_output_error_and_die
            (
                "%s",
                explain_fopen(path.c_str(), mode)
            );
        }
    }
}


void
language::print_results(const rcstring &ofn)
    const
{
    FILE *ofp = (ofn == "-" ? stdout : explain_fopen_or_die(ofn.c_str(), "w"));
    assert(ofp);
    if (!include_files.empty())
    {
        rcstring_list result(include_files);
        result.sort();
        if (one_per_line)
        {
            if (!prefix.empty())
                fprintf(ofp, "%s\n", prefix.c_str());
            for (size_t j = 0; j < result.size(); ++j)
                fprintf(ofp, "%s\n", result[j].c_str());
            if (!suffix.empty())
                fprintf(ofp, "%s\n", suffix.c_str());
        }
        else
        {
            fprintf(ofp, "%s", prefix.c_str());
            for (size_t j = 0; j < result.size(); ++j)
                fprintf(ofp, " %s", result[j].c_str());
            if (!suffix.empty())
                fprintf(ofp, " %s", suffix.c_str());
            fprintf(ofp, "\n");
        }
    }
    explain_fflush_or_die(ofp);
    if (ofp != stdout)
        explain_fclose_or_die(ofp);
}


void
language::lex_open(const rcstring &infile)
{
    if (infile == "-")
    {
        lex_stack.push_back(context::create("stdin", stdin));
    }
    else
    {
        FILE *ifp = view_fopen(infile, "r");
        lex_stack.push_back(context::create(infile, ifp));
    }
}


bool
language::lex_open_include(const rcstring &infile)
{
    if (infile[0] == '/')
    {
        if (include_files.member(infile))
            return true;
        include_files.push_back(infile);
        if (view_access(infile, F_OK) != 0)
            return false;
        lex_open(infile);
    }

    if (include_path.empty())
        include_path.push_back(".");
    for (size_t j = 0; j < include_path.size(); ++j)
    {
        rcstring dir = include_path[j];
        rcstring path = path_join(dir, infile);
        if (include_files.member(path))
            return true;
        if (view_access(path, F_OK) != 0)
            continue;
        include_files.push_back(path);
        if (recursive)
            lex_open(path);
        return true;
    }

    rcstring dir = include_path[0];
    rcstring path = path_join(dir, infile);
    include_files.push_back_unique(path);

    return false;
}


int
language::lex_getc(void)
{
    for (;;)
    {
        if (lex_stack.empty())
            return EOF;
        context::pointer cp = lex_stack.back();
        assert(cp);
        int c = cp->getch();
        if (c != EOF || lex_stack.size() == 1)
            return c;
        lex_stack.pop_back();
    }
}


void
language::lex_ungetc(int c)
{
    if (c == EOF)
        return;
    assert(!lex_stack.empty());
    context::pointer cp = lex_stack.back();
    cp->ungetch(c);
}


bool
language::lex_read_one_line(rcstring &line)
{
    rcstring_accumulator ac;
    for (;;)
    {
        int c = lex_getc();
        switch (c)
        {
        case EOF:
            if (ac.empty())
                return false;
            // Fall through...

        case '\n':
            line = ac.mkstr();
            return true;
        }
        ac.push_back((unsigned char)c);
    }
}


location
language::get_lex_location(void)
    const
{
    assert(!lex_stack.empty());
    return lex_stack.back()->get_location();
}


// vim: set ts=8 sw=4 et :
