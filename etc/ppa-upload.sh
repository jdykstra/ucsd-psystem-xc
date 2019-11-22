#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2010, 2012 Peter Miller
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at
# you option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>
#
set -e

project=ucsd-psystem-xc
wwwdir=archive

AEGIS_PROJECT=${project}.cur
export AEGIS_PROJECT

RELEASES="$*"
if [ -z "$RELEASES" ]
then
    RELEASES="`cat /home/archives/ubuntu.release.names`"
    if [ -z "$RELEASES" ]
    then
        RELEASES="lucid precise"
    fi
fi

PPA=ppa:pmiller-opensource/ppa

#find the project baseline
bl=`aegis -cd -bl`
dir=$bl/$wwwdir
f=`( cd $dir && ls *.tar.gz ) | tail -1`
if [ -z "$f" ]
then
    echo "can't find tarball" 1>&2
    exit 1
fi
tgz=$dir/$f

tdir=/tmp/${project}-ppa-$$

for release in $RELEASES
do
    cd /tmp
    rm -rf $tdir
    mkdir $tdir
    cd $tdir
    tar xzf $tgz
    cd ${project}-*

    PACKAGE=`head -1 debian/changelog | awk '{print $1}'`
    VERSION=`head -1 debian/changelog | awk '{print $2}' |
        sed -r -e 's/^\(//;s/\)$//'`

    sed -i -r -e "1s/\) [^;]+; /~pm~${release}) ${release}; /" debian/changelog
    head -1 debian/changelog
    dpkg-buildpackage -S -sa
    ls -lho ..
    dput $PPA ../${PACKAGE}_${VERSION}~pm~${release}_source.changes

    cd /tmp
    rm -rf $tdir
done
exit 0

# vim: set ts=8 sw=4 et :
