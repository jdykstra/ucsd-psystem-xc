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

#include <lib/config.h>
#include <lib/ac/string.h>
#include <libexplain/output.h>

#include <lib/cardinal.h>
#include <lib/output/filter/prefix.h>
#include <lib/output/filter/wrap.h>
#include <lib/output/wrap_make.h>
#include <lib/pascal_id.h>
#include <lib/sizeof.h>

#include <ucsdpsys_osmakgen/collect.h>
#include <ucsdpsys_osmakgen/target/build/makefile.h>


target_build_makefile::~target_build_makefile()
{
}


target_build_makefile::target_build_makefile(const rcstring &a_filename) :
    target_build(a_filename)
{
    remember_dist_clean_file("Makefile");
}


target_build_makefile::pointer
target_build_makefile::create(const rcstring &a_filename)
{
    return pointer(new target_build_makefile(a_filename));
}


void
target_build_makefile::wrap_comment(const rcstring &text)
{
    output::pointer op1 = output_filter_prefix::create(op, "# ");
    output::pointer op2 = output_filter_wrap::create(op1);
    op2->fputs(text);
}


void
target_build_makefile::wrap_assignment(const rcstring &name,
    const rcstring_list &value, bool immed)
{
    output::pointer op1 = output_wrap_make::create(op);
    op1->fputs(name);
    op1->fputc(' ');
    if (immed)
        op1->fputc(':');
    op1->fputc('=');
    for (size_t j = 0; j < value.size(); ++j)
    {
        op1->fputc(' ');
        op1->fputs(value[j]);
    }
    op1->fputc('\n');
}


void
target_build_makefile::write_rule(const rcstring &rtarget,
    const rcstring_list &ingredients, const rcstring_list &body)
{
    output::pointer wop = output_wrap_make::create(op);
    wop->fputc('\n');
    wop->fputs(rtarget);
    wop->fputc(':');
    for (size_t j = 0; j < ingredients.size(); ++j)
    {
        wop->fputc(' ');
        wop->fputs(ingredients[j]);
    }
    wop->fputc('\n');
    if (!body.empty())
    {
        rcstring dir = rtarget.dirname();
        if (dir != ".")
        {
            wop->fputs("\t@mkdir -p ");
            wop->fputs(dir);
            wop->fputc('\n');
        }
        for (size_t j = 0; j < body.size(); ++j)
        {
            wop->fputc('\t');
            wop->fputs(body[j]);
            wop->fputc('\n');
        }
    }
}


void
target_build_makefile::write_clean_files_rule(const rcstring &lhs,
    const rcstring_list &rhs, const rcstring_list &files)
{
    op->fputs(lhs);
    op->fputc(':');
    for (size_t j = 0; j < rhs.size(); ++j)
    {
        op->fputc(' ');
        op->fputs(rhs[j]);
    }
    op->fputc('\n');

    if (!files.empty())
    {
        output::pointer op1 = output_filter_prefix::create(op, "\trm -f ");
        output::pointer op2 = output_filter_wrap::create(op1);
        for (size_t j = 0; j < files.size(); ++j)
        {
            if (j)
                op2->fputc(' ');
            op2->fputs(files[j]);
        }
        op2->fputc('\n');
    }
}


void
target_build_makefile::write_clean_dirs_rule(const rcstring &lhs,
    const rcstring_list &rhs, const rcstring_list &dirs)
{
    op->fputs(lhs);
    op->fputc(':');
    for (size_t j = 0; j < rhs.size(); ++j)
    {
        op->fputc(' ');
        op->fputs(rhs[j]);
    }
    op->fputc('\n');

    if (!dirs.empty())
    {
        output::pointer op1 = output_filter_prefix::create(op, "\trm -rf ");
        output::pointer op2 = output_filter_wrap::create(op1);
        for (size_t j = 0; j < dirs.size(); ++j)
        {
            if (j)
                op2->fputc(' ');
            op2->fputs(dirs[j]);
        }
        op2->fputc('\n');
    }
}


rcstring
target_build_makefile::load(const rcstring &name)
    const
{
    return "$(" + name + ")";
}


rcstring
target_build_makefile::shell(const rcstring &command)
    const
{
    return "$(shell " + command + ")";
}


void
target_build_makefile::cond_if_exists(const rcstring &filename)
{
    op->fputs("ifneq ($(wildcard ");
    op->fputs(filename);
    op->fputs("),)\n");
}


void
target_build_makefile::cond_if_not_exists(const rcstring &filename)
{
    op->fputs("ifeq ($(wildcard ");
    op->fputs(filename);
    op->fputs("),)\n");
}


void
target_build_makefile::cond_if_eq(const rcstring &lhs, const rcstring &rhs)
{
    op->fputs("ifeq (");
    op->fputs(lhs);
    op->fputs(",");
    op->fputs(rhs);
    op->fputs(")\n");
}


void
target_build_makefile::cond_else(void)
{
    op->fputs("else\n");
}


void
target_build_makefile::cond_endif(void)
{
    op->fputs("endif\n");
}


// vim: set ts=8 sw=4 et :
