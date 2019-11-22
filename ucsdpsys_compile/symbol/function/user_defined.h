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

#ifndef UCSDPSYS_COMPILE_SYMBOL_FUNCTION_USER_DEFINED_H
#define UCSDPSYS_COMPILE_SYMBOL_FUNCTION_USER_DEFINED_H

#include <lib/symbol/function.h>
#include <lib/segment/builder.h>

class translator_compile; // forward
class scope; // forward

/**
  * The symbol_function_user_defined class is used to represent a
  * function (or procedure) defined by the user as the program is
  * compiled.
  */
class symbol_function_user_defined:
    public symbol_function
{
public:
    typedef boost::shared_ptr<symbol_function_user_defined> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_function_user_defined();

private:
    /**
      * The constructor.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param scp
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lxlvl
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @param seg_num
      *     The segment number
      * @param proc_num
      *     The procedure number within the segment.
      * @param cntxt
      *     The translation context we are working within.
      */
    symbol_function_user_defined(const rcstring &name,
        const scope::pointer &scp, int lxlvl, bool seg_proc, int seg_num,
        int proc_num, translator_compile *cntxt);

    /**
      * The constructor.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param dec_loc
      *     The location of the fucntion declaration
      * @param scp
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lxlvl
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @param seg_num
      *     The segment number
      * @param proc_num
      *     The procedure number within the segment.
      * @param cntxt
      *     The translation context we are working within.
      */
    symbol_function_user_defined(const rcstring &name, const location &dec_loc,
        const scope::pointer &scp, int lxlvl, bool seg_proc, int seg_num,
        int proc_num, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param scp
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lxlvl
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @param seg_num
      *     The segment number
      * @param proc_num
      *     The procedure number within the segment.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create(const rcstring &name, const scope::pointer &scp,
        int lxlvl, bool seg_proc, int seg_num, int proc_num,
        translator_compile *cntxt);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param dec_loc
      *     The location of the fucntion declaration
      * @param scp
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lxlvl
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @param seg_num
      *     The segment number
      * @param proc_num
      *     The procedure number within the segment.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create(const rcstring &name, const location &dec_loc,
        const scope::pointer &scp, int lxlvl, bool seg_proc, int seg_num,
        int proc_num, translator_compile *cntxt);

    // See base class for documentation.
    expression::pointer function_call_expression_factory(
        const expression_list &rhs) const;

    // See base class for documentation.
    unsigned get_segment_number(void) const;

    // See base class for documentation.
    unsigned get_procedure_number(void) const;

    // See base class for documentation.
    void notify_forward_suspend(void);

    // See base class for documentation.
    void notify_forward_resume(void);

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The segment_number instance variable is used to remember the
      * number of the segment in which the code for this function is
      * stored.
      */
    unsigned segment_number;

    /**
      * The procedure_number instance variable is used to remember
      * the procedure (function) number of this function, within its
      * segment.
      */
    unsigned procedure_number;

    /**
      * The segbuilder instance variable is used to remember the segment,
      * between the FORWARD declaration and until is resumed.  This is
      * handled by the #notify_forward_suspend and #notify_forward_resume
      * methods.  This only applies for forward declared SEGMENT procedures
      * and functions, normal procedures and functions are unaffected.
      */
    segment_builder::pointer segbuilder;

    /**
      * The default constructor.  Do not use.
      */
    symbol_function_user_defined();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_function_user_defined(const symbol_function_user_defined &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_function_user_defined &operator=(
        const symbol_function_user_defined &);
};

#endif // UCSDPSYS_COMPILE_SYMBOL_FUNCTION_USER_DEFINED_H
// vim: set ts=8 sw=4 et :
