//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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

#include <lib/ac/sys/stat.h>

#include <libexplain/chmod.h>
#include <libexplain/fclose.h>
#include <libexplain/fopen.h>

#include <lib/pascal_id.h>

#include <ucsdpsys_osmakgen/target/debian.h>


target_debian::~target_debian()
{
}


target_debian::target_debian(const rcstring &fn) :
    target(fn)
{
}


target_debian::pointer
target_debian::create(const rcstring &filename)
{
    return pointer(new target_debian(filename));
}


void
target_debian::generate(const rcstring_list &)
{
    mkdir("debian", 0755);
    generate_changelog();
    generate_compat();
    generate_control();
    generate_copyright();
    generate_rules();
    generate_install();
}


void
target_debian::generate_changelog(void)
{
    FILE *fp = explain_fopen_or_die("debian/changelog", "w");
    rcstring project_name = "ucsd-psystem-os";
    fprintf
    (
        fp,
        "%s (%s-1) unstable; urgency=low\n",
        project_name.c_str(),
        version.c_str()
    );
    fprintf(fp, "\n");

    rcstring description = "No commit comment.";
    fprintf(fp, "  * %s\n", description.c_str());
    fprintf(fp, "\n");

    rcstring developer_name = "Peter Miller";
    rcstring developer_email = "<pmiller@opensource.org.au>";
    time_t when = time(0);
    struct tm *tmp = localtime(&when);
    assert(tmp);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %z", tmp);
    fprintf
    (
        fp,
        " -- %s %s  %s\n",
        developer_name.c_str(),
        developer_email.c_str(),
        buffer
    );
    fprintf(fp, "\n");

    explain_fclose_or_die(fp);
}


void
target_debian::generate_compat(void)
{
    mkdir("debian", 0755);
    FILE *fp = explain_fopen_or_die("debian/compat", "w");
    fprintf(fp, "5\n");
    explain_fclose_or_die(fp);

    mkdir("debian/source", 0755);
    fp = explain_fopen_or_die("debian/source/format", "w");
    fprintf(fp, "1.0\n");
    explain_fclose_or_die(fp);
}


void
target_debian::generate_control(void)
{
    rcstring source_description = "UCSD Pascal Operating System";
    rcstring source_name = "ucsd-psystem-os";
    rcstring bin_name = source_name;

    FILE *fp = explain_fopen_or_die("debian/control", "w");
    fprintf(fp, "Source: %s\n", source_name.c_str());

    rcstring section = "utils";
    fprintf(fp, "Section: %s\n", section.c_str());

    rcstring priority = "optional";
    fprintf(fp, "Priority: %s\n", priority.c_str());

    rcstring maintainer = "Peter Miller <pmiller@opensource.org.au>";
    fprintf(fp, "Maintainer: %s\n", maintainer.c_str());

    rcstring homepage = "http://miller.emu.id.au/pmiller/ucsd-psystem-os/";
    fprintf(fp, "Homepage: %s\n", homepage.c_str());

    fprintf(fp, "DM-Upload-Allowed: yes\n");

    rcstring_list build_depends_list;
    build_depends_list.push_back_unique("debhelper (>= 5)");
    build_depends_list.push_back_unique("groff");
    build_depends_list.push_back_unique("ucsd-psystem-fs");
    build_depends_list.push_back_unique("ucsd-psystem-vm");
    build_depends_list.push_back_unique("ucsd-psystem-xc");
    build_depends_list.sort();
    fprintf
    (
        fp,
        "Build-Depends: %s\n",
        build_depends_list.unsplit(", ").c_str()
    );

    fprintf(fp, "Standards-Version: 3.9.1\n");

    fprintf(fp, "\n");
    fprintf(fp, "Package: %s\n", bin_name.c_str());
    fprintf(fp, "Architecture: any\n");

    rcstring bin_description = source_description;
    fprintf(fp, "Description: %s\n", bin_description.c_str());
    fprintf(fp, " This project is built from the original UCSD\n");
    fprintf(fp, " Pascal sources.  The result is system disk images\n");
    fprintf(fp, " that are completely legal.  They are NOT boot-leg\n");
    fprintf(fp, " proprietary disk images.\n");

    rcstring_list depends_list;
    depends_list.push_back("${shlibs:Depends}");
    depends_list.push_back("${misc:Depends}");
    depends_list.push_back_unique("ucsd-psystem-fs");
    depends_list.push_back_unique("ucsd-psystem-vm");
    depends_list.push_back_unique("ucsd-psystem-xc");
    depends_list.sort();
    fprintf(fp, "Depends: %s\n", depends_list.unsplit(", ").c_str());

    explain_fclose_or_die(fp);
}


void
target_debian::generate_install(void)
{
    rcstring project_name = "ucsd-psystem-os";
    rcstring filename = "debian/" + project_name + ".install";
    FILE *fp = explain_fopen_or_die(filename.c_str(), "w");
    fprintf(fp, "usr/share/%s\n", project_name.c_str());
    explain_fclose_or_die(fp);
}


void
target_debian::generate_copyright(void)
{
    FILE *fp = explain_fopen_or_die("debian/copyright", "w");
    fprintf
    (
        fp,
        "Copyright (C) 1979 Regents of the University of California\n"
        "All Rights Reserved\n"
        "\n"
        "Permission to use, copy, modify and distribute any part of this\n"
        "UCSD PASCAL for educational, research and non-profit purposes,\n"
        "without fee, and without a written agreement is hereby granted,\n"
        "provided that the above copyright notice, this paragraph and the\n"
        "following three paragraphs appear in all copies.\n"
        "\n"
        "Those desiring to incorporate UCSD PASCAL into commercial products\n"
        "or use for commercial purposes should contact the Technology\n"
        "Transfer & Intellectual Property Services, University of California,\n"
        "San Diego, 9500 Gilman Drive, Mail Code 0910, La Jolla, CA\n"
        "92093-0910, Ph: (858) 534-5815, Fax: (858) 534-7345,\n"
        "E-Mail: <invent@ucsd.edu>.\n"
        "\n"
        "IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY\n"
        "PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL\n"
        "DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF UCSD\n"
        "PASCAL, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF\n"
        "THE POSSIBILITY OF SUCH DAMAGE.\n"
        "\n"
        "THE SOFTWARE PROVIDED HEREIN IS ON AN\"AS IS\" BASIS, AND THE\n"
        "UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE,\n"
        "SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.  THE UNIVERSITY\n"
        "OF CALIFORNIA MAKES NO REPRESENTATIONS AND EXTENDS NO WARRANTIES\n"
        "OF ANY KIND, EITHER IMPLIED OR EXPRESS, INCLUDING, BUT NOT LIMITED\n"
        "TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A\n"
        "PARTICULAR PURPOSE, OR THAT THE USE OF UCSD PASCAL WILL NOT INFRINGE\n"
        "ANY PATENT, TRADEMARK OR OTHER RIGHTS.\n"
    );
    explain_fclose_or_die(fp);
}


void
target_debian::generate_rules(void)
{
    FILE *fp = explain_fopen_or_die("debian/rules", "w");
    fprintf
    (
        fp,
        "#!/usr/bin/make -f\n"
        "\n"
        "# Uncomment this to turn on verbose mode.\n"
        "#export DH_VERBOSE=1\n"
        "\n"
        "CFLAGS = -Wall -g\n"
        "ifneq (,$(findstring noopt,$(DEB_BUILD_OPTIONS)))\n"
        "CFLAGS += -O0\n"
        "else\n"
        "CFLAGS += -O2\n"
        "endif\n"
        "\n"
        "config.status: configure\n"
        "\tdh_testdir\n"
        "\tsh configure --prefix=/usr --localstatedir=/var \\\n"
        "\t\t--sysconfdir=/etc --mandir=/usr/share/man \\\n"
        "\t\tCFLAGS=\"$(CFLAGS)\" LDFLAGS=\"-Wl,-z,defs\"\n"
        "\n"
        "build: build-stamp\n"
        "\n"
        "build-stamp: config.status\n"
        "\tdh_testdir\n"
        "\t$(MAKE)\n"
        "\ttouch $@\n"
        "\n"
        "clean:\n"
        "\tdh_testdir\n"
        "\tdh_testroot\n"
        "\trm -f build-stamp\n"
        "\ttest ! -f Makefile || $(MAKE) distclean\n"
        "\tdh_clean\n"
        "\n"
        "install: build\n"
        "\tdh_testdir\n"
        "\tdh_testroot\n"
        "\tdh_clean -k\n"
        "\tdh_installdirs -A --list-missing\n"
        "\tmkdir -p $(CURDIR)/debian/tmp/usr/lib\n"
        "\tmkdir -p $(CURDIR)/debian/tmp/usr/share\n"
        "\t$(MAKE) DESTDIR=$(CURDIR)/debian/tmp install\n"
        "\trm -rf $(CURDIR)/debian/tmp/usr/share/man\n"
        "\n"
        "binary-indep: build install\n"
        "\n"
        "# Build architecture-dependent files here.\n"
        "binary-arch: build install\n"
        "\tdh_testdir\n"
        "\tdh_testroot\n"
        "\tdh_installchangelogs\n"
        "\tdh_installdocs -A\n"
        "\tdh_install --fail-missing --sourcedir=debian/tmp\n"
        // "\tdh_installman -A\n"
        "\tdh_compress -A\n"
        "\tdh_fixperms\n"
        "\tdh_installdeb\n"
        "\tdh_gencontrol\n"
        "\tdh_md5sums\n"
        "\tdh_builddeb\n"
        "\n"
        "binary: binary-indep binary-arch\n"
        "\n"
        ".PHONY: build clean binary-indep binary-arch binary install\n"
    );
    explain_fclose_or_die(fp);

    // make sure it is executable
    explain_chmod_or_die("debian/rules", 0755);
}


void
target_debian::set_version(const rcstring &text)
{
    version = text;
}


// vim: set ts=8 sw=4 et :
