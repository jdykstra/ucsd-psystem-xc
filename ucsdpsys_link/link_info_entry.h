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

#ifndef UCSDPSYS_LINK_LINK_INFO_ENTRY_H
#define UCSDPSYS_LINK_LINK_INFO_ENTRY_H

#include <lib/ac/list>
#include <boost/shared_ptr.hpp>

#include <lib/link_info.h>
#include <lib/opformat.h>
#include <lib/rcstring.h>

#include <lib/link_info_type.h>

class segrec; // forward

/**
  * The link_info_entry class is used to represent a link information
  * record off the end of a segment in the codefile.
  *
  * The common link_info class is used to hold most of the information,
  * however there is auxilliary information required by the linker, and
  * that is remembered here.
  */
class link_info_entry
{
public:
    typedef boost::shared_ptr<link_info_entry> pointer;

    /**
      * The destructor.
      */
    ~link_info_entry();

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param lip
      *     The link_info that we are working with.
      * @param origin
      *     The segment from which this link information originates.
      */
    link_info_entry(const link_info::pointer &lip, segrec *origin);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lip
      *     The link_info that we are working with.
      * @param origin
      *     The segment from which this link information originates.
      */
    static pointer create(const link_info::pointer &lip, segrec *origin);

    rcstring get_symbol_table_key(void) const;

    static rcstring get_symbol_table_key(const rcstring &name,
        link_info_type_t litype);

private:
    link_info::pointer lip;

public:
    rcstring get_name(void) const { return lip->get_name(); }
    link_info_type_t get_litype(void) const { return lip->get_litype(); }
    opformat_t get_format(void) const { return lip->get_format(); }
    bool has_references(void) const { return lip->has_references(); }
    int get_nextlc(void) const { return lip->get_nextlc(); }
    int get_srcproc(void) const { return lip->get_srcproc(); }
    int get_homeproc(void) const { return lip->get_homeproc(); }
    int get_nwords(void) const { return lip->get_nwords(); }
    int get_nparams(void) const { return lip->get_nparams(); }

    const link_info::references_t &
    get_references(void)
        const
    {
        return lip->get_references();
    }

    int get_icoffset(void) const { return lip->get_icoffset(); }
    int get_baseoffset(void) const { return lip->get_baseoffset(); }
    int get_constval(void) const { return lip->get_constval(); }

    void sanity_check(const rcstring &caption) const;

    segrec *origin;

    struct place_t
    {
        place_t();
        int destbase;
        int srcbase;
        int length;
        rcstring representation(void) const;
    };

    // EXTFUNC:
    // EXTPROC:
    // SEPFUNC:
    // SEPPROC:
    place_t place;

    rcstring representation(void) const;

private:
    /**
      * The default constructor.  Do not use.
      */
    link_info_entry();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialization.
      */
    link_info_entry(const link_info_entry &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    link_info_entry &operator=(const link_info_entry &rhs);
};

#endif // UCSDPSYS_LINK_LINK_INFO_ENTRY_H
// vim: set ts=8 sw=4 et :
