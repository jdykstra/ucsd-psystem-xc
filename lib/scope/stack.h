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

#ifndef LIB_SCOPE_STACK_H
#define LIB_SCOPE_STACK_H

#include <lib/ac/stddef.h>

#include <lib/bit_address.h>
#include <lib/expression.h>
#include <lib/link_info.h>
#include <lib/scope.h>
#include <lib/statement.h>
#include <lib/symbol.h>
#include <lib/type.h>
#include <lib/variant.h>

class name_type_list; // forward
class rcstring; // forward
class variable_name; // forward
class variable_name_list; // forward
class code_source_map; // forward
class symbol_variable; // forward

/**
  * The scope_stack class is used to represent a push-down stack of
  * symbol look-up scopes.  This implies an ordering for symbol lookups.
  */
class scope_stack
{
public:
    /**
      * The destructor.
      */
    virtual ~scope_stack();

    /**
      * The default constructor.
      */
    scope_stack();

    /**
      * The push_back method is used to push another scope on the top of
      * the stack.
      */
    void push_back(const scope::pointer &sp);

    /**
      * The pop_back method is used to remove the top item from the stack.
      */
    void pop_back(void);

    /**
      * The back method is used to obtain a pointer to the scope on
      * the top of the stack.  Use sparingly, usually a new method in
      * scope_stack is a better idea.
      *
      * @returns
      *     a pointer to the scope on the top of the stack, or NULL if
      *     the stack is empty.
      */
    scope::pointer back(void) const;

    /**
      * The push_front method is used to add a scope to the front of the
      * stack (where it cannot be declared in).
      *
      * Only to be used to move unit interfaces once defined.
      *
      * @param sp
      *     the interface scope of interest
      */
    void push_front(const scope::pointer &sp);

    /**
      * The define method is used to define a symbol in a symbol table.
      *
      * @param sp
      *     The symbol instance to be define, and managed by the symbol table.
      * @param isa_param
      *      wherter or not they symbol is a parameter
      */
    void define(const symbol::pointer &sp, bool isa_param);

    /**
      * The define method is used to define a symbol in a symbol table.
      *
      * @param sp
      *     The symbol instance to be define, and managed by the symbol table.
      * @param where
      *     The location within the stack frame.
      */
    void define(const symbol::pointer &sp, const bit_address &where);

    /**
      * The lookup method is used to look for a symbol in the symbol
      * table, by name.
      *
      * @param name
      *     The name of the symbol to look for.
      * @param from_p
      *     If this is not NULL, return the containing scope here.
      * @returns
      *     a prointer to the symbol on success, or the NULL pointer on
      *     failure.
      */
    symbol::pointer lookup(const rcstring &name, scope::pointer *from_p = 0)
        const;

    /**
      * The lookup_fuzzy method is used to look for a symbol in the
      * symbol table, by name, but very generously.
      *
      * @param name
      *     The name of the symbol to look for.
      * @param from_p
      *     If this is not NULL, return the containing scope here.
      * @returns
      *     a prointer to the symbol on success, or the NULL pointer on
      *     failure.
      */
    symbol::pointer lookup_fuzzy(const rcstring &name,
        scope::pointer *from_p = 0) const;

    /**
      * The begin_notify method is called just before the various scope
      * clauses are parsed.
      */
    void begin_notify(void);

    /**
      * The traversal method is used by translator::traversal to walk
      * the entire program.  It only traverses the bottom most scope,
      * assuming that all the others will be traversed recursively, or
      * have already been traversed.
      */
    void traversal(void) const;

    /**
      * The attach_function_parameters method is used by the translator
      * to notify the top-most scope of the associated function
      * parameters.  They have already been validated and added to the
      * symbol table.
      *
      * @param ntlp
      *     The list of names and types.
      */
    void attach_function_parameters(name_type_list *ntlp);

    /**
      * The attach_return_type method is used to attach the return type
      * the to top of the scope stack.
      *
      * @param tp
      *     The type to attach.
      */
    void attach_return_type(const type::pointer &tp);

    /**
      * The attach_body method is used to attach the body the to top of
      * the scope stack.
      *
      * @param sp
      *     The statement to attach.
      */
    void attach_body(const statement::pointer &sp);

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
    void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value);

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
    void type_declaration_notify(const variable_name &name,
        const type::pointer &tp);

    typedef boost::shared_ptr<symbol_variable> symbol_variable_p;

    /**
      * The variable_declaration_notify method is used to let the
      * derived class know when a variable is defined, after it has been
      * stored into the symbol table, etc.
      *
      * @param svp
      *     The variable symbol being defined
      */
    void variable_declaration_notify(const symbol_variable_p &svp);

    /**
      * The label_declaration_notify method is used to let the
      * derived class know when a label is defined, after it has been
      * stored into the symbol table, etc.
      *
      * @param names
      *     The label names that have been defined.
      */
    void label_declaration_notify(const variable_name_list &names);

    /**
      * The size method is used to obtain the number of scopes in the
      * stack.
      */
    size_t size(void) const { return length; }

    /**
      * The is_top_symbol method is used to determine whether or not the
      * given symbol is the symbol associated with the current top of
      * scope stack.
      *
      * @param sp
      *     The symbol to test
      * @returns
      *     boolean; true if is current function, false if not
      */
    bool is_top_symbol(const symbol::pointer &sp) const;

    /**
      * The function_is_on_scope_stack method is used to determine
      * whether or not the given symbol is suitable for a return value
      * assignment.
      *
      * @param sp
      *     The symbol to test
      * @returns
      *     boolean; true if suitable, false if not
      */
    bool function_is_on_scope_stack(const symbol::pointer &sp) const;

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
      * The current_function_definition_was_forward method is used to
      * test if this scope was a function, and was a forward declared
      * function.  This is used by the grammar to determine whether or
      * not the function return type is required.
      */
    bool current_function_definition_was_forward(void) const;

    /**
      * The set_forward method is used to tell the current function
      * symbol that it is the subject of a forward declaration.
      */
    void set_forward(bool ign_seg0);

    /**
      * The drop_locals method is used to tell the current function
      * symbol that it may safely discard its local scope, as those
      * symbols are no longer needed.
      */
    void drop_locals(void);

    /**
      * The check_defined method is used to check for undefined FORWARD
      * declared symbols.  Only the top of stack scope is checked.
      */
    void check_defined(void) const;

    /**
      * The get_lex_level method is used to calculate the lex-level of
      * the program.  It is a measure of function nesting depth,
      * made more complicated by, well, Pascal.
      */
    int get_lex_level(void) const { return lex_level_cache; }

    /**
      * The late_binding method may be used to see if the scope on the
      * top of the stack has enough information to resolve any of its
      * forward declared named types.  It is called after each type
      * declaration.
      */
    void late_binding(void);

    /**
      * The allocate_space method is used to look for a non-temporary
      * scope and allocate space within it.
      */
    bit_address allocate_space(const type::pointer &tp);

    /**
      * The list symbols method is used to generate a list of symbols
      * from the top-of-stack scope, into the given listing.
      */
    virtual void list_symbols(code_source_map &where) const;

    /**
      * The within_a_unit method may be used to determine if the current
      * parse context is within a unit definition, or a unit interface.
      * This is used to prohibit segment functions and procedures in
      * these contexts.
      */
    bool within_a_unit(void) const;

    /**
      * The get_link_info_extproc method is sued to obtaina suitable
      * link_info instance to be added to the segment's linker
      * information.
      */
    link_info::pointer get_link_info_extproc(void) const;

    /**
      * The variable_factory method is used to ask the scope to create a
      * variable.  Rather than simply create symbol_variable instance, a more
      * deried type is created, based on whether it is a unit scope (privref)
      * or an intrinsic unit (extvar) or a "normal" variable.
      *
      * The variable is <b>not</b> entered into the scope's symbol table.
      * The variable does not yet have a defined offset.
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
    symbol_variable::pointer variable_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    /**
      * The parameter_factory method is used to ask the scope to create
      * a parameter variable.  Rather than simply create symbol_variable
      * instance, a more deried type is created.
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
    symbol_variable::pointer parameter_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    /**
      * The constant_factory method is used to ask the scope to create a
      * constant symbol.
      *
      * The symbol is <b>not</b> entered into the scope's symbol table.
      *
      * @param name
      *     The name of the symbol to be created.
      * @param lex_level
      *     The lexical level of the symbol.
      * @param value
      *     The value of the constant.
      * @returns
      *     a pointer to a new dynamically allocated variable instance.
      */
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

    void unit_interface_begin(void);

    void unit_interface_end(void);

    void unit_implementation_begin(void);

    void unit_implementation_end(void);

private:
    /**
      * The length instance variable is used to remeber how much of the
      * dynamicvally allocated array has been used to date.
      */
    size_t length;

    /**
      * The maximum instance variable is used to remeber how marge a
      * space was dynamically allocated for the array.
      */
    size_t maximum;

    /**
      * The length instance variable is used to remeber the array of
      * pointers scopes.  There is a catch: it points to the element
      * immediately beyond the end of the used portion (length) of the
      * list, because this makes for fast [-1] top-of-stack operations.
      */
    scope::pointer *list;

    /**
      * The representation method is used to obtain a simple string
      * describing the scope stack (a list of names).  This can be
      * useful for debugging.
      */
    rcstring representation(void) const;

    /**
      * The lex_level_cache is used to remember the sum of the stack's
      * get_lex_level_height methods.  This makes #get_lex_level
      * faster, at the cost of slightly complicating the #push_back and
      * #pop_back methods.
      */
    int lex_level_cache;

    /**
      * The topmost_non_temporary method is used to obtain a pointer to
      * the top-most non-temporary scope on the stack.
      */
    scope::pointer topmost_non_temporary(void) const;

    /**
      * The topmost_will_accept_enums method is used to obtain a pointer to
      * the top-most scope on the stack that will_acceps_enums.
      */
    scope::pointer topmost_will_accept_enums(void) const;

    /**
      * The copy constructor.  Do not use.
      */
    scope_stack(const scope_stack &);

    /**
      * The assignment operator.  Do not use.
      */
    scope_stack &operator=(const scope_stack &);
};

#endif // LIB_SCOPE_STACK_H
// vim: set ts=8 sw=4 et :
