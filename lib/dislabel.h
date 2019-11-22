//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#ifndef LIB_DISLABEL_H
#define LIB_DISLABEL_H

#include <lib/symtab/template.h>

/**
  * The dislabel class is used to represent the set of labels for a
  * procedure being disassembled.
  *
  * The implementation currently uses a simple list, giving many
  * operations an O(n) behaviour.  But since UCSD programs tend to be
  * small, this is not seen as a serious problem.
  */
class dislabel
{
public:
    /**
      * The destructor.
      */
    virtual ~dislabel();

    /**
      * The constructor.
      */
    dislabel(const rcstring &prefix);

    void add(unsigned addr);

    void add(unsigned addr, const rcstring &suffix);

    rcstring get_name(unsigned addr) const;

private:
    rcstring prefix;
    symtab<rcstring> name_from_address;
    int ordering;

    /**
      * The default constructor.
      */
    dislabel();

    /**
      * The copy constructor.
      */
    dislabel(const dislabel &);

    /**
      * The assignment operator.
      */
    dislabel &operator=(const dislabel &);
};

#endif // LIB_DISLABEL_H
// vim: set ts=8 sw=4 et :
