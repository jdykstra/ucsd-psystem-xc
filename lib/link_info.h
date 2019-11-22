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

#ifndef LIB_LINK_INFO_H
#define LIB_LINK_INFO_H

#include <lib/ac/vector>
#include <boost/shared_ptr.hpp>

#include <lib/library_map_columns.h>
#include <lib/link_info_type.h>
#include <lib/opformat.h>
#include <lib/output.h>
#include <lib/rcstring.h>

class codefile; // forward

/**
  * The link_info class is used to represent an item of link
  * information.
  *
  * The question is, so we turn this into a class hierarchy?  If we wind
  * up implementing too many switches on the type_t, then yes.
  */
class link_info
{
public:
    typedef boost::shared_ptr<link_info> pointer;

    /**
      * The destructor.
      */
    virtual ~link_info();

    /**
      * The create class method is used to create new
      * dynamically allocated
      * instances of this class.
      *
      * @param name
      *     The name of the symbol
      * @param litype
      *     The link information type
      * @param word5
      *     whatever comes next
      * @param word6
      *     whatever comes next
      * @param word7
      *     whatever comes next
      */
    static pointer create(const rcstring &name, link_info_type_t litype,
        int word5 = 0, int word6 = 0, int word7 = 0);

    /**
      * The print_library_map method is used to print a human-readable
      * representation of the linker information.
      */
    void print_library_map(library_map_columns &columns) const;

    /**
      * The representation method is used to obtain a string containing
      * a text representation of the link_info.  Useful for debugging.
      */
    rcstring representation(void) const;

    /**
      * The serialize_size method is used to obtain the number of bytes
      * required to represent this linker information record.
      */
    unsigned serialize_size(void) const;

    /**
      * The serialize method is used to build the codefile representation
      * of this inker information record.
      *
      * @param data
      *     Where to write the representation
      * @param cfp
      *     The codefile the data will eventually be written to, so
      *     that we can use the codefile's endian information to write
      *     integer words into the data.
      * @returns
      *     The number of bytes written.  It returns the same number of
      *     bytes as #serialize_size does.
      */
    unsigned serialize(unsigned char *data, const codefile *cfp) const;

    /**
      * The is_eofmark method is used to determine whether or not this
      * link_info record is an end-of-file-mark record.
      */
    bool is_eofmark(void) const { return (litype == EOFMARK); }

    /**
      * The add_reference method is used to append another reference
      * address to a link info record.
      *
      * @param addr
      *     The address making the reference, the address to be patched later.
      */
    void add_reference(int addr);

    /**
      * The same_as method is used to determine whether or not two link
      * info records are the same as each other (i.e. they matchin in
      * both symbol name and type).
      *
      * @param other
      *     The link info record to compare against
      */
    bool same_as(const link_info &other) const;

    /**
      * The copy_references method is used to copy the references of one
      * link info record to the end of this link info record.
      */
    void copy_references(const link_info &other);

    bool has_references(void) const { return !references.empty(); }

    rcstring get_name(void) const { return name; }
    link_info_type_t get_litype(void) const { return litype; }
    opformat_t get_format(void) const { return format; }
    int get_nextlc(void) const { return nextlc; }
    int get_srcproc(void) const { return srcproc; }
    int get_nparams(void) const { return nparams; }
    int get_homeproc(void) const { return homeproc; }
    int get_icoffset(void) const { return icoffset; }
    int get_baseoffset(void) const { return baseoffset; }
    int get_constval(void) const { return constval; }
    int get_nwords(void) const { return nwords; }

    /**
      * The sanity_check method is used to ensure that the link_info
      * record has acceptable parameter values.
      *
      * It does not return if errors are found; instead is prints a
      * fatal error and exits.
      *
      * @param caption
      *     Context informatuion to be included in the error message(s).
      */
    void sanity_check(const rcstring &caption) const;

    typedef std::vector<int> references_t;
    const references_t &get_references(void) const { return references; }

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      */
    link_info(const rcstring &name, link_info_type_t litype, int word5,
        int word6, int word7);

    // all, except EOFMARK
    rcstring name;

    // all
    link_info_type_t litype;

    // CONSTREF:
    // GLOBREF:
    // PRIVREF:
    // PUBLREF:
    // SEPFREF:
    // SEPPREF:
    // UNITREF:
    // how to deal with the refs
    opformat_t format;

    // CONSTREF:
    // GLOBREF:
    // PRIVREF:
    // PUBLREF:
    // SEPFREF:
    // SEPPREF:
    // UNITREF:
    references_t references;

    // CONSTREF:
    // GLOBREF:
    // PRIVREF:
    // PUBLREF:
    // SEPFREF:
    // SEPPREF:
    // UNITREF:
    // size of private or nparams
    int nwords;

    // EXTFUNC:
    // EXTPROC:
    // SEPFUNC:
    // SEPPROC:
    // the procnum in source seg
    int srcproc;

    // EXTFUNC:
    // EXTPROC:
    // SEPFUNC:
    // SEPPROC:
    // words passed/expected
    int nparams;

    // GLOBDEF:
    // which procedure it occurs in
    int homeproc;

    // GLOBDEF:
    // its byte offset in pcode
    int icoffset;

    // PUBLDEF:
    // compiler assign word offset
    int baseoffset;

    // CONSTDEF:
    // user defined value
    int constval;

    // EOFMARK:
    // private variable allocation information
    int nextlc;

    void print_library_map_ref(output::pointer &os) const;

    /**
      * The default constructor.  Do not use.
      */
    link_info();

    /**
      * The copy constructor.  Do not use.
      */
    link_info(const link_info &);

    /**
      * The assignment operator.  Do not use.
      */
    link_info &operator=(const link_info &);
};

#endif // LIB_LINK_INFO_H
// vim: set ts=8 sw=4 et :
