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

#ifndef UCSDPSYS_LINK_SEGREC_H
#define UCSDPSYS_LINK_SEGREC_H

#include <lib/ac/map>

#include <lib/codefile.h>

#include <ucsdpsys_link/workrec.h>

/**
  * The segrec class is used to represent a codefile segment.
  * Most of that job is already done by the #segment class,
  * however auxilliary information is required by the linker.
  */
class segrec
{
public:
    typedef boost::shared_ptr<segrec> pointer;

    /**
      * The destructor.
      */
    virtual ~segrec();

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param hostfile
      *     The codefile the segment appears in
      * @param sp
      *     The segment of interest.
      */
    segrec(const codefile::pointer &hostfile, const segment::pointer &sp);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param hostfile
      *     The codefile the segment appears in
      * @param sp
      *     The segment of interest.
      */
    static pointer create(const codefile::pointer &hostfile,
        const segment::pointer &sp);

    int get_segment_number(void) const;

    int get_codeaddr(void) const;

    int get_codeleng(void) const;

    int get_size_in_blocks(void) const;

    int get_length(void) const;

    rcstring get_src_filename(void) const;

    rcstring get_src_segname(void) const;

    int get_textstart(void) const;

    void symtab_insert(const link_info_entry::pointer &ep);

    link_info_entry::pointer symtab_lookup(const rcstring &name,
        link_info_type_t litype);

    segkind_t get_segkind(void) const;

    unsigned short fetch_word(const unsigned char *buf) const;

    const unsigned char *get_data(void) const;

    void set_srcfile(const codefile::pointer &fp, int segnum);

    int get_link_info_block(void) const;

    int get_seginfo_packed(void) const;

    /**
      * The get_mtype method is sued to obtain the machine type of this
      * segment.
      */
    mtype_t get_mtype(void) const;

    /**
      * The set_mtype method is used to update the machine type of the
      * segment, but only if it is more specific than the existing
      * machine type.
      */
    void set_mtype(mtype_t value);

    const link_info_list &
    get_link_info(void)
        const
    {
        return sp->get_link_info();
    }

    typedef std::map<rcstring, link_info_entry::pointer> symbol_table_t;

    // symbol table tree
    symbol_table_t symtab;

    /**
      * The read_source_segment function determines the final segment size
      * after adding in the external procedures and functions, allocates
      * enough area for the entire output code segment, memcpy()s in the
      * original code (or uses identity segment for sephost special case),
      * and splits the procedure dictionary off from the code.  For all
      * procedures to be linked, a new destbase position is assigned in the
      * segment and the new procedure number is set up in the procedure
      * dictionary.  The segment number field of the procedure dictionary
      * is also updated to the value of s.  All is ready to copy in the
      * separate procedures and functions.  The values for #out_data
      * and #out_data_len are set.
      */
    void read_source_segment(const workrec_list_t &work);

    /**
      * The copy_in_procedures function goes through procs list and
      * copies procedure bodies from the separate segments into
      * the destination code segment into locations set up in the
      * segrec::read_source_segment method.  If all goes right, we
      * should fill destination segment to the exact byte.  The procedure
      * dictionary is updated to show the procedures' positions.
      */
    void copy_in_procedures(const workrec_list_t &procs);

    /**
      * The fix_up_references function is called to search through reflists
      * and fix operand fields of P-code and native code to refer to the
      * resolved values.  If fix_all_refs is true, then all pointers in the
      * ref lists are used, otherwise the reference pointers are checked to
      * see if they occur in the procedures to be linked.
      */
    void fix_up_references(workrec_list_t &works, bool fix_all_refs,
        const workrec_list_t &procs);

    void write_map(FILE *map, const workrec_list_t &procs,
        const workrec_list_t &local, const workrec_list_t &other) const;

    void update_proc1_varsize(int nextbaselc);

    /**
      * The write_to_codefile function takes the finalized destination
      * segment and writes it to the output codefile.
      */
    void write_to_codefile(const codefile::pointer &cfp);

private:
    // source file of segment
    codefile::pointer srcfile;

    // source file segment number
    segment::pointer sp;

    mutable unsigned char *in_data;
    unsigned char *out_data;
    size_t out_data_len;
    int nextspot;

    /**
      * The read_and_split method arranges for the source segment to be
      * placed in room allocated for #out_data.  This may involve a memcpy
      * or perhaps only creating an empty segment.  In any case #out_data
      * points at lowest addr, and #nextspot is pointed at the next place
      * code can be copied into.  This is used for destbase assignment in
      * the #read_source_segment method.
      */
    void read_and_split(void);

    /**
      * The default constructor.  Do not use.
      */
    segrec();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    segrec(const segrec &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    segrec &operator=(const segrec &rhs);
};

#endif // UCSDPSYS_LINK_SEGREC_H
// vim: set ts=8 sw=4 et :
