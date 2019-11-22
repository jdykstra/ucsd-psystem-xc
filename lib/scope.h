//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#ifndef LIB_SCOPE_H
#define LIB_SCOPE_H

#include <lib/ac/list>
#include <boost/shared_ptr.hpp>

#include <lib/bit_address.h>
#include <lib/expression.h>
#include <lib/link_info.h>
#include <lib/name_type/list.h>
#include <lib/statement.h>
#include <lib/symbol/variable.h>
#include <lib/symtab/template.h>

class code_source_map; // forward
class symbol_functor; // forward
class variable_name_list; // forward
class symbol_variable; // forward


/**
  * The scope class is used to represent an identifier nesting scope,
  * and also some aspects of what type of translation unit is being
  * parsed.
  */
class scope
{
public:
    /**
      * The pointer type is to be used for all pointers to scopes.
      */
    typedef boost::shared_ptr<scope> pointer;

    /**
      * The destructor.
      *
      * The destructor is private because you are required to go via the
      * refence_count_down method.  Only when the last reference is
      * gone will this instace be deleted.
      */
    virtual ~scope();

    /**
      * The get_name method is used to obtain the name of the symbol
      * corresponding to this scope.
      */
    rcstring get_name(void) const { return name; }

    /**
      * The define method is used to define a symbol.  The symbol's
      * name isn't used, because scope_stack has massaged it so that
      * symbols appear to be case-insensitive, etc, BUT the symbols are
      * remembered in the case the user defined them in.
      *
      * @param name
      *     The name to remember the symbol as
      * @param sp
      *     The symbol we are remembering into the symbol table.
      * @param isa_param
      *     indicates whether or not this is a parameter declaration
      */
    virtual void define(const rcstring &name, const symbol::pointer &sp,
        bool isa_param = false);

    /**
      * The define method is used to define a symbol.  The symbol's
      * name isn't used, because scope_stack has massaged it so that
      * symbols appear to be case-insensitive, etc, BUT the symbols are
      * remembered in the case the user defined them in.
      *
      * @param sp
      *     The symbol we are remembering into the symbol table.
      * @param isa_param
      *     indicates whether or not this is a parameter declaration
      */
    virtual void define(const symbol::pointer &sp, bool isa_param = false);

    /**
      * The define method is used to define a variable at a specific
      * frame location.
      *
      * @param sp
      *     The symbol to bind to this scope.
      * @param where
      *     The location of the symbol.  There is no need to allocate
      *     space for it, that has been done already.
      */
    virtual void define(const symbol::pointer &sp, const bit_address &where);

    /**
      * The lookup method is used to look for a symbol in this scope.
      * The scope_stack takes care of making things case-insensitive,
      * and walking the stack if the symbol is in a lower scope than
      * this one.
      *
      * @param name
      *     The name of the symbol to look for.
      * @returns
      *     pointer to the symbol on success, or the NULL pointer on failure
      */
    virtual symbol::pointer lookup(const rcstring &name) const;

    /**
      * The lookup_fuzzy method is used to look for a symbol in this scope.
      * The scope_stack takes care of making things case-insensitive,
      * and walking the stack if the symbol is in a lower scope than
      * this one.
      *
      * @param name
      *     The name of the symbol to look for.
      * @param best
      *     The fstrcmp level minimum before a symbol is "better",
      *     updated on return
      * @returns
      *     the name of the least fuzzy symbol, or the empty string if
      *     nothing is better
      */
    virtual rcstring lookup_fuzzy(const rcstring &name, double &best) const;

    /**
      * The traversal method is used to walk the scope, generating
      * the code, or what ever this translator is doing.  The default
      * implementation does nothing; you almost certainly need to
      * override it.
      */
    virtual void traversal(void) const;

    /**
      * The attach_function_parameters method is used to remember the function
      * parameter names and types, once translator::attach_function_parameters
      * has taken care of routine checks and allocations.
      *
      * The remembered types are used to validate (and in some cases
      * auto promote) parameters to functions calls, and the names give
      * us better error messages.
      *
      * @param ntlp
      *     The names and their types.
      */
    virtual void attach_function_parameters(name_type_list *ntlp);

    /**
      * The was_forward method is used to test if this scope was a
      * function, and was a forward declared function.  This is used by
      * the grammar to determine whether or not the function return type
      * is required.
      */
    virtual bool was_forward(void) const;

    /**
      * The set_forward method is used to tell a function symbol that it
      * is the subject of a forward declaration.
      */
    virtual void set_forward(bool ign_seg0);

    /**
      * The drop_locals method is used to tell a function symbol that
      * it may safely discard its local scope, as those symbols are no
      * longer needed.
      */
    virtual void drop_locals(void);

    /**
      * The get_lex_level_height method is used to determine the
      * lex_level increment of this level.  Typically this is either 1
      * (for functions and procedures), and 0 for everything else.
      */
    virtual int get_lex_level_height(void) const = 0;

    /**
      * The is_temporary method is used to determine whether or not this
      * scope is a temporary block scope (used by WITH statements, etc).
      * The default implementation returns false.
      */
    virtual bool is_temporary(void) const;

    /**
      * The begin_notify method is called just before the const, type,
      * var, nested procedures, and nested functions are parsed.
      */
    virtual void begin_notify(void);

    /**
      * The variable_factory method is used to ask the scope to create a
      * variable.  Rather than simply create symbol_variable instance, a more
      * derived type is created, based on whether it is a unit scope (privref)
      * or an intrinsic unit (extvar) or a "normal" variable.
      *
      * The variable is <b>not</b> entered into the scope's symbol table.
      *
      * @param name
      *     The name of the variable to be created.
      * @param tp
      *     The type of the variable to be created.
      * @param lex_level
      *     The lexical level of the symbol.
      * @returns
      *     a pointer to a new dynamically allocated variable instance.
      */
    virtual symbol_variable::pointer variable_factory(const variable_name &name,
        const type::pointer &tp, int lex_level) = 0;

    /**
      * The parameter_factory method is used to ask the scope to create
      * a new function or variable parameter.  Rather than simply create
      * symbol_variable instance, a more derived type is created.
      *
      * The variable is <b>not</b> entered into the scope's symbol table.
      *
      * @param name
      *     The name of the parameter to be created.
      * @param tp
      *     The type of the parameter to be created.
      * @param lex_level
      *     The lexical level of the symbol.
      * @returns
      *     a pointer to a new dynamically allocated parameter instance.
      */
    virtual symbol_variable::pointer parameter_factory(
        const variable_name &name, const type::pointer &tp, int lex_level) = 0;

    /**
      * The constant_factory method is used to ask the scope to create
      * a constant symbol.  Rather than simply create symbol_expression
      * instance, a more derived type is created, beased on whether it
      * is a unit scope (constdef) or a "normal" constant.
      *
      * The variable is <b>not</b> entered into the scope's symbol table.
      *
      * @param name
      *     The name of the variable to be created.
      * @param lex_level
      *     The lexical level of the symbol.
      * @param value
      *     The value of the constant
      * @returns
      *     a pointer to a new dynamically allocated variable instance.
      */
    virtual symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value) = 0;

    /**
      * The attach_return_type method is used to attach the return type
      * to the function.  The default implementation does nothing; you
      * almost certainly need to override it.
      *
      * @param tp
      *     The type to be attached.
      */
    virtual void attach_return_type(const type::pointer &tp);

    /**
      * The attach_body method is used to attach the code to the
      * scope.  The gives traversal something to work with.  The default
      * implementation does nothing; you almost certainly need to
      * override it.
      *
      * @param sp
      *     The statament to be attached.
      */
    virtual void attach_body(const statement::pointer &sp);

    /**
      * The attach_symbol method is used to bind this scope to the
      * corresponding symbol.
      *
      * @param sp
      *     The symbol in question.
      */
    virtual void attach_symbol(const symbol::pointer &sp);

    /**
      * The constant_declaration_notify method is used to let the
      * derived class know when a constant is defined, after it has been
      * stored into the symbol table, etc.
      *
      * @param name
      *     The name of the constant being defined
      * @param value
      *     The value of the constant being defined
      */
    virtual void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value) = 0;

    /**
      * The type_declaration_notify method is used to notify a scope
      * that a type declaration has been made.  This is called by the
      * type_declaration method after they symbol has been entered into
      * the symbol table.
      *
      * @param name
      *     The name of the typedef
      * @param tp
      *     The type being named.
      */
    virtual void type_declaration_notify(const variable_name &name,
        const type::pointer &tp) = 0;

    typedef boost::shared_ptr<symbol_variable> symbol_variable_p;

    /**
      * The variable_declaration_notify method is used to let the
      * derived class know when a variable is defined, after it has been
      * stored into the symbol table, etc.
      *
      * @param svp
      *     The symbol just defined.
      */
    virtual void variable_declaration_notify(const symbol_variable_p &svp) = 0;

    /**
      * The label_declaration_notify method is used to let the
      * derived class know when a label is defined, after it has been
      * stored into the symbol table, etc.
      *
      * @param names
      *     The label names that have been defined.
      */
    virtual void label_declaration_notify(const variable_name_list &names) = 0;

    /**
      * The get_sp method is used to gain access to the symbol attached
      * to, and being defined by, this scope.
      */
    virtual symbol::pointer get_sp(void) const;

    /**
      * The get_link_info_extproc method is used to obtain a suitable link_info
      * instance to be added to the segment's linker information.
      */
    virtual link_info::pointer get_link_info_extproc(void) const;

    /**
      * The do_not_search_past_this_point method is used by the
      * scope_stack class to decide whether or not to search further
      * down the stack.  Most of the time this is true (which is why
      * the default implementation returns true) but it is not true when
      * declaring records.
      */
    virtual bool do_not_search_past_this_point(void) const;

    /**
      * The will_accept_enums method is used to decide nwhether or not a
      * scope will accept enums.  If the top of stack won't, look deeper.
      * The default implementation always returns true.
      */
    virtual bool will_accept_enums(void) const;

    /**
      * The get_size_in_words method is used to obtain the size (the
      * maximum high water mark, not the current) of this scope, in
      * 16-bit words.
      */
    unsigned get_size_in_words(void) const;

    /**
      * The get_size_in_bytes method is used to obtain the size (the
      * maximum high water mark, not the current) of this scope, in
      * 8-bit bytes.
      */
    unsigned get_size_in_bytes(void) const;

    /**
      * The get_size_in_bits method is used to obtain the size (the
      * maximum high water mark, not the current) of this scope, in
      * bits.
      */
    unsigned get_size_in_bits(void) const;

    /**
      * The get_data_mark method is used to obtain the current data
      * space high water mark for the current scope.  It is used for
      * variant record declarations, and anonymous temporary variables
      * in statement implementations.
      */
    bit_address get_data_mark(void) const;

    /**
      * The set_data_mark method may be used to restore the data space
      * position.
      *
      * @param arg
      *     The data pace poition
      *
      * @note
      *     Thou shalt only pass values obtain from the get_data_mark
      *     method.  Thou shalt treat it as an opaque type.
      */
    void set_data_mark(const bit_address &arg);

    /**
      * The late binding method is used to resolve the types of record
      * members.  All other derivations do nothing.
      */
    virtual void late_binding(void);

    /**
      * The need_late_binding method may be used to determine whether or
      * not there are symbols in this scope with incompletely defined
      * types.
      */
    bool need_late_binding(void) const;

    /**
      * The check_defined method is used to check for undefined FORWARD
      * declared symbols.
      */
    void check_defined(void) const;

    /**
      * The symbol_table_walk method may be used to apply a functor to
      * each symbol in the symbol table.
      *
      * @param f
      *     The functor to apply to each symbol.
      */
    void symbol_table_walk(symbol_functor &f) const;

    /**
      * The allocate_space method is used to allocate space for
      * variables, paramaters or fields within the scope.  The scope's
      * allocation size will be advanced, and highwater adjusted if
      * necessary.
      *
      * @param tp
      *     The type of the variable.  This is used to determine now
      *     much space should be allocated.
      * @returns
      *     bit address of new variable.
      */
    virtual bit_address allocate_space(const type::pointer &tp);

    /**
      * The list_symbols method is used to generate a list of symbols.
      *
      * @param where
      *     When to send the listing.
      */
    virtual void list_symbols(code_source_map &where) const;

    /**
      * The is_a_unit method may be used to determine whenther or not a
      * scope is a unit (interface) scope.  Most are not.
      */
    virtual bool is_a_unit(void) const;

    virtual void unit_interface_begin(void);

    virtual void unit_interface_end(void);

    virtual void unit_implementation_begin(void);

    virtual void unit_implementation_end(void);

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param name
      *     The name of the scope.
      * @param packed
      *     whether or not the scope is using packed variable space allocation
      */
    scope(const rcstring &name, bool packed = false);

    /**
      * The notify_function_parameters method is used to perform
      * any translator specific actons on function parameters, once
      * attach_function_parameters has taken care of common checks and
      * allocations.
      *
      * @param ntl
      *     The names and their types.
      */
    virtual void notify_function_parameters(const name_type_list &ntl) = 0;

    /**
      * The pre_order_traversal method is called by the traversal method
      * before the statement body is traversed.
      */
    virtual void pre_order_traversal(void) const;

    /**
      * The post_order_traversal method is called by the traversal
      * method before the statement body is traversed.
      */
    virtual void post_order_traversal(void) const;

    /**
      * The get_body method is used to obtain the code body attached to
      * this scope.
      */
    statement::pointer get_body(void) const { return body; }

    /**
      * The get_param_size method is used to obtain the size of the
      * parameters section of this scope (e.g. for functions and
      * procedures).
      *
      * This is used to fill the "param size" field in the procedure
      * attributes table.
      *
      * @returns
      *     the size in bytes of the local variables
      */
    unsigned get_param_size_in_bytes(void) const;

    /**
      * The get_data_size method is used to obtain the size of the
      * data section of this scope (for programs, units, functions and
      * procedures).
      *
      * This is used to fill the "data size" field in the procedure
      * attributes table.
      *
      * @returns
      *     the size in bytes of the local variables
      */
    unsigned get_data_size_in_bytes(void) const;

    /**
      * The get_next_local_bit_offset method is used to obtain the bit
      * offset to the next available space for parameters and local
      * variables.
      */
    bit_address get_next_local_bit_offset(void) const;

    /**
      * The get_next_bit_address method is used to allocate space within
      * a record.
      */
    virtual bit_address get_next_bit_address(unsigned bit_width);

    /**
      * The reserve_param_space_in_bytes method is used to reserve
      * paramater space for the implicit return value local variable.
      *
      * @param nbytes
      *     The number of bytes to reserve
      */
    void reserve_param_space_in_bytes(unsigned nbytes);

    /**
      * The is_packed method is used to determine whether or not the
      * variables in this scope are to be packed closely together (true)
      * or word aligned (false).  The default implementation (correct
      * for all but record scopes) returns false.
      */
    bool is_packed(void) const { return packed; };

private:
    /**
      * The name instance variable is used to remember the name of the
      * segment, or function, etc.
      */
    rcstring name;

    typedef symtab<symbol::pointer> symbol_table_t;

    /**
      * The symbol_table instance variable is used to remember the types
      * and variables, etc, declared in this scope.
      */
    symbol_table_t symbol_table;

    /**
      * The sp instance variable is used to remember the symbol
      * associated with this scope (e.g. the procedure name).
      */
    symbol::pointer sp;

    /**
      * The body instance variable is used to remember the code tree
      * generated for this scope's body.
      */
    statement::pointer body;

    /**
      * The param_size instance variable is used to remember the number
      * of bytes used in parameters for this scope.
      * It also includes the space for the return value.
      *
      * frame_size - parameter_size = data_size
      */
    bit_address parameter_size;

    /**
      * The data_size_current instance variable is used to remember the
      * number of bytes used in local variables for this scope.
      */
    bit_address frame_size_current;

    /**
      * The data_size_highwater instance variable is used to remember
      * the maximum number of bytes used in local variables for this
      * scope.
      */
    bit_address frame_size_highwater;

    /**
      * The packed instance variable is used to remember whether or not
      * the variables in this scope are to be packed closely together
      * (true) or word aligned (false).
      */
    bool packed;

    typedef std::list<symbol::pointer> symbols_in_need_of_late_binding_t;

    /**
      * The symbols_in_need_of_late_binding instance variable is used to
      * keep track of symbols that need late binding of their types.
      */
    symbols_in_need_of_late_binding_t symbols_in_need_of_late_binding;

    /**
      * The default constructor.  Do not use.
      */
    scope();

    /**
      * The copy constructor.  Do not use.
      */
    scope(const scope &);

    /**
      * The assignment operator.  Do not use.
      */
    scope &operator=(const scope &);
};

#endif // LIB_SCOPE_H
// vim: set ts=8 sw=4 et :
