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

#ifndef UCSDPSYS_OPCODES_ARCH_ADAPTABLE_H
#define UCSDPSYS_OPCODES_ARCH_ADAPTABLE_H

#include <lib/ac/vector>

#include <ucsdpsys_opcodes/arch.h>

/**
  * The arch_adaptable class is used to represent the opcode format used
  * by the adaptable assembler.  Let's hope the didn't mess with it too
  * much each time they adapted it to a new microprocessor.
  */
class arch_adaptable:
    public arch
{
public:
    typedef boost::shared_ptr<arch_adaptable> pointer;

    /**
      * The destructor.
      */
    virtual ~arch_adaptable();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    arch_adaptable(mtype_t mtype);

    // See base class for documentation.
    void read_binary(const rcstring &filename);

    // See base class for documentation.
    void write_binary(const rcstring &filename) const;

    // See base class for documentation.
    void read_text(const rcstring &filename);

    // See base class for documentation.
    void write_text(const rcstring &filename) const;

    // See base class for documentation.
    void random_test_data(void);

private:
    enum atribute_t
    {
        defabs,
        procs,
        ops1,
        ops2,
        ops3,
        ops4,
        ops5,
        ops6,
        ops7,
        ops8,
        ops9,
        ops10,
        ops11,
        ops12,
        ops13,
        ops14,
        ops15,
        ops16,
        ops17,
        ops18,
        ops19,
        ops20,
        defrp,
        defreg,
        defcc,
        defir,
        defunknown,
        interps,
        publics,
        consts,
        privates,
        refs,
        defs,
        funcs,
        abs,
        labels,
        unknown,
        macros
    };

    bool atribute_from_name(const rcstring &name, atribute_t &value) const;

    struct oprec
    {
        rcstring opname;
        int opvalue;
        atribute_t opatrib;

        oprec(const rcstring &a_opname, int a_opvalue, atribute_t a_opatrib) :
            opname(a_opname),
            opvalue(a_opvalue),
            opatrib(a_opatrib)
        {
        }
    };

    typedef std::vector<oprec> opcodes_t;

    opcodes_t opcodes;

    virtual rcstring atribute_name(atribute_t) const;

    /**
      * The default constructor.  Do not use.
      */
    arch_adaptable();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    arch_adaptable(const arch_adaptable &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    arch_adaptable &operator=(const arch_adaptable &rhs);
};

#endif // UCSDPSYS_OPCODES_ARCH_ADAPTABLE_H
// vim: set ts=8 sw=4 et :
