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

#ifndef LIB_SYMBOL_FUNCTION_H
#define LIB_SYMBOL_FUNCTION_H

#include <lib/expression.h>
#include <lib/scope.h>
#include <lib/symbol.h>
#include <lib/name_type/list.h>

class expression_list; // forward
class translator; // forward

/**
  * The symbol_function class is used to represent a symbol refering to
  * the whole function being compiled.
  */
class symbol_function:
    public symbol
{
public:
    typedef boost::shared_ptr<symbol_function> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_function();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param locals
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lex_level
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      */
    symbol_function(const rcstring &name, const scope::pointer &locals,
        int lex_level, bool seg_proc = false);

    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param dec_loc
      *     The declaration location of the function.
      * @param locals
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lex_level
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      */
    symbol_function(const rcstring &name, const location &dec_loc,
        const scope::pointer &locals, int lex_level, bool seg_proc = false);

public:
    /**
      * The attach_return_type is used to update the function with its
      * return type, which is parsed long after the function has been
      * defined.
      */
    void attach_return_type(const type::pointer &tp);

    /**
      * The function_call_expression_factory method is used to have the
      * function (name) create a suitable function call expression node.
      * This will usually be a user function or procedure, but it could
      * equally be one of several types of system builtin functions.
      *
      * The implementation is reponsible for all parameter count and type
      * checking, and all parameter type promotions.
      *
      * @param rhs
      *     The parameters given to the function
      * @returns
      *     a pointer to a new expression instance
      */
    virtual expression::pointer function_call_expression_factory(
        const expression_list &rhs) const = 0;

    /**
      * The attach_function_parameters method is used to attaches the
      * names and types of the parameters of the function.
      *
      * @param ntl
      *     The name and types
      */
    void attach_function_parameters(const name_type_list &ntl);

    /**
      * The is_forward method may be used during parsing to determine
      * whether a function has been declared forward, but not yet defined.
      */
    bool is_forward() const { return forward_decl; }

    /**
      * The get_locals method is used by the translator at compile time
      * to obtain the scope of local variables, so that it may be pushed
      * onto the scope stack.
      *
      * This method calls #notify_forward_resume when a function
      * declared FORWARD resumes definition.
      */
    scope::pointer get_locals(void);

    /**
      * The notify_forward_resume is used to tell derived classes when a
      * function declared FORWARD resumes definition.  It is called by
      * the #get_locals method.  The default implementation does nothing.
      */
    virtual void notify_forward_resume(void);

    /**
      * The set_forward method is used to tell a function symbol that it
      * is the subject of a forward declaration.
      *
      * This method calls #notify_forward_suspend when a function
      * declared FORWARD suspends definition.
      */
    void set_forward(bool ign_seg0);

    /**
      * The notify_forward_suspend is used to tell derived classes when a
      * function declared FORWARD suspends definition.  It is called by
      * the #set_forward method.  The default implementation does nothing.
      */
    virtual void notify_forward_suspend(void);

    /**
      * The drop_locals method is used to tell a function symbol that
      * it may safely discard its local scope, as those symbols are no
      * longer needed.
      */
    void drop_locals(void);

    /**
      * The segment_procedure instance variable is used to remember
      * whether or not this is a segment procedure.  True if this is a
      * segment procedure, false if is a normal procedure.
      */
    bool is_segment_procedure(void) const { return segment_procedure; }

    // See base class for documentation.
    const char *get_kind() const;

    /**
      * The get_procedure_number method is used to obtain the procedure number
      * of this function.  This is used when generating link_info records.
      */
    virtual unsigned get_procedure_number(void) const;

protected:
    /**
      * The check_parameters method is used to check the number and types
      * of the function parameters, to be sure they match the function
      * declaration.
      *
      * @param rhs
      *     The expressions of the call to the fucntion.  Note that
      *     rhs[0] is expected to be the function name.
      * @param xlat
      *     The translation context this is happening within.
      * @returns
      *     a new expression list is returned.
      *     If there was any error, the returned list will be empty.
      * @note
      *     Pascal calls them parameters, so we must also.
      */
    expression_list check_parameters(const expression_list &rhs,
        translator &xlat) const;

    /**
      * The parameter_error_wrong_number method is used to isse an error
      * about the number of parameters passed to a procedure or function.
      *
      * @param given
      *     The list of expressions given; used for its location and its size
      * @param requires
      *     The correct number of parameters
      */
    void parameter_error_wrong_number(const expression_list &given,
        int requires) const;

    /**
      * The parameter_error_wrong_type method is used to issue and error
      * about type type of a particular parameter.
      *
      * @param param_number
      *     The number of the parameter being cvomplained about,
      *     counting from the left starting at one.
      * @param given
      *     The actual expression given, used for its location and its type.
      * @param requires
      *     The actual type that was needed.
      */
    void parameter_error_wrong_type(int param_number,
        const expression::pointer &given, const type::pointer &requires) const;

    /**
      * The get_parameter method is used to obtain the name and type of
      * the given parameter.
      *
      * @param n
      *     The parameter number (zero based, and not counting the
      *     function name)
      */
    const name_type &get_parameter(size_t n) const { return parameters[n]; }

    /**
      * The get_parameter_count method is used to obtain the number of
      * parameters the function was declaraed to take.
      */
    size_t get_parameter_count(void) const { return parameters.size(); }

    // See base class for documentation.
    void check_defined(void);

private:
    /**
      * The parameters instance variable is used to remember the
      * names and types of the function araguments from the function
      * declaration.  The types are used for validation of function call
      * parameters, and the names give better error messages.
      */
    name_type_list parameters;

    /**
      * The forward_decl instance variable is used to remember when a
      * function is the subject of a forward declaration.  (Once the
      * forward declaration is actually defined, this will once again be
      * false.)
      */
    bool forward_decl;

    /**
      * The locals instance variable is used to remember symbols
      * subordinate to this function.  Only non-null during parsing of
      * this function and its nested functions.
      */
    scope::pointer locals;

    /**
      * The check_string_size_match method is used to detect the case
      * where a funtion with a string parameters is called, and the
      * largest string the caller could pass will exceed the declared
      * size if the function parameter.  It's only a warning, but it
      * informs the user that teher is a possibility of a run-time error
      * occuring when the call-by-value string is copied into its place
      * in the callee's stack frame.
      */
    void check_string_size_match(const name_type &nt,
        const expression::pointer &ep, int pnum) const;

    /**
      * The ignore_segment_zero instance variable is used to remember
      * whether or not to ignore forward declared procedures and
      * functions from segment zero.
      */
    bool ignore_segment_zero;

    /**
      * The segment_procedure instance variable is used to remember
      * whether or not this is a SEGMENT procedure.  It's here simply to
      * be able to detect mismatches wbetween procedure definitions and
      * theor FORWARD declarations.
      */
    bool segment_procedure;

    /**
      * The get_segment_number method may be used to obtain the segment
      * in which the code for this function is stored.
      */
    virtual unsigned get_segment_number(void) const;

    /**
      * The default constructor.  Do not use.
      */
    symbol_function();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_function(const symbol_function &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_function &operator=(const symbol_function &);
};

#endif // LIB_SYMBOL_FUNCTION_H
// vim: set ts=8 sw=4 et :
