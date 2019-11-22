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

#include <lib/ac/ctype.h>

#include <lib/output/filter/prefix.h>
#include <lib/output/filter/suffix.h>
#include <lib/output/filter/wrap.h>
#include <lib/output/wrap_cook.h>

#include <ucsdpsys_osmakgen/target/build/cook.h>


target_build_cook::~target_build_cook()
{
}


target_build_cook::target_build_cook(const rcstring &a_filename) :
    target_build(a_filename)
{
}


target_build_cook::pointer
target_build_cook::create(const rcstring &a_filename)
{
    return pointer(new target_build_cook(a_filename));
}


void
target_build_cook::wrap_comment(const rcstring &text)
{
    const char *cp = text.c_str();
    const char *end = cp + text.size();
    while (cp < end && isspace((unsigned char)*cp))
        ++cp;
    while (cp < end && isspace((unsigned char)end[-1]))
        --end;
    op->fputs("/*\n");
    {
        output::pointer op1 = output_filter_prefix::create(op, " * ");
        output::pointer op2 = output_filter_wrap::create(op1);
        op2->write(cp, end - cp);
    }
    op->fputs(" */\n");
}


static rcstring
fix(const rcstring &text)
{
    return text.replace("=", "\\=");
}


void
target_build_cook::wrap_assignment(const rcstring &name,
    const rcstring_list &value, bool)
{
    output::pointer op1 = output_wrap_cook::create(op);
    op1->fputs(name);
    op1->fputs(" =");
    for (size_t j = 0; j < value.size(); ++j)
    {
        op1->fputc(' ');
        op1->fputs(fix(value[j]));
    }
    if (value.empty())
        op1->fputc(' ');
    op1->fputs(";\n");
}


void
target_build_cook::write_rule(const rcstring &rtarget,
    const rcstring_list &ingredients, const rcstring_list &body)
{
    output::pointer wop = output_wrap_cook::create(op);
    wop->fputc('\n');
    wop->fputs(rtarget);
    wop->fputc(':');
    for (size_t j = 0; j < ingredients.size(); ++j)
    {
        wop->fputc(' ');
        wop->fputs(fix(ingredients[j]));
    }
    if (body.empty())
    {
        wop->fputs(";\n");
    }
    else
    {
        wop->fputs("\n{\n");
        for (size_t j = 0; j < body.size(); ++j)
        {
            wop->fputs("    ");
            wop->fputs(fix(body[j]));
            wop->fputs(";\n");
        }
        wop->fputs("}\n");
    }
}


void
target_build_cook::write_clean_files_rule(const rcstring &lhs,
    const rcstring_list &rhs, const rcstring_list &files)
{
    op->fputs(lhs);
    op->fputc(':');
    for (size_t j = 0; j < rhs.size(); ++j)
    {
        op->fputc(' ');
        op->fputs(rhs[j]);
    }
    if (files.empty())
    {
        op->fputs(";\n");
    }
    else
    {
        op->fputs("\n    set errok\n{\n");
        output::pointer op1 = output_filter_suffix::create(op, ";");
        output::pointer op2 = output_filter_prefix::create(op1, "    rm -f ");
        output::pointer op3 = output_filter_wrap::create(op2);
        for (size_t j = 0; j < files.size(); ++j)
        {
            if (j)
                op3->fputc(' ');
            op3->fputs(files[j]);
        }
        op3->fputc('\n');
    }
    op->fputs("}\n");
}


void
target_build_cook::write_clean_dirs_rule(const rcstring &lhs,
    const rcstring_list &rhs, const rcstring_list &dirs)
{
    op->fputs(lhs);
    op->fputc(':');
    for (size_t j = 0; j < rhs.size(); ++j)
    {
        op->fputc(' ');
        op->fputs(rhs[j]);
    }
    if (dirs.empty())
    {
        op->fputs(";\n");
    }
    else
    {
        op->fputs("\n    set errok\n{\n");
        output::pointer op1 = output_filter_suffix::create(op, ";");
        output::pointer op2 = output_filter_prefix::create(op1, "    rm -rf ");
        output::pointer op3 = output_filter_wrap::create(op2);
        for (size_t j = 0; j < dirs.size(); ++j)
        {
            if (j)
                op3->fputc(' ');
            op3->fputs(dirs[j]);
        }
        op3->fputc('\n');
    }
    op->fputs("}\n");
}


rcstring
target_build_cook::load(const rcstring &name)
    const
{
    return "[" + name + "]";
}


rcstring
target_build_cook::resolve(const rcstring &name)
    const
{
    return "[resolve " + name + "]";
}


rcstring
target_build_cook::shell(const rcstring &command)
    const
{
    return "[collect " + command + "]";
}


void
target_build_cook::cond_if_exists(const rcstring &filename)
{
    op->fputs("if [exists [resolve ");
    op->fputs(filename);
    op->fputs("]] then\n{\n");
}


void
target_build_cook::cond_if_not_exists(const rcstring &filename)
{
    op->fputs("if [not [exists [resolve ");
    op->fputs(filename);
    op->fputs("]]] then\n{\n");
}


void
target_build_cook::cond_if_eq(const rcstring &lhs, const rcstring &rhs)
{
    op->fputs("if [in ");
    op->fputs(lhs);
    op->fputs(" ");
    op->fputs(rhs);
    op->fputs("] then\n{\n");
}


void
target_build_cook::cond_else(void)
{
    op->fputs("}\nelse\n{\n");
}


void
target_build_cook::cond_endif(void)
{
    op->fputs("}\n");
}


rcstring
target_build_cook::extra_include_flags(void)
    const
{
    return "-J. -Jbl -Jblbl -Jblblbl";
}


void
target_build_cook::extra_file_header(void)
{
    op->fputs("set mkdir;\n");
    op->fputs("set unlink;\n");
    op->fputs("if [not [defined DESTDIR]] then DESTDIR = ;\n");
}


// vim: set ts=8 sw=4 et :
