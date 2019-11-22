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

#ifndef UCSDPSYS_COMPILE_LABEL_COMPILE_H
#define UCSDPSYS_COMPILE_LABEL_COMPILE_H

#include <lib/ac/stddef.h>
#include <lib/ac/list>

#include <lib/label.h>

class translator_compile; // forward

/**
  * The label_compile class is used to represent the code generation
  * needed to define and jump to labels.
  */
class label_compile:
    public label
{
public:
    typedef boost::shared_ptr<label_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~label_compile();

private:
    /**
      * The default constructor.
      * It is private on purpose, use the #create class method instead.
      */
    label_compile(translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(translator_compile *cntxt);

protected:
    // See base class for documentation.
    void define_here();

    // See base class for documentation.
    void goto_from_here();

    // See base class for documentation.
    void branch_from_here_if_false();

    // See base class for documentation.
    void branch_from_here_if_true();

    // See base class for documentation.
    void branch_from_here_if_equal();

    // See base class for documentation.
    void branch_from_here_if_not_equal();

    // See base class for documentation.
    void self_relative_from_here(void);

    // See base class for documentation.
    bool is_defined(void) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The address instance variable is used to remember the address of
      * this label.
      */
    unsigned address;

    /**
      * The defined instance variable is used to remember whether or
      * not this label has been defined yet.  This is important because
      * short backwards jumps go directly into the code.  Forwards
      * jumps, and long backwards jumps, go via the procedure jump
      * table.
      */
    bool defined;

    /**
      * The jump_table_index instance variable is used to remember the
      * jump table index for this address, or 0 if it has bot been set.
      */
    int jump_table_index;

    /**
      * The repair_length instance variable is used to remember how many
      * address slots have been used in the repair list array.
      */
    size_t repair_length;

    /**
      * The repair_maximum instance variable is used to remember how
      * many address slots have been allocated to the repair list array.
      */
    size_t repair_maximum;

    /**
      * The repair_list instance variable is used to remember the array
      * allocated from dynamic memory to hold the address repair list.
      */
    unsigned *repair_list;

    /**
      * The jump_from_here method is used to emit generic jump opcodes.
      *
      * @param opcode
      *     The jump opcode to append the offset to.
      */
    void jump_from_here(unsigned char opcode);

    /**
      * The repair_now method is used to insert the appropriate offets
      * on the end of jump opcodes.
      */
    void repair_now();

    /**
      * The repair_subscribe method is used to add the address to the
      * list of addresses which need to be poked with the appropriate
      * offset, once the adress of the label is known.
      *
      * @param address
      *     The address to be patched (plus one)
      */
    void repair_subscribe(unsigned address);

    std::list<unsigned> self_relative_repair;

    void self_relative_repair_subscribe(unsigned poke_addr);

    void self_relative_repair_now(void);

    /**
      * The copy constructor.  Do not use.
      */
    label_compile(const label_compile &);

    /**
      * The assignment operator.  Do not use.
      */
    label_compile &operator=(const label_compile &);
};

#endif // UCSDPSYS_COMPILE_LABEL_COMPILE_H
// vim: set ts=8 sw=4 et :
