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

#ifndef LIB_TRANSLATOR_H
#define LIB_TRANSLATOR_H

#include <lib/ac/vector>

#include <lib/scope.h>
#include <lib/scope/stack.h>
#include <lib/expression/dispatcher/binary.h>
#include <lib/expression/dispatcher/unary.h>
#include <lib/label.h>
#include <lib/link_info.h>
#include <lib/segkind.h>
#include <lib/statement.h>
#include <lib/symbol/function.h>
#include <lib/tcontrol.h>
#include <lib/type/array.h>
#include <lib/type/enumerated.h>
#include <lib/type/record.h>
#include <lib/variant.h>

class case_list; // forward
class code_source_map; // forward
class expression_list; // forward
class name_type_list; // forward
class rcstring; // forward
class scope_stack; // forward
class statement_list; // forward
class symbol; // forward
class symbol_function; // forward
class type_list; // forward
class variable_name; // forward
class variable_name_list; // forward

/**
  * The translator abstract base class is used to represent something
  * you can do to or do with the in-memory parse tree representation
  * of a Pascal program.
  */
class translator
{
public:
    /**
      * The destructor.
      */
    virtual ~translator();

    /**
      * The default constructor.
      */
    translator();

    /**
      * The parse method is used to parse a TCL input into an in-memory
      * parse tree.
      */
    void parse(const rcstring &filename);

    /**
      * The define_builtin_symbols method is called by the grammar after
      * the first token has been read.  This means that control comments
      * like (*$U-*) have time to take effect.
      */
    void define_builtin_symbols(void);

    /**
      * The sizeof_expression method is used to handle SIZEOF
      * expressions from the grammar.
      *
      * @param arg
      *     The expression who's type we want the size of.
      * @returns
      *     An expression with the size as a constant.
      */
    expression::pointer sizeof_expression(
        const expression::pointer &arg);

    /**
      * The program_expression method exists solely to support the
      *
      *     EXIT(PROGRAM)
      *
      * statement.  Could Pascal get any more inconsistent?  It seems
      * that "program" is a synonym of whatever you called the program.
      * So the context will go spelunking in the name space, looking
      * for a symbol of type "program".  There will either be one (when
      * compiling a program) or none (when compiling a unit).
      *
      * Or does it get stranger than that, and exit segment 1 procedure 1,
      * no matter where it is compiled, even in a unit.  Yuck.
      */
    expression::pointer program_expression(void);

    /**
      * The new_array_type method is used to create a new array type,
      * after routine checks have been made.
      *
      * @param packed
      *     True if the array is a packed array, false if not.
      * @param index_type
      *     The subrange type to be used for the array index.
      *     This describes the array bounds.
      * @param subtype
      *     The type of the array elements.
      */
    type::pointer new_array_type(bool packed, const type::pointer &index_type,
        const type::pointer &subtype);

    /**
      * The new_array_type method is used to create a new array type,
      * after routine checks have been made.
      * This is implemented as multiple calls to the non-list method.
      *
      * @param packed
      *     True if the array is a packed array, false if not.
      * @param index_types
      *     The subrange types to be used for the array indexes.
      *     This describes the array bounds.
      * @param subtype
      *     The type of the array elements.
      */
    type::pointer new_array_type(bool packed, const type_list &index_types,
        const type::pointer &subtype);

    /**
      * The library_path class method is used to add another directory
      * to the search path for library code files.  You can call this as
      * many times as required.
      *
      * @param path
      *     The path of the directory.
      */
    static void library_path(const rcstring &path);

    /**
      * The sizeof_expression method is used to handle SIZEOF
      * expressions from the grammar.
      *
      * @param locn
      *     The location of the name in the source code.
      * @param arg
      *     The type we want the size of.
      * @returns
      *     An expression with the size as a constant.
      */
    expression::pointer sizeof_expression(const location &locn,
        const type::pointer &arg);

protected:
    /**
      * The sizeof_expression_factory method is used by the
      * sizeof_expression method after routine checks.
      *
      * @param arg
      *     The expression who's type we want the size of.
      * @returns
      *     An expression with the size as a constant.
      */
    virtual expression::pointer sizeof_expression_factory(
        const expression::pointer &arg);

    /**
      * The sizeof_expression_factory method is used by the
      * sizeof_expression method after routine checks.
      *
      * @param locn
      *     The location of the name in the source code.
      * @param arg
      *     The type we want the size of.
      * @returns
      *     An expression with the size as a constant.
      */
    virtual expression::pointer sizeof_expression_factory(
        const location &locn, const type::pointer &arg) = 0;

public:
    /**
      * The program_scope method is used to create a top-level program
      * scope, in response to a PROGRAM declaration at the start of the
      * program.
      *
      * The #program_scope_factory method is called to create
      * the actual program scope instance, as appropriate for the
      * translator.
      *
      * @param name
      *     The name of the program.
      * @note
      *     <b>NOT VIRTUAL</b>, see the #program_scope_factory method, below.
      */
    scope::pointer program_scope(const rcstring &name);

protected:
    /**
      * The program_scope_factory method is used to create a new
      * program scope instance.  It is called by the non-virtual
      * #program_scope method.
      *
      * @param name
      *     The name of the program.
      */
    virtual scope::pointer program_scope_factory(const rcstring &name) = 0;

    /**
      * The builtin_scope_factory method is used to create a new
      * builtin scope instance.  It is called by the non-virtual
      * #define_builtin_symbols method.
      */
    virtual scope::pointer builtin_scope_factory(void) = 0;

    /**
      * The compound_temporary_scope_factory method is used to create a
      * new compound statement temporary scope instance.  It is called
      * by the non-virtual #compound_scope_begin method.
      *
      * @param stk
      *     The scope stack, used to track the temporary variable size,
      *     and restore it when go out of scope.
      */
    virtual scope::pointer compound_temporary_scope_factory(scope_stack &stk)
        = 0;

    /**
      * The with_temporary_scope_factory method is used to create a
      * new WITH statement temporary scope instance.  It is called
      * by the non-virtual #with_scope_begin method.
      *
      * @param stk
      *     The scope stack, used to track the temporary variable size,
      *     and restore it when go out of scope.
      * @param base
      *     The base expression of the WITH statement
      * @param assign
      *     The assignment of the base pointer, or NULL if not required.
      */
    virtual scope::pointer with_temporary_scope_factory(scope_stack &stk,
        const expression::pointer &base, const statement::pointer &assign) = 0;

public:
    /**
      * The unit_scope method is used to create a top-level unit scope,
      * in response to a UNIT declaration at the start of the unit.
      *
      * The #unit_scope_factory method is called to create the actual
      * unit scope instance, as appropriate for the translator.
      *
      * @param name
      *     The name of the unit.
      * @note
      *     <b>NOT VIRTUAL</b>, see the #unit_scope_factory method, below.
      */
    scope::pointer unit_scope(const rcstring &name);

    /**
      * The unit_intrinsic_code method is used by the grammar to pass
      * the segment number for an intrinsic unit's code segment.
      *
      * @param segnum
      *     The unit segment number.
      */
    void unit_intrinsic_code(int segnum);

    /**
      * The unit_intrinsic_data method is used by the grammar to pass
      * the segment number for an intrinsic unit's data segment.
      *
      * @param segnum
      *     The unit segment number.
      */
    void unit_intrinsic_data(int segnum);

    /**
      * The unit_interface_text is called from the grammar to set
      * the interface text associated with the unit currently being
      * compiled.
      *
      * @param text
      *     The text falling between the INTERFACE and IMPLEMENTATION
      *     keywords (excluding both keywords).
      *     Case is preserved, as are comments.
      */
    virtual void unit_interface_text(const rcstring &text) = 0;

    virtual void unit_interface_begin(void);

    virtual void unit_interface_end(void);

    virtual void unit_implementation_begin(void);

    virtual void unit_implementation_end(void);

protected:
    /**
      * The unit_scope_factory method is used to create a new unit scope
      * instance.  It is called by the non-virtual #unit_scope method.
      *
      * @param name
      *     The name of the unit.
      * @param code_seg
      *     the INTRINSIC CODE segment number specified in the unit
      *     declaration.
      * @param data_seg
      *     the INTRINSIC DATA segment number specified in the unit
      *     declaration.
      * @param llht
      *     lex level height of the unit
      * @param segkind
      *     the kind of segment to create (UNITSEG, LINKED_INTRINS, etc)
      */
    virtual scope::pointer unit_scope_factory(const rcstring &name,
        int code_seg, int data_seg, int llht, segkind_t segkind) = 0;

    /**
      * The function_symbol_factory is used to create
      * translator-specific function objects.  They need to be
      * translator specific, because they need to know how to create
      * translator-specific function call expression nodes.
      *
      * @param name
      *     The name of the function
      * @param scp
      *     The scope this function defines.
      * @param lex_level
      *     The function nesting level.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @returns
      *     pointer to new function symbol in dynamic memory
      */
    virtual symbol::pointer function_symbol_factory(const rcstring &name,
        const scope::pointer &scp, int lex_level, bool seg_proc) = 0;

public:
    /**
      * The function_scope method is used to create a function scope, in
      * response to a FUNCTION or PROCEDURE declaration.
      *
      * After routine checks the #function_scope_factory method
      * is called to create the actual program scope instance, as
      * appropriate for the translator.
      *
      * @param name
      *     The name of the function or procedure.
      * @param is_func
      *     true for functions, false for procedures.  This is needed
      *     because this information is needed long before the function
      *     return type is available.
      * @param new_seg
      *     The new_seg argument indicates whether this function or
      *     procedure starts a new segment (if true) or adds another
      *     function to the current segment (if false).
      * @note
      *     <b>NOT VIRTUAL</b>, see the #function_scope_factory method,
      *     below.
      */
    scope::pointer function_scope(const rcstring &name, bool is_func,
        bool new_seg);

protected:
    /**
      * The function_scope_factory method is used to create a new
      * function scope instance.  It is called by the non-virtual
      * #function_scope method.
      *
      * @param name
      *     The name of the function or procedure.
      * @param is_func
      *     true for functions, false for procedures.  This is needed
      *     because this information is needed long before the function
      *     return type is available.
      * @param new_seg
      *     The new_seg argument indicates whether this function or
      *     procedure starts a new segment (if true) or adds another
      *     function to the current segment (if false).
      */
    virtual scope::pointer function_scope_factory(const rcstring &name,
        bool is_func, bool new_seg) = 0;

public:
    /**
      * The scope_attach_return_type method is used to attach the return
      * type to a function declaration.
      */
    void scope_attach_return_type(const type::pointer &tp);

    /**
      * The current_function_definition_was_forward method is used to
      * test if this scope was a function, and was a forward declared
      * function.  This is used by the grammar to determine whether or
      * not the function return type is required.
      */
    bool current_function_definition_was_forward(void) const;

    /**
      * The scope_to_be_continued method is used to tell a function
      * symbol that it is the subject of a forward declaration.  The
      * scope is not discarded, so that it may be used at a future time.
      *
      * @note
      *     You are only allowed to call one of scope_to_be_continued or
      *     #scope_end, but not both.
      */
    void scope_to_be_continued(void);

    void scope_part_begin(void);

    /**
      * The scope_end method is used to tell the translator that a
      * previously entered scope (produced by one of the scope factory
      * methods) has concluded.
      */
    virtual void scope_end(void);

protected:
    /**
      * The scope_end_external_link_info method is used to obtain
      * the link_info record correspending to the current EXTERNAL
      * function or procedure declaration.  It is called by the
      * #scope_end_external method.
      */
    link_info::pointer scope_end_external_link_info(void) const;

public:
    /**
      * The scope_end_external method is used to tell the translator
      * that a previously entered scope (produced by one of the scope
      * factory methods) has concluded, and that the associated symbol
      * is not defined in this compilation, it will have to be linked at
      * a later time.
      */
    virtual void scope_end_external(void);

    /**
      * The program_end method is called by the grammar just before the
      * parse ends (if it was successful).  The default implementation
      * does nothing.
      */
    virtual void program_end(void);

    /**
      * The scope_attach_function_parameters method is used by the
      * grammar to pass the list of parameters and their types.
      *
      * Because the value stack grows downwards, addresses have to
      * be allocated to function parameters in reverse, so that the
      * lowest numbered local variable corresponds to the last function
      * parameters, and so on.
      *
      * However, the pretty printer needs to print them in the fowrdas
      * direction, and the compiler needs to make several passes for
      * things like strings and lingints, etc, which is why the complete
      * list is presented.
      *
      * @param ntlp
      *     The list of names and types.
      * @note
      *     This list will be passed by the translator to the delete
      *     operator, eventually.  The grammar shall not free it.
      */
    void scope_attach_function_parameters(name_type_list *ntlp);

    /**
      * The constant_declaration method is used to instantiate a
      * constant into the current scope.
      *
      * @param name
      *     The constant name being defined
      * @param ep
      *     The value of the constant being defined
      */
    void constant_declaration(const variable_name &name,
        const expression::pointer &ep);

    /**
      * The type_declaration method is used to instantiate a
      * named type into the current scope.
      *
      * @param name
      *     The type name being defined
      * @param tp
      *     The type of the named type being defined
      */
    void type_declaration(const variable_name &name,
        const type::pointer &tp);

    /**
      * The type_lookup method is used by the parser to find a named
      * type.
      *
      * @param name
      *     The name of the type to look for
      * @param deferable
      *     true if the lookup can be deferred (will create a forward
      *     reference), false if it is an error for the type name to be
      *     undefined.
      * @returns
      *     The type in question, or an error type if not found.
      *     Either way, the grammer need do no checking.
      */
    type::pointer type_lookup(const rcstring &name, bool deferable);

    /**
      * The type_lookup method is used by the parser to find a named
      * type, with size qualifier.  The lookup cannot be deferred.
      * Only useful for string and integer types.
      *
      * @param name
      *     The name of the type to look for
      * @param size
      *     The size of the string or integer, e.g. STRING[107];
      * @returns
      *     The type in question, or an error type if not found.
      *     Either way, the grammer need do no checking.
      */
    type::pointer type_lookup(const rcstring &name,
        const expression::pointer &size);

    /**
      * The variable_declaration method is used to instantiate a variable
      * into the current scope.
      *
      * @param names
      *     The variable names being defined
      * @param tsp
      *     The type of the variables being defined
      */
    void variable_declaration(const variable_name_list &names,
        const type::pointer &tsp);

    /**
      * The scope_attach_body method is used to attach a main procedure
      * body to funtions, provedures and programs (etc).
      *
      * @param sp
      *     The statement to attach.
      */
    void scope_attach_body(const statement::pointer &sp);

    /**
      * The compound_statement_factory method is used to create a new
      * compound statement instance in dynamic memory.
      *
      * @param arg
      *     The statements which form the body of the compound statement.
      * @returns
      *     pointer to a statement in dynamic memory
      */
    virtual statement::pointer compound_statement_factory(
        const statement_list &arg);

public:
    /**
      * The expression_statement method is used to create a new
      * expression statement instance in dynamic memory.  After routine
      * checks, it calls the expression_statement_factory to create the
      * new instance.
      *
      * @param arg
      *     The expression to be evaluated as the statement's effect.
      * @returns
      *     pointer to a statement in dynamic memory
      */
    statement::pointer expression_statement(const expression::pointer &arg);

protected:
    /**
      * The expression_statement_factory method is used to create a new
      * expression statement instance in dynamic memory.
      *
      * @param arg
      *     The expression to be evaluated as the statement's effect.
      * @returns
      *     pointer to a statement in dynamic memory
      */
    virtual statement::pointer expression_statement_factory(
        const expression::pointer &arg);

public:
    /**
      * The if_statement method is used to create a new "if"
      * statement object instance.  After routine checks and implicit
      * promotions, it calls the if_statement_factory method.
      *
      * @param condition
      *     The controlling expression.
      * @param then_clause
      *     The action to perform if the condition is strue.
      * @param else_clause
      *     The action to perform if the condition if false.
      * @returns
      *     A pointer to a statement object.
      */
    statement::pointer if_statement(const expression::pointer &condition,
        const statement::pointer &then_clause,
        const statement::pointer &else_clause = statement::pointer());

protected:
    /**
      * The if_statement_factory method is used to create a new not
      * equal to expression object instance.
      *
      * @param condition
      *     The controlling expression
      * @param then_clause
      *     The statement to perform if the condition is true
      * @param else_clause
      *     The statement to perform if the condition is false
      * @returns
      *     A pointer to an expression object.
      */
    virtual statement::pointer if_statement_factory(
        const expression::pointer &condition,
        const statement::pointer &then_clause,
        const statement::pointer &else_clause) = 0;

public:
    /**
      * The while_statement method is used to create a new "while"
      * statement object instance.  After routine checks and implicit
      * promotions, it calls the while_statement_factory method.
      *
      * @param condition
      *     The loop control expression.
      * @param body
      *     The loop body.
      * @returns
      *     A pointer to a statement object.
      */
    statement::pointer while_statement(const expression::pointer &condition,
        const statement::pointer &body);

protected:
    /**
      * The while_statement_factory method is used to create a new
      * while statement object instance.
      *
      * @param condition
      *     The loop control expression.
      * @param body
      *     The loop body.
      * @returns
      *     A pointer to an statement object.
      */
    virtual statement::pointer while_statement_factory(
        const expression::pointer &condition,
        const statement::pointer &body) = 0;

public:
    /**
      * The until_statement method is used to create a new "until"
      * statement object instance.  After routine checks and implicit
      * promotions, it calls the until_statement_factory method.
      *
      * @param body
      *     The loop body.
      * @param condition
      *     The loop control expression.
      * @returns
      *     A pointer to a statement object.
      */
    statement::pointer until_statement(const statement_list &body,
        const expression::pointer &condition);

protected:
    /**
      * The until_statement_factory method is used to create a new until
      * statement object instance.
      *
      * @param body
      *     The loop body.
      * @param condition
      *     The loop control expression.
      * @returns
      *     A pointer to an statement object.
      */
    virtual statement::pointer until_statement_factory(
        const statement_list &body, const expression::pointer &condition) = 0;

    /**
      * The infinite_loop_factory method is used to create a new not
      * equal to expression object instance.
      *
      * @param body
      *     The loop body.
      * @returns
      *     A pointer to an statement object.
      */
    virtual statement::pointer infinite_loop_factory(
        const statement::pointer &body) = 0;

public:
    /**
      * The empty_statement_factory method is used to create a new
      * empty statement instance in dynamic memory.
      *
      * @returns
      *     pointer to a statement in dynamic memory
      */
    virtual statement::pointer empty_statement_factory(void);

    /**
      * The with_scope_begin method is used to inform the translator
      * that a WITH statament has begun.  This is usually used to
      * create an anonymous pointer variable that is dereferenced to
      * reach record members.
      *
      * @param ep
      *     The expression describing the variable
      * @returns
      *     The anonymous pointer variable assignment,
      */
    void with_scope_begin(const expression::pointer &ep);

    /**
      * The with_scope_end method is used ot inform the translator that
      * a WITH scope has ended.  This calls with_statement_factory
      * to build its result.
      *
      * @param ep
      *     The expression at the start of the with statement.
      * @param sp
      *     The subordinate statement.
      * @returns
      *     The statement that the WITH grammar will return,
      *     usually sp if no factory is required.
      */
    statement::pointer with_scope_end(const expression::pointer &ep,
        const statement::pointer &sp);

protected:
    /**
      * The with_statement_factory method is used to inform the translator that
      * a WITH scope is complete.
      *
      * @param ep
      *     The expression describing the variable
      * @param base_asign
      *     An assignment statement for the anonymous pointer variable,
      *     or NULL if none required.
      * @param body
      *     The subordinate statement.
      * @returns
      *     The statement that the WITH grammar will return,
      *     the default implementation returns sp with no changes.
      */
    virtual statement::pointer with_statement_factory(
        const expression::pointer &ep, const statement::pointer &base_asign,
        const statement::pointer &body) = 0;

public:
    /**
      * The writeln_statement_factory method is used to create a new
      * compound statement instance in dynamic memory.
      *
      * @param args
      *     The expressions which detail what the writeln statement
      *     is to print.
      * @param eoln
      *     True if end-of-line should be printed too (writeln),
      *     false if no end-of-line is to be printed (write).
      * @returns
      *     pointer to a statement in dynamic memory
      */
    virtual statement::pointer writeln_statement_factory(
        const expression_list &args, bool eoln) = 0;

    /**
      * The function_call_expression method is used to create
      * a new function call expression object instance.
      * The instance is created indirectly via the virtual
      * expression::function_call_expression_factory method via the
      * virtual symbol_function::function_call_expression_factory
      * method.
      *
      * @param fname
      *     The function name expression.
      * @param args
      *     The function parameter expressions.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer function_call_expression(
        const expression::pointer &fname, const expression_list &args);

    /**
      * The name_expression method is used to create a new name
      * expression object instance.  The instance is created via the
      * virtual name_expression_factory method.
      *
      * @param locn
      *     The location of the name in the source code.
      * @param name
      *     The name of the string.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer name_expression(const location &locn,
        const rcstring &name);

    /**
      * The and_expression method is used to create a new logical and
      * expression object instance, or a new bit-wise and expression
      * object instance.  After routine checks and implicit promotions,
      * it calls the logical_and_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the binary and.
      * @param rhs
      *     The right hand side of the binary and.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer and_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The logical_and_expression_factory method is used to create a
      * new logical and expression object instance.
      *
      * @param lhs
      *     The left hand side of the logical and.
      * @param rhs
      *     The right hand side of the logical and.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer logical_and_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The bitwise_and_expression_factory method is used to create a
      * new bitwise and expression object instance.
      *
      * @param lhs
      *     The left hand side of the bitwise and.
      * @param rhs
      *     The right hand side of the bitwise and.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer bitwise_and_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The or_expression method is used to create a new or expression
      * object instance.  After routine checks and implicit promotions,
      * it calls the logical_or_expression_factory method or the
      * bitwise_or_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the logical or.
      * @param rhs
      *     The right hand side of the logical or.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer logical_or_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The logical_or_expression_factory method is used to create a
      * new logical or expression object instance.
      *
      * @param lhs
      *     The left hand side of the logical or.
      * @param rhs
      *     The right hand side of the logical or.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer logical_or_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The bitwise_or_expression_factory method is used to create a
      * new bitwise or expression object instance.
      *
      * @param lhs
      *     The left hand side of the bitwise or.
      * @param rhs
      *     The right hand side of the bitwise or.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer bitwise_or_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The addition_expression method is used to create a new
      * addition expression object instance.  After routine checks and
      * implicit promotions, it calls the addition_expression_factory
      * method.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer addition_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The integer_plus_integer_expression_factory method is used to
      * create a new addition expression object instance.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_plus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_plus_real_expression_factory method is used to
      * create a new addition expression object instance.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_plus_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_plus_integer_expression_factory method is used to
      * create a new addition expression object instance.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_plus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_plus_real_expression_factory method is used to
      * create a new addition expression object instance.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_plus_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The set_union_expression_factory method is used to
      * create a new set union expression object instance.
      *
      * @param lhs
      *     The left hand side of the union.
      * @param rhs
      *     The right hand side of the union.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer set_union_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_plus_lint_expression method is used to create a new
      * addition expression object instance, for adding a "normal"
      * integer and a long integer.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer int_plus_lint_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The lint_plus_int_expression method is used to create a new
      * addition expression object instance, for adding a long integer
      * and a "normal" integer.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer lint_plus_int_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The lint_plus_lint_expression_factory method is used to create
      * a new addition expression object instance, for adding two sized
      * long integers.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer lint_plus_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The subtraction_expression method is used to create a new
      * subtraction expression object instance.  After routine checks and
      * implicit promotions, it calls the subtraction_expression_factory
      * method.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer subtraction_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The integer_minus_integer_expression_factory method is used to
      * create a new subtraction expression object instance.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_minus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_minus_real_expression_factory method is used to
      * create a new subtraction expression object instance.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_minus_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_minus_integer_expression_factory method is used to
      * create a new subtraction expression object instance.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_minus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_minus_real_expression_factory method is used to
      * create a new subtraction expression object instance.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_minus_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The set_difference_expression_factory method is used to
      * create a new set difference expression object instance.
      *
      * @param lhs
      *     The left hand side of the set difference.
      * @param rhs
      *     The right hand side of the set difference.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer set_difference_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_minus_lint_expression method is used to create a new
      * subtraction expression object instance, for subtracting a
      * "normal" integer and a long integer.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer int_minus_lint_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The lint_minus_int_expression method is used to create a new
      * subtraction expression object instance, for subtracting a long
      * integer and a "normal" integer.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer lint_minus_int_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The lint_minus_lint_expression_factory method is used to create
      * a new subtraction expression object instance, for subtracting
      * two sized long integers.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer lint_minus_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The multiplication_expression method is used to create
      * a new multiplication expression object instance.  After
      * routine checks and implicit promotions, it calls the
      * multiplication_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the multiplication.
      * @param rhs
      *     The right hand side of the multiplication.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer multiplication_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The modulo_expression method is used to create a new modulo
      * expression object instance.  After routine checks and implicit
      * promotions, it calls the #modulo_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the modulo.
      * @param rhs
      *     The right hand side of the modulo.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer modulo_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The modulo_expression_factory method is used to create a new modulo
      * expression object instance.
      *
      * @param lhs
      *     The left hand side of the modulo.
      * @param rhs
      *     The right hand side of the modulo.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer modulo_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_times_integer_expression_factory method is used to
      * create a new integer multiplication expression object instance.
      *
      * @param lhs
      *     The left hand side of the multiplication.
      * @param rhs
      *     The right hand side of the multiplication.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_times_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_times_real_expression_factory method is used to
      * create a new multiplication expression object instance.
      *
      * @param lhs
      *     The left hand side of the multiplication.
      * @param rhs
      *     The right hand side of the multiplication.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_times_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_times_integer_expression_factory method is used to
      * create a new multiplication expression object instance.
      *
      * @param lhs
      *     The left hand side of the multiplication.
      * @param rhs
      *     The right hand side of the multiplication.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_times_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_times_real_expression_factory method is used to
      * create a new real multiplication expression object instance.
      *
      * @param lhs
      *     The left hand side of the multiplication.
      * @param rhs
      *     The right hand side of the multiplication.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_times_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_times_lint_expression method is used to
      * create a new long integer multiplication expression object instance.
      *
      * @param lhs
      *     The left hand side of the multiplication, an INTEGER.
      * @param rhs
      *     The right hand side of the multiplication, an INTEGER[*].
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer int_times_lint_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The lint_times_int_expression method is used to
      * create a new long integer multiplication expression object instance.
      *
      * @param lhs
      *     The left hand side of the multiplication, an INTEGER[*].
      * @param rhs
      *     The right hand side of the multiplication, an INTEGER.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer lint_times_int_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The lint_times_lint_expression_factory method is used to
      * create a new long integer multiplication expression object instance.
      *
      * @param lhs
      *     The left hand side of the multiplication.
      * @param rhs
      *     The right hand side of the multiplication.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer lint_times_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The set_intersection_expression_factory method is used to
      * create a new set intersection expression object instance.
      *
      * @param lhs
      *     The left hand side of the set intersection.
      * @param rhs
      *     The right hand side of the set intersection.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer set_intersection_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The integer_division_expression method is used to create
      * a new integer_division expression object instance.  After
      * routine checks and implicit promotions, it calls the
      * integer_division_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the integer_division.
      * @param rhs
      *     The right hand side of the integer_division.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_division_expression(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The integer_div_integer_expression_factory method is used to
      * create a new integer div expression object instance.
      *
      * @param lhs
      *     The left hand side of the integer division.
      * @param rhs
      *     The right hand side of the integer division.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_div_integer_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs) = 0;

public:
    /**
      * The real_division_expression method is used to create
      * a new real_division expression object instance.  After
      * routine checks and implicit promotions, it calls the
      * real_division_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the real_division.
      * @param rhs
      *     The right hand side of the real_division.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_division_expression(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The integer_over_integer_expression_factory method is used to
      * create a new real division expression object instance.
      *
      * @param lhs
      *     The left hand side of the real division.
      * @param rhs
      *     The right hand side of the real division.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_over_integer_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The integer_over_real_expression_factory method is used to
      * create a new real division expression object instance.
      *
      * @param lhs
      *     The left hand side of the real division.
      * @param rhs
      *     The right hand side of the real division.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_over_real_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The real_over_integer_expression_factory method is used to
      * create a new real division expression object instance.
      *
      * @param lhs
      *     The left hand side of the real division.
      * @param rhs
      *     The right hand side of the real division.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_over_integer_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The real_over_real_expression_factory method is used to create a
      * new real division expression object instance.
      *
      * @param lhs
      *     The left hand side of the real division.
      * @param rhs
      *     The right hand side of the real division.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_over_real_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs) = 0;

    /**
      * The int_over_lint_expression method is used to create a new long
      * integer division expression object instance.
      *
      * @param lhs
      *     The left hand side of the division.
      * @param rhs
      *     The right hand side of the division.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer int_over_lint_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The lint_over_int_expression method is used to create a new long
      * integer division expression object instance.
      *
      * @param lhs
      *     The left hand side of the division.
      * @param rhs
      *     The right hand side of the division.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer lint_over_int_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The lint_over_lint_expression_factory method is used to create a
      * new long integer division expression object instance.
      *
      * @param lhs
      *     The left hand side of the division.
      * @param rhs
      *     The right hand side of the division.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer lint_over_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The ge_expression method is used to create a new greater than
      * or equal expression object instance.  After routine checks and
      * implicit promotions, it calls the ge_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer ge_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The array_ge_array_expression method is used to create a new
      * greater-than-or-equal expression object instance.  It calls the
      * #array_ge_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_ge_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_ge_string_expression method is used to create a new
      * greater-than-or-equal expression object instance.  It calls the
      * #array_ge_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_ge_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_ge_array_expression method is used to create a new
      * greater-than-or-equal expression object instance.  It calls the
      * #array_ge_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer string_ge_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_ge_array_expression_factory method is used to
      * create a new greater-than-or-equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer array_ge_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The boolean_ge_boolean_expression_factory method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer boolean_ge_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The char_ge_char_expression method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_ge_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The char_ge_string_expression method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_ge_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_ge_char_expression method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer string_ge_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The enum_ge_enum_expression_factory method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer enum_ge_enum_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_ge_integer_expression_factory method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_ge_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_ge_real_expression_factory method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_ge_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_ge_integer_expression_factory method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_ge_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_ge_real_expression_factory method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_ge_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The string_ge_string_expression_factory method is used to
      * create a new greater than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than or equal.
      * @param rhs
      *     The right hand side of the greater than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer string_ge_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The gt_expression method is used to create a new greater than
      * expression object instance.  After routine checks and implicit
      * promotions, it calls the gt_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer gt_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The array_gt_array_expression method is used to create a
      * new greater-than expression object instance.  It calls the
      * #array_gt_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_gt_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_gt_string_expression method is used to create a
      * new greater-than expression object instance.  It calls the
      * #array_gt_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_gt_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_gt_array_expression method is used to create a
      * new greater-than expression object instance.  It calls the
      * #array_gt_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer string_gt_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_gt_array_expression_factory method is used to create a
      * new greater-than expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer array_gt_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The boolean_gt_boolean_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer boolean_gt_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The char_gt_char_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_gt_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The char_gt_string_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_gt_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_gt_char_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer string_gt_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The enum_gt_enum_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer enum_gt_enum_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_gt_integer_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_gt_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_gt_real_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_gt_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_gt_integer_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_gt_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_gt_real_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_gt_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The string_gt_string_expression_factory method is used to
      * create a new greater than expression object instance.
      *
      * @param lhs
      *     The left hand side of the greater than.
      * @param rhs
      *     The right hand side of the greater than.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer string_gt_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The le_expression method is used to create a new less than or
      * equal expression object instance.  After routine checks and
      * implicit promotions, it calls the le_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer le_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The array_le_array_expression method is used to create a new
      * less-than-or_equal expression object instance.  It calls the
      * #array_le_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison
      * @param rhs
      *     The right hand side of the comparison
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_le_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_le_string_expression method is used to create a new
      * less-than-or-equal expression object instance.  It calls the
      * #array_le_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_le_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_le_array_expression method is used to create a new
      * less-than-or-equal expression object instance.  It calls the
      * #array_le_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer string_le_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_le_array_expression_factory method is used to create a
      * new less-than-or-equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer array_le_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The boolean_le_boolean_expression_factory method is used to
      * create a new less than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer boolean_le_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The char_le_char_expression method is used to create a new less
      * than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_le_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The char_le_string_expression method is used to create a new less
      * than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_le_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_le_char_expression method is used to create a new less
      * than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer string_le_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The enum_le_enum_expression_factory method is used to create a
      * new less than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer enum_le_enum_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_le_integer_expression_factory method is used to
      * create a new less than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_le_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_le_real_expression_factory method is used to create
      * a new less than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_le_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_le_integer_expression_factory method is used to create
      * a new less than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_le_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_le_real_expression_factory method is used to create a
      * new less than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_le_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The string_le_string_expression_factory method is used to create
      * a new less than or equal expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than or equal.
      * @param rhs
      *     The right hand side of the less than or equal.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer string_le_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The lt_expression method is used to create a new less than
      * expression object instance.  After routine checks and implicit
      * promotions, it calls the lt_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer lt_expression(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The array_lt_array_expression method is used to create a
      * new less-than expression object instance.  It calls the
      * #array_lt_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_lt_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_lt_string_expression method is used to create a
      * new less-than expression object instance.  It calls the
      * #array_lt_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_lt_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_lt_array_expression method is used to create a
      * new less-than expression object instance.  It calls the
      * #array_lt_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer string_lt_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_lt_array_expression_factory method is used to
      * create a new less-than expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer array_lt_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The boolean_lt_boolean_expression_factory method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer boolean_lt_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The char_lt_char_expression method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_lt_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The char_lt_string_expression method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_lt_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_lt_char_expression method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer string_lt_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The enum_lt_enum_expression_factory method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer enum_lt_enum_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_lt_integer_expression_factory method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_lt_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_lt_real_expression_factory method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_lt_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_lt_integer_expression_factory method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_lt_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The real_lt_real_expression_factory method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_lt_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The string_lt_string_expression_factory method is used to
      * create a new less than expression object instance.
      *
      * @param lhs
      *     The left hand side of the less than.
      * @param rhs
      *     The right hand side of the less than.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer string_lt_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The eq_expression method is used to create
      * a new equal to expression object instance.  After
      * routine checks and implicit promotions, it calls the
      * eq_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer eq_expression(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The boolean_eq_boolean_expression_factory method is used to
      * create a new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer boolean_eq_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The char_eq_char_expression method is used to create a new equal
      * to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_eq_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The char_eq_string_expression method is used to create a new
      * equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_eq_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_eq_char_expression method is used to create a new
      * equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer string_eq_char_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The enum_eq_enum_expression_factory method is used to
      * create a new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer enum_eq_enum_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_eq_integer_expression_factory method is used to
      * create a new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer integer_eq_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The record_eq_record_expression method is used to create a
      * new equal to expression object instance.  It calls the
      * #record_eq_record_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer record_eq_record_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The record_eq_record_expression method is used to create a
      * new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer record_eq_record_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The set_eq_set_expression method is used to create a
      * new equal to expression object instance.  It calls the
      * #set_eq_set_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer set_eq_set_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_eq_set_expression_factory method is used to create a
      * new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer set_eq_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The lint_eq_lint_expression_factory method is used to create a
      * new equal to expression object instance, of long integers.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer lint_eq_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_eq_lint_expression_factory method is used to create a
      * new equal to expression object instance, of long integers.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer int_eq_lint_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The int_eq_lint_expression_factory method is used to create a
      * new equal to expression object instance, of long integers.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer lint_eq_int_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_le_set_expression method is used to create a new less
      * than or equal to than expression object instance.  It calls the
      * #set_le_set_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer set_le_set_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_le_set_expression_factory method is used to create a
      * new less than or equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer set_le_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The lint_le_lint_expression_factory method is used to create a
      * new less than or equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer lint_le_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_le_lint_expression method is used to create a new less
      * than or equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer int_le_lint_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The lint_le_int_expression method is used to create a new less
      * than or equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer lint_le_int_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The set_lt_set_expression method is used to create a
      * new less than expression object instance.  It calls the
      * #set_le_set_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer set_lt_set_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_lt_set_expression method is used to create a new less
      * than expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer set_lt_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The lint_lt_lint_expression method is used to create a new less
      * than expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer lint_lt_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_lt_lint_expression method is used to create a new less
      * than expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer int_lt_lint_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The lint_lt_int_expression method is used to create a new less
      * than expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer lint_lt_int_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_gt_set_expression method is used to create a
      * new greater than expression object instance.  It calls the
      * #set_gt_set_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer set_gt_set_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_gt_set_expression_factory method is used to create a
      * new greater than to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer set_gt_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The lint_gt_lint_expression_factory method is used to create a
      * new greater than to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer lint_gt_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_gt_lint_expression method is used to create a new
      * greater than to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer int_gt_lint_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The lint_gt_int_expression method is used to create a new
      * greater than to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer lint_gt_int_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_ge_set_expression method is used to create a new greater
      * than or equal to expression object instance.  It calls the
      * #set_ge_set_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer set_ge_set_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_ge_set_expression_factory method is used to create a
      * new greater than or equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer set_ge_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The lint_ge_lint_expression_factory method is used to create a
      * new greater than or equal to expression object instance,
      * for comparing long integers.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer lint_ge_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_ge_lint_expression method is used to create a
      * new greater than or equal to expression object instance,
      * for comparing long integers.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer int_ge_lint_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The lint_ge_int_expression method is used to create a
      * new greater than or equal to expression object instance,
      * for comparing long integers.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer lint_ge_int_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The set_ne_set_expression method is used to create a
      * new not equal to expression object instance.  It calls the
      * #set_ne_set_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer set_ne_set_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The set_ne_set_expression_factory method is used to create a
      * new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer set_ne_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The lint_ne_lint_expression_factory method is used to create a
      * new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer lint_ne_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The int_ne_lint_expression_factory method is used to create a
      * new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer int_ne_lint_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The int_ne_lint_expression_factory method is used to create a
      * new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer lint_ne_int_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The record_ne_record_expression method is used to create a
      * new not equal to expression object instance.  It calls the
      * #record_ne_record_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer record_ne_record_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The record_ne_record_expression method is used to create a
      * new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer record_ne_record_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The array_eq_array_expression method is used to create a
      * new equal to expression object instance.  It calls the
      * #array_eq_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_eq_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_eq_string_expression method is used to create a
      * new equal to expression object instance.  It calls the
      * #array_eq_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_eq_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_eq_array_expression method is used to create a
      * new equal to expression object instance.  It calls the
      * #array_eq_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer string_eq_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_eq_array_expression_factory method is used to
      * create a new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer array_eq_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The integer_eq_real_expression_factory method is used to
      * create a new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_eq_real_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The real_eq_integer_expression_factory method is used to
      * create a new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_eq_integer_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The real_eq_real_expression_factory method is used to
      * create a new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_eq_real_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs) = 0;

    /**
      * The string_eq_string_expression_factory method is used to
      * create a new equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the equal to.
      * @param rhs
      *     The right hand side of the equal to.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer string_eq_string_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs) = 0;

public:
    /**
      * The ne_expression method is used to create a new not equal to
      * expression object instance.  After routine checks and implicit
      * promotions, it calls the ne_expression_factory method.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer ne_expression(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The array_ne_array_expression method is used to create a
      * new not equal to expression object instance.  It calls the
      * #array_ne_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_ne_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_ne_string_expression method is used to create a
      * new not equal to expression object instance.  It calls the
      * #array_ne_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer array_ne_string_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_ne_array_expression method is used to create a
      * new not equal to expression object instance.  It calls the
      * #array_ne_array_expression_factory method after routine checks.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    expression::pointer string_ne_array_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The array_ne_array_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to a new expression object.
      */
    virtual expression::pointer array_ne_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

    /**
      * The boolean_ne_boolean_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer boolean_ne_boolean_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs) = 0;

    /**
      * The char_ne_char_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_ne_char_expression(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The char_ne_char_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer char_ne_string_expression(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The char_ne_char_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer string_ne_char_expression(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The enum_ne_enum_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer enum_ne_enum_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_ne_integer_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_ne_integer_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs) = 0;

    /**
      * The integer_ne_integer_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer integer_ne_real_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The integer_ne_integer_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer real_ne_integer_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The integer_ne_integer_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_ne_real_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs) = 0;

    /**
      * The integer_ne_integer_expression_factory method is used to
      * create a new not equal to expression object instance.
      *
      * @param lhs
      *     The left hand side of the not equal to.
      * @param rhs
      *     The right hand side of the not equal to.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer string_ne_string_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs) = 0;

public:
    /**
      * The dereference_expression method is used to create a new
      * reference expression object instance.  After routine checks and
      * implicit promotions, it calls the dereference_expression_factory
      * method.
      *
      * @param arg
      *     The operand of the dereference.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer dereference_expression(
        const expression::pointer &arg);

protected:
    /**
      * The dereference_expression_factory method is used to create a
      * new dereference expression object instance.
      *
      * @param arg
      *     The operand of the dereference.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer dereference_expression_factory(
        const expression::pointer &arg) = 0;

public:
    /**
      * The address_of_expression_factory method is used to create a new
      * pointer expression object instance.  This is what the "address
      * of" operator (@) does... in C this is the unary "&" operator.
      *
      * @param arg
      *     The operand of the address-of.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer address_of_expression_factory(
        const expression::pointer &arg) = 0;

    /**
      * The boolean_expression_factory method is used to create a new
      * string constant expression object instance.
      *
      * @param locn
      *     The location in the source code of the constant.
      * @param value
      *     The value of the boolean.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer boolean_expression_factory(const location &locn,
        bool value) = 0;

    /**
      * The integer_expression_factory method is used to create a new
      * string constant expression object instance.
      *
      * @param locn
      *     The location in the source code of the constant.
      * @param value
      *     The value of the string.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_expression_factory(
        const location &locn, const rcstring &value) = 0;

    /**
      * The lint_expression_factory method is used to create a new
      * INTEGER[] constant expression object instance.
      *
      * @param locn
      *     The location in the source code of the constant.
      * @param value
      *     The value of the string.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer lint_expression_factory(
        const location &locn, const rcstring &value) = 0;

    /**
      * The negate_expression method is used to create a new negate
      * expression object instance.  It will invoke one of the negate
      * expression factories below.
      *
      * @param arg
      *     The expression to be negated.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer negate_expression(const expression::pointer &arg);

    /**
      * The unary_plus_expression method is used to create a new unary_plus
      * expression object instance.  It will invoke one of the unary_plus
      * expression factories below.
      *
      * @param arg
      *     The expression to be unary_plusd.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer unary_plus_expression(const expression::pointer &arg);

protected:
    /**
      * The integer_negate_expression_factory method is used to create a new
      * integer negate expression object instance.
      *
      * @param arg
      *     The integer expression to be negated.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer integer_negate_expression_factory(
        const expression::pointer &arg) = 0;

    /**
      * The real_negate_expression_factory method is used to create a new
      * real negate expression object instance.
      *
      * @param arg
      *     The real expression to be negated.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_negate_expression_factory(
        const expression::pointer &arg) = 0;

    /**
      * The lint_negate_expression_factory method is used to create a new
      * long integer negate expression object instance.
      *
      * @param arg
      *     The expression to be negated.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer lint_negate_expression_factory(
        const expression::pointer &arg) = 0;

public:
    /**
      * The not_expression method is used to create a new NOT expression
      * object instance, after routine checks.
      *
      * @param arg
      *     The expression to be negated.
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer not_expression(const expression::pointer &arg);

protected:
    /**
      * The logical_not_expression_factory method is used to create a
      * new negate expression object instance.
      *
      * @param arg
      *     The expression to be logically negated.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer logical_not_expression_factory(
        const expression::pointer &arg) = 0;

    /**
      * The bitwise_not_expression_factory method is used to create a
      * new negate expression object instance.
      *
      * @param arg
      *     The expression to be bitwise not-ed.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer bitwise_not_expression_factory(
        const expression::pointer &arg) = 0;

public:
    /**
      * The empty_set_expression method is used to create a new
      * empty set expression object instance, after routine checks.
      *
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer empty_set_expression(void);

protected:
    /**
      * The empty_set_expression_factory method is used to create a new
      * empty set expression object instance.
      *
      * @param locn
      *     The source file location of this expression node.
      * @param tp
      *     The type of the set.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer empty_set_expression_factory(
        const location &locn, const type::pointer &tp) = 0;

public:
    /**
      * The set_expression method is used to create a new set expression
      * object instance, after routine checks.
      *
      * @param lo
      *     The expression to be the low side of the range
      * @param hi
      *     The expression to be the high side of the range
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer set_expression(const expression::pointer &lo,
        const expression::pointer &hi);

    /**
      * The set_expression method is used to create a new set expression
      * object instance, after routine checks.
      *
      * @param singleton
      *     The expression to be the whole of the range
      * @returns
      *     A pointer to an expression object.
      */
    expression::pointer set_expression(const expression::pointer &singleton);

protected:
    /**
      * The set_expression_factory method is used to create a new set
      * expression object instance.
      *
      * @param lo
      *     The expression to be the low side of the range
      * @param hi
      *     The expression to be the high side of the range
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer set_expression_factory(
        const expression::pointer &lo, const expression::pointer &hi) = 0;

    /**
      * The set_expression_factory method is used to create a new set
      * expression object instance.
      *
      * @param singleton
      *     The expression to be the whole of the range
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer set_expression_factory(
        const expression::pointer &singleton) = 0;

public:
    /**
      * The char_expression_factory method is used to create a new
      * char constant expression object instance.
      *
      * @param locn
      *     The location of the constant
      * @param value
      *     The value of the char.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer char_expression_factory(const location &locn,
        unsigned char value) = 0;

    /**
      * The real_expression_factory method is used to create a new
      * real constant expression object instance.
      *
      * @param locn
      *     The location of the number in the source code.
      * @param value
      *     The value of the string.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_expression_factory(
        const location &locn, const rcstring &value) = 0;

    /**
      * The real_expression_factory method is used to create a new
      * real constant expression object instance.
      *
      * @param locn
      *     The location of the string constant in the source code.
      * @param value
      *     The value of the string.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer string_expression_factory(
        const location &locn, const rcstring &value) = 0;

    /**
      * The traversal method is used to walk the tree of declarations
      * (and, recursively, the statements and expressions).
      */
    void traversal(void) const;

    /**
      * The enable_grammar_debug method is used to turn on the
      * yacc/bison debug messages during parsing.  Almost always only of
      * interest during debugging.
      */
    void enable_grammar_debug(void);

public:
    /**
      * The call_noparam_func_if_necessary method is used to dereference
      * references (i.e. implicit pointer deferences as opposed to
      * explicit pointer dereferences).
      *
      * @param arg
      *     The expression to implicitly dereference
      * @returns
      *     an expression which is not a reference
      */
    expression::pointer call_noparam_func_if_necessary(
        const expression::pointer &arg);

    /**
      * The assignment_expression_factory method is used to perform
      * routine checks before calling the lhs->assgnment_expresion_
      * factory() method.  This method is intentionally <b>not</b>
      * virtual.
      *
      * We ask the left hand side to turn itself into an assignment.  We
      * don't do this as a virtual method of this translate base class
      * because the assignment will be specific to the left hand side
      * expression.  And the left hand side was manufactured by the
      * derived class, so there is ample opportunity for the derived
      * classes to seize control, if they need to.
      *
      * @param lhs
      *     The left hand side, the variable to assign into.
      * @param rhs
      *     The right hand side, the variable to assign into.
      * @returns
      *     an expression which perorms the assignment.
      */
    expression::pointer assignment_expression_factory(
        const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The real_from_integer_expression_factory method is used to build
      * an implcit real(integer) cast expression node.
      *
      * @param arg
      *     The integer expression to promote to a real value.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer real_from_integer_expression_factory(
        const expression::pointer &arg) = 0;

    /**
      * The paoc_from_string_expression_factory method is used to build
      * an implcit packed_array_of_char(string_constant) cast expression node.
      *
      * @param arg
      *     The constant string expression to promote to a packed array
      *     of char value.
      * @param tp
      *     The exact type of the resulting value.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer paoc_from_string_expression_factory(
        const expression::pointer &arg, const type::pointer &tp) = 0;

    /**
      * The writeln2_expression_factory method is used to build
      * an expression to describe a writeln(arg1:arg2) construct.
      *
      * @param value
      *     The first parameter.
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer writeln1_expression_factory(
        const expression::pointer &value) = 0;

    /**
      * The writeln2_expression_factory method is used to build
      * an expression to describe a writeln(arg1:arg2) construct.
      *
      * @param value
      *     The first parameter
      * @param width
      *     The second parameter
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer writeln2_expression_factory(
        const expression::pointer &value, const expression::pointer &width) = 0;

    /**
      * The writeln3_expression_factory method is used to build
      * an expression to describe a writeln(arg1:arg2:arg3) construct.
      *
      * @param value
      *     The first parameter
      * @param width
      *     The second parameter
      * @param precision
      *     The third parameter
      * @returns
      *     A pointer to an expression object.
      */
    virtual expression::pointer writeln3_expression_factory(
        const expression::pointer &value, const expression::pointer &width,
        const expression::pointer &precision) = 0;

    /**
      * The dot_expression method is used by the parser to handle a dot
      * (record field access) expression.  After routine
      * checks, the virtual lhs->dot_expression_factory is called.
      *
      * @param lhs
      *     The record expression to have its member accessed.
      * @param rhs
      *     The name of the field member to be accessed.
      */
    expression::pointer dot_expression(const expression::pointer &lhs,
        const variable_name &rhs);

    /**
      * The implicit_dot_expression method is used by the parser's WITH
      * statement to handle implcit record field access expressions.  By
      * default, it simply calls dot_expression.
      *
      * @param lhs
      *     The record expression to have its member accessed.
      * @param rhs
      *     The name of the field member to be accessed.
      */
    virtual expression::pointer implicit_dot_expression(
        const expression::pointer &lhs, const variable_name &rhs);

    /**
      * The array_index_expression method is used by the parser to
      * handle an array index expression.  After routine checks, the
      * virtual lhs->array_index_expression_factory method is called.
      *
      * @param lhs
      *     The array to be indexed
      * @param rhs
      *     The array index to be applied
      * @returns
      *     a new expression node
      */
    expression::pointer array_index_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The array_index_expression method is used by the parser to
      * handle an array index expression.
      * It is implemented by making repeated calls to the non-list method.
      *
      * @param lhs
      *     The array to be indexed
      * @param rhs
      *     The array indexes to be applied
      * @returns
      *     a new expression node
      */
    expression::pointer array_index_expression(const expression::pointer &lhs,
        const expression_list &rhs);

    /**
      * The subrange_type method is used to create a new subrange type
      * with the given bounds.
      *
      * @param minimum
      *     The minimum value of the subrange.
      * @param maximum
      *     The maximum value of the subrange.
      * @returns
      *     pointer to the new type
      */
    type::pointer subrange_type(const expression::pointer &minimum,
        const expression::pointer &maximum);

    /**
      * The set_type method is used to create a new set type with the
      * given subtype.
      *
      * @param subtype
      *     The subtype of the set.
      * @returns
      *     pointer to the new type
      */
    type::pointer set_type(const type::pointer &subtype);

protected:
    /**
      * The pre_order_traversal method is called (by the traversal
      * method) before any of the declarations are traversed.  The
      * default implementation does nothing.
      */
    virtual void pre_order_traversal(void) const;

    /**
      * The post_order_traversal method is called (by the traversal
      * method) after all of the declarations are traversed.  The
      * default implementation does nothing.
      */
    virtual void post_order_traversal(void) const;

public:
    /**
      * The is_current_function method is used to determine whether or
      * not the given symbol is the function for which code is currently
      * being generated (current top of scope stack).
      *
      * @param sp
      *     The symbol to test
      * @returns
      *     boolean; true if is current function, false if not
      */
    bool is_current_function(const symbol::pointer &sp) const;

    /**
      * The function_is_on_scope_stack method is used to determine
      * whether or not the given symbol is a function in the scope
      * stack.  This is used to determien whether or not a return value
      * assignment would be valid.
      *
      * @param sp
      *     The symbol to test
      * @returns
      *     boolean; true if part of the scope stack, false if not
      */
    bool function_is_on_scope_stack(const symbol::pointer &sp) const;

    /**
      * The label_factory method is used to create new label instances,
      * so that code jumps can be generated for conditional and loop
      * statements.
      */
    virtual label::pointer label_factory(void);

    /**
      * The get_lex_level function is used to obtain the degree of
      * nesting when functions are declared within other functions.
      *
      * If this is a normal program compilation, the PROGRAM is at lex
      * level zero, and nested functions increase this by one for every
      * level of nesting.
      *
      * If this is a system compilation (*$U-*), the PROGRAM starts at level -1.
      *
      * If this is a UNIT, we'll have to figure that out later.
      */
    int get_lex_level(void) const;

    /**
      * The is_a_sysprog method may be used to determine whether or not
      * a system program is being compiled.
      */
    bool is_a_sysprog(void) const { return syscomp; }

    /**
      * The comment method is used to to process comments.
      *
      * @param text
      *     The content of the comment.
      *     If (text[0] == '$') this is a control comment.
      */
    void comment(const rcstring &text);

    /**
      * The comment_arch method is called in response to a
      * (*$feature arch /name/ *)
      * command, or equivalent command line option.
      * This is used to set the byte sex based on the CPU architecture
      * (see #mtype_from_name for the mapping).
      *
      * @param arch_name
      *     The name of the CPU architecture.
      */
    void comment_arch(const rcstring &arch_name);

    /**
      * The comment_host method is called in response to a
      * (*$feature host /name/ *)
      * command, or equivalent command line option.
      * This is used to set the byte sex based on the host system
      * (see #mtype_from_host for the mapping).
      *
      * @param host_name
      *     The name of the host, a brand name (e.g. "apple").
      */
    void comment_host(const rcstring &host_name);

    /**
      * The comment_dispatcher instance variable is used to remember
      * the database of control comments.  This is used to interpret
      * control comments.  It is initially NULL, and is filled in by
      * #comment_dispatcher_init when the first control comment is seen.
      *
      * You shall always call #comment_dispatcher_init before using the
      * comment_dispatcher.
      */
    tcontrol::pointer comment_dispatcher;

    /**
      * The comment_dispatcher_init method is used to initialize the
      * database of control comments.  This is used to interpret control
      * comments.
      */
    void comment_dispatcher_init(void);

    /**
      * The interpret_comment_syscomp method is used to internpret the
      * (*$U-*) control comment.  We need a method because it isn't a
      * simple setting of a single boolean instance variable.
      */
    void interpret_comment_syscomp(bool yesno);

    bool get_short_case(void) const { return short_case_flag; }

protected:
    /**
      * The comment_notify method is used to do translator-specific
      * processing of a comment.  This does NOT include actually
      * interpreting the contents of "dollar comments", that is done
      * elsewhere.  The default implementation does nothing.
      */
    virtual void comment_notify(const rcstring &text);

    /**
      * The interpret_comment is called by the parser to process "dollar
      * comments".  This is called AFTER the comment method.  Note that
      * include directives jave already been processed, ad do not result
      * in a call to this method.
      */
    void interpret_comment(const rcstring &text);

protected:
    /**
      * The codefile_bigendian method is called by the interpret_comment
      * method when it sees a (*$B*) control comment, to switch from
      * making little-endian code files to big-endian code files.  The
      * default implementation does nothing.
      *
      * @param yesno
      *     true if the fiel is big-endian,
      *     false if the file is little-endian.
      */
    virtual void codefile_bigendian(bool yesno);

    /**
      * The codefile_copyright method is called by the interpret_comment
      * method when it sees a (*$C blah blah*) control comment, to
      * insert the text given into the generated code file.  The default
      * implementation does nothing.
      */
    virtual void codefile_copyright(const rcstring &text);

    /**
      * The codefile_library method is called by the interpret_comment
      * method when it sees a (*$U filename*) control comment, to say
      * where to get UNITs from.  The default implementation does
      * nothing.
      */
    virtual void codefile_library(const rcstring &filename);

public:
    /**
      * The set2set_expression_factory method is used to create a new
      * cast expression to convert one set type to another.
      *
      * @param to
      *     The type of the resulting expression.
      * @param from
      *     the expression to be cast
      * @returns
      *     pointer to new expression node.
      */
    virtual expression::pointer set2set_expression_factory(
        const type::pointer &to, const expression::pointer &from) = 0;

    /**
      * The lint2lint_expression_factory method is used to create a new
      * cast expression to convert one long integer type to another.
      *
      * @param to
      *     The type of the resulting expression.
      * @param from
      *     the expression to be cast
      * @returns
      *     pointer to new expression node.
      */
    virtual expression::pointer lint2lint_expression_factory(
        const type::pointer &to, const expression::pointer &from) = 0;

    /**
      * The lint_from_integer_expression_factory method is used to
      * create a new cast expression, to convert a 16-bit integer into a
      * long integer.
      *
      * @param operand
      *     The expression to be cast.
      */
    virtual expression::pointer lint_from_integer_expression_factory(
        const expression::pointer &operand) = 0;

    /**
      * The integer_from_lint_expression method is used to create a
      * new cast expression, to convert a long integer into a "normal"
      * integer.  It will issue an error, because the UCSD native
      * compiler required an explicit TRUNC call for this (even though
      * we can figure it out without help from the user).
      *
      * Calls the #integer_from_lint_expression_factory method oafter
      * routine checks and casts.
      *
      * @param operand
      *     The expression to be cast.
      */
    expression::pointer integer_from_lint_expression(
        const expression::pointer &operand);

protected:
    /**
      * The integer_from_lint_expression_factory method is used to
      * create a new cast expression, to convert a long integer into a
      * "normal" integer.
      *
      * @param operand
      *     The expression to be cast.
      */
    virtual expression::pointer integer_from_lint_expression_factory(
        const expression::pointer &operand) = 0;

    /**
      * The builtin_abs_factory method is used to create the translator
      * specific implmentation of the built-in "abs" function.
      */
    virtual symbol::pointer builtin_abs_factory(void) = 0;

    /**
      * The builtin_arctan_factory method is used to create the translator
      * specific implmentation of the built-in "arctan" function.
      */
    virtual symbol::pointer builtin_arctan_factory(void) = 0;

    /**
      * The builtin_atan_factory method is used to create the translator
      * specific implmentation of the built-in "atan" function.
      */
    virtual symbol::pointer builtin_atan_factory(void) = 0;

    /**
      * The builtin_blockread_factory method is used to create the translator
      * specific implmentation of the built-in "blockread" function.
      */
    virtual symbol::pointer builtin_blockread_factory(void) = 0;

    /**
      * The builtin_blockwrite_factory method is used to create the translator
      * specific implmentation of the built-in "blockwrite" function.
      */
    virtual symbol::pointer builtin_blockwrite_factory(void) = 0;

    /**
      * The builtin_chr_factory method is used to create the translator
      * specific implmentation of the built-in "chr" function.
      */
    virtual symbol::pointer builtin_chr_factory(void) = 0;

    /**
      * The builtin_close_factory method is used to create the translator
      * specific implmentation of the built-in "close" function.
      */
    virtual symbol::pointer builtin_close_factory(void) = 0;

    /**
      * The builtin_concat_factory method is used to create the translator
      * specific implmentation of the built-in "concat" function.
      */
    virtual symbol::pointer builtin_concat_factory(void) = 0;

    /**
      * The builtin_copy_factory method is used to create the translator
      * specific implmentation of the built-in "copy" function.
      */
    virtual symbol::pointer builtin_copy_factory(void) = 0;

    /**
      * The builtin_cos_factory method is used to create the translator
      * specific implmentation of the built-in "cos" function.
      */
    virtual symbol::pointer builtin_cos_factory(void) = 0;

    /**
      * The builtin_dec_factory method is used to create the translator
      * specific implmentation of the built-in "dec" function.
      */
    virtual symbol::pointer builtin_dec_factory(void) = 0;

    /**
      * The builtin_delete_factory method is used to create the translator
      * specific implmentation of the built-in "delete" function.
      */
    virtual symbol::pointer builtin_delete_factory(void) = 0;

    /**
      * The builtin_eof_factory method is used to create the translator
      * specific implmentation of the built-in "eof" function.
      */
    virtual symbol::pointer builtin_eof_factory(void) = 0;

    /**
      * The builtin_eoln_factory method is used to create the translator
      * specific implmentation of the built-in "eoln" function.
      */
    virtual symbol::pointer builtin_eoln_factory(void) = 0;

    /**
      * The builtin_exit_factory method is used to create the translator
      * specific implmentation of the built-in "exit" function.
      */
    virtual symbol::pointer builtin_exit_factory(void) = 0;

    /**
      * The builtin_exp_factory method is used to create the translator
      * specific implmentation of the built-in "exp" function.
      */
    virtual symbol::pointer builtin_exp_factory(void) = 0;

    /**
      * The builtin_fillchar_factory method is used to create the translator
      * specific implmentation of the built-in "fillchar" function.
      */
    virtual symbol::pointer builtin_fillchar_factory(void) = 0;

    /**
      * The builtin_get_factory method is used to create the translator
      * specific implmentation of the built-in "get" function.
      */
    virtual symbol::pointer builtin_get_factory(void) = 0;

    /**
      * The builtin_gotoxy_factory method is used to create the translator
      * specific implmentation of the built-in "gotoxy" function.
      */
    virtual symbol::pointer builtin_gotoxy_factory(void) = 0;

    /**
      * The builtin_halt_factory method is used to create the translator
      * specific implmentation of the built-in "halt" function.
      */
    virtual symbol::pointer builtin_halt_factory(void) = 0;

    /**
      * The builtin_idsearch_factory method is used to create the translator
      * specific implmentation of the built-in "idsearch" function.
      */
    virtual symbol::pointer builtin_idsearch_factory(void) = 0;

    /**
      * The builtin_inc_factory method is used to create the translator
      * specific implmentation of the built-in "inc" function.
      */
    virtual symbol::pointer builtin_inc_factory(void) = 0;

    /**
      * The builtin_insert_factory method is used to create the translator
      * specific implmentation of the built-in "insert" function.
      */
    virtual symbol::pointer builtin_insert_factory(void) = 0;

    /**
      * The builtin_ioresult_factory method is used to create the translator
      * specific implmentation of the built-in "ioresult" function.
      */
    virtual symbol::pointer builtin_ioresult_factory(void) = 0;

    /**
      * The builtin_length_factory method is used to create the translator
      * specific implmentation of the built-in "length" function.
      */
    virtual symbol::pointer builtin_length_factory(void) = 0;

    /**
      * The builtin_ln_factory method is used to create the translator
      * specific implmentation of the built-in "ln" function.
      */
    virtual symbol::pointer builtin_ln_factory(void) = 0;

    /**
      * The builtin_ln_factory method is used to create the translator
      * specific implmentation of the built-in "log" function.
      */
    virtual symbol::pointer builtin_log_factory(void) = 0;

    /**
      * The builtin_mark_factory method is used to create the translator
      * specific implmentation of the built-in "mark" function.
      */
    virtual symbol::pointer builtin_mark_factory(void) = 0;

    /**
      * The builtin_memavail_factory method is used to create the translator
      * specific implmentation of the built-in "memavail" function.
      */
    virtual symbol::pointer builtin_memavail_factory(void) = 0;

    /**
      * The builtin_moveleft_factory method is used to create the translator
      * specific implmentation of the built-in "moveleft" function.
      */
    virtual symbol::pointer builtin_moveleft_factory(void) = 0;

    /**
      * The builtin_moveright_factory method is used to create the translator
      * specific implmentation of the built-in "moveright" function.
      */
    virtual symbol::pointer builtin_moveright_factory(void) = 0;

    /**
      * The builtin_new_factory method is used to create the translator
      * specific implmentation of the built-in "new" function.
      */
    virtual symbol::pointer builtin_new_factory(void) = 0;

    /**
      * The builtin_odd_factory method is used to create the translator
      * specific implmentation of the built-in "odd" function.
      */
    virtual symbol::pointer builtin_odd_factory(void) = 0;

    /**
      * The builtin_opennew_factory method is used to create the translator
      * specific implmentation of the built-in "opennew" function.
      */
    virtual symbol::pointer builtin_opennew_factory(void) = 0;

    /**
      * The builtin_openold_factory method is used to create the translator
      * specific implmentation of the built-in "openold" function.
      */
    virtual symbol::pointer builtin_openold_factory(void) = 0;

    /**
      * The builtin_ord_factory method is used to create the translator
      * specific implmentation of the built-in "ord" function.
      */
    virtual symbol::pointer builtin_ord_factory(void) = 0;

    /**
      * The builtin_page_factory method is used to create the translator
      * specific implmentation of the built-in "page" function.
      */
    virtual symbol::pointer builtin_page_factory(void) = 0;

    /**
      * The builtin_pos_factory method is used to create the translator
      * specific implmentation of the built-in "pos" function.
      */
    virtual symbol::pointer builtin_pos_factory(void) = 0;

    /**
      * The builtin_pred_factory method is used to create the translator
      * specific implmentation of the built-in "pred" function.
      */
    virtual symbol::pointer builtin_pred_factory(void) = 0;

    /**
      * The builtin_put_factory method is used to create the translator
      * specific implmentation of the built-in "put" function.
      */
    virtual symbol::pointer builtin_put_factory(void) = 0;

    /**
      * The builtin_pwroften_factory method is used to create the translator
      * specific implmentation of the built-in "pwroften" function.
      */
    virtual symbol::pointer builtin_pwroften_factory(void) = 0;

    /**
      * The builtin_read_factory method is used to create the translator
      * specific implmentation of the built-in "read" function.
      */
    virtual symbol::pointer builtin_read_factory(void) = 0;

    /**
      * The builtin_read_factory method is used to create the translator
      * specific implmentation of the built-in "read" function.
      */
    virtual symbol::pointer builtin_readln_factory(void) = 0;

    /**
      * The builtin_release_factory method is used to create the translator
      * specific implmentation of the built-in "release" function.
      */
    virtual symbol::pointer builtin_release_factory(void) = 0;

    /**
      * The builtin_reset_factory method is used to create the translator
      * specific implmentation of the built-in "reset" function.
      */
    virtual symbol::pointer builtin_reset_factory(void) = 0;

    /**
      * The builtin_rewrite_factory method is used to create the translator
      * specific implmentation of the built-in "rewrite" function.
      */
    virtual symbol::pointer builtin_rewrite_factory(void) = 0;

    /**
      * The builtin_round_factory method is used to create the translator
      * specific implmentation of the built-in "round" function.
      */
    virtual symbol::pointer builtin_round_factory(void) = 0;

    /**
      * The builtin_seek_factory method is used to create the translator
      * specific implmentation of the built-in "seek" function.
      */
    virtual symbol::pointer builtin_seek_factory(void) = 0;

    /**
      * The builtin_sin_factory method is used to create the translator
      * specific implmentation of the built-in "sin" function.
      */
    virtual symbol::pointer builtin_sin_factory(void) = 0;

    /**
      * The builtin_sqr_factory method is used to create the translator
      * specific implmentation of the built-in "sqr" function.
      */
    virtual symbol::pointer builtin_sqr_factory(void) = 0;

    /**
      * The builtin_sqrt_factory method is used to create the translator
      * specific implmentation of the built-in "sqrt" function.
      */
    virtual symbol::pointer builtin_sqrt_factory(void) = 0;

    /**
      * The builtin_str_factory method is used to create the translator
      * specific implmentation of the built-in "str" function.
      */
    virtual symbol::pointer builtin_str_factory(void) = 0;

    /**
      * The builtin_succ_factory method is used to create the translator
      * specific implmentation of the built-in "succ" function.
      */
    virtual symbol::pointer builtin_succ_factory(void) = 0;

    /**
      * The builtin_time_factory method is used to create the translator
      * specific implmentation of the built-in "time" function.
      */
    virtual symbol::pointer builtin_time_factory(void) = 0;

    /**
      * The builtin_treesearch_factory method is used to create the
      * translator specific implmentation of the built-in "treesearch"
      * function.
      */
    virtual symbol::pointer builtin_treesearch_factory(void) = 0;

    /**
      * The builtin_trunc_factory method is used to create the translator
      * specific implmentation of the built-in "trunc" function.
      */
    virtual symbol::pointer builtin_trunc_factory(void) = 0;

    /**
      * The builtin_unitbusy_factory method is used to create the translator
      * specific implmentation of the built-in "unitbusy" function.
      */
    virtual symbol::pointer builtin_unitbusy_factory(void) = 0;

    /**
      * The builtin_unitclear_factory method is used to create the translator
      * specific implmentation of the built-in "unitclear" function.
      */
    virtual symbol::pointer builtin_unitclear_factory(void) = 0;

    /**
      * The builtin_unitread_factory method is used to create the translator
      * specific implmentation of the built-in "unitread" function.
      */
    virtual symbol::pointer builtin_unitread_factory(void) = 0;

    /**
      * The builtin_unitstatus_factory method is used to create the translator
      * specific implmentation of the built-in "unitstatus" function.
      */
    virtual symbol::pointer builtin_unitstatus_factory(void) = 0;

    /**
      * The builtin_unitwait_factory method is used to create the translator
      * specific implmentation of the built-in "unitwait" function.
      */
    virtual symbol::pointer builtin_unitwait_factory(void) = 0;

    /**
      * The builtin_unitwrite_factory method is used to create the translator
      * specific implmentation of the built-in "unitwrite" function.
      */
    virtual symbol::pointer builtin_unitwrite_factory(void) = 0;

    /**
      * The builtin_syscall_factory method is used to create a symbol
      * for a segment-zero function or procedure.
      *
      * @param name
      *     the name of the symbol
      * @param seg_num
      *     the number of the segment containg the function implementation
      * @param proc_num
      *     the procedure number within the segment
      * @returns
      *     pointer to dynamically allocated symbol
      */
    virtual symbol_function::pointer builtin_syscall_factory(
        const rcstring &name, int seg_num, int proc_num) = 0;

    /**
      * The nil_expression_factory method is used to manufacture an
      * expression suitable for use when defining the NIL named constant
      * as a built-in.  It shall be of type pointer to void (so as to
      * match all pointers).
      */
    virtual expression::pointer nil_expression_factory(void) = 0;

public:
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
      * The variant_record_begin method is used by the grammar to signal
      * the start of a variant record section.
      *
      * @param tp
      *     The type of the selector
      */
    void variant_record_begin(const type::pointer &tp);

    /**
      * The variant_record_field method is used by the grammar to signal
      * the end of a presence of a variant record alternative.
      *
      * @param selectors
      *     The constant expression to select which variant
      * @param child
      *     The child to be attached to these values.
      */
    void variant_record_field(const expression_list &selectors,
        const variant::pointer &child);

    /**
      * The variant_record_end method is used by the grammar to signal
      * the end of a variant record section.
      */
    variant::pointer variant_record_end(void);

    /**
      * The issue_iocheck method may be used to determine whether or not
      * to generate implicit IOCHECK calls into the code.
      */
    bool issue_iocheck(void) const { return iocheck; }

    /**
      * The issue_breakpoint method may be used to determine whether or
      * not to generate BPT opcodes into the code.
      */
    bool issue_breakpoint(void) const { return breakpoints_flag; }

    /**
      * The issue_range_check method is used to determine whether or not
      * to issue opcodes to perform checking on array subscripts and
      * assignments to variables of subrange types.
      */
    bool issue_range_check(void) const { return feature_range_check_flag; }

    /**
      * The issue_chr_range_check method is used to determine whether or not
      * to issue opcodes to perform checking on CHR function parameters.
      */
    bool issue_chr_range_check(void) const {
        return feature_chr_range_check_flag; }

    /**
      * The record_begin method is used to introduce a record scope
      * while the fields are declared.  Record declarations can be
      * nested.
      *
      * @param is_packed
      *     indicates whether or not this is a packed record.
      */
    type_record::pointer record_begin(bool is_packed);

    /**
      * The record_field method is used to declare a field of a record.
      *
      * @param name
      *     The names of the field or fields to be declared
      * @param tp
      *     The type of the fields
      */
    void record_field(variable_name_list &name, type::pointer &tp);

    /**
      * The record_end method is used to indicate that the record
      * declaration is complete.
      */
    void record_end(void);

    /**
      * The is_a_type_name method is used to determine whether or not
      * the given name is the name of a type.
      *
      * @param name
      *     The name to test for type-ness
      * @returns
      *     bool; true if the name is a type, false if it is undefined
      *     or defined but not a type.
      */
    bool is_a_type_name(const rcstring &name) const;

    /**
      * The get_type_by_name method is used to obtain a type of the
      * given name, if such a symbol exists, and if that symbol is a
      * named type.
      *
      * @param name
      *     The name to test for type-ness
      * @returns
      *     pointer to the type, or NULL on error.
      */
    type::pointer get_type_by_name(const rcstring &name) const;

    /**
      * The enumerated_begin method is used to allocate an new
      * enumerated type and prepare to have tags added to it.
      *
      * @returns
      *     a pointer to the new type
      */
    type::pointer enumerated_begin(void);

    /**
      * The enumerated_tag method is used to add another tag to an
      * enumerated type begin declared.
      *
      * @param name
      *     the name of the tag being declared.
      */
    void enumerated_tag(const variable_name &name);

protected:
    /**
      * The enumerated_symbol_factory method is used to create a new
      * translator-specific enumerated tag constant.
      *
      * @param name
      *     the name of the tag being declared.
      * @param parent
      *     The parent type containg each enumeration tag.
      * @param lex_level
      *     The lexical level of the declaration
      * @param value
      *     The integere value of the tag.
      */
    virtual symbol::pointer enumerated_symbol_factory(const variable_name &name,
        const type_enumerated::pointer &parent, int lex_level, int value) = 0;

public:
    /**
      * The enumerated_end method is used to note the end of an
      * enumerated type being declared.
      */
    void enumerated_end(void);

    /**
      * The complain_if_not_swapping method is called by the parse when
      * it sees a UNIT declaration.  If the swapping option has not been
      * enabled, the compiler will issue a warning.  This is because the
      * UCSD native compiler barfs at the same point.
      */
    void complain_if_not_swapping(void) const;

    /**
      * The complain_if_not_syscomp method is called by the parse when
      * it sees a sub-UNIT declaration.  If the (*$U-*) option has not been
      * enabled, the compiler will issue a warning.  This is because the
      * UCSD native compiler barfs at the same point.
      */
    void complain_if_not_syscomp(void) const;

    /**
      * The complain_about_empty_parens method is used to issue a an
      * error when C-type function calls with no parameters are seen
      * (declarations similarly).
      */
    virtual void complain_about_empty_parens(void);

    /**
      * The complain_about_address_of method is used to issue an error
      * when at-sign address-of expressions are seen.  These are common
      * in modern Pascal implementations, but didi not exist in UCSD
      * Pascal.
      */
    virtual void complain_about_address_of(void);

    /**
      * The fail_hook method is called if the compile fails because of
      * fatal errors.  It can be used to clean up after the translation,
      * if necessary.  The default implementation does nothing.
      */
    virtual void fail_hook(void);

    /**
      * The complain_about_named_label method is used to issue a warning
      * about named labels.  While this is a common feature in modern
      * Pascal implementations, they were not present in UCSD Pascal.
      *
      * @param name
      *     The name of the label, at the point of declaration.
      */
    void complain_about_named_label(const variable_name &name);

    /**
      * The complain_about_otherwise method is used to issue a warning
      * about the use of the otherwise clause in a case statement.
      */
    void complain_about_otherwise(void);

    /**
      * The uses method is ised to implement the USES declaration for
      * programs.  It then injects pseudo-input from the system.library
      * file, for the unit's interface section.
      *
      * @param names
      *     The names of the desired library segments.
      */
    void uses(const variable_name_list &names);

    /**
      * The uses method is ised to implement the USES declaration for
      * programs.  It then injects pseudo-input from the system.library
      * file, for the unit's interface section.
      *
      * @param name
      *     The name of the desired library segment.
      */
    void uses(const variable_name &name);

    /**
      * The uses_interface_push method is invoked by the grammar by
      * the magic text inserted by the USES clause, to start declaring
      * things in the unit of interest.
      *
      * @param name
      *     The name of the unit
      * @param code_seg
      *     The segment number of the unit's code.
      *     assert(code_seg > 0);
      * @param data_seg
      *     The segment number of the unit's data, or -1 if none.
      *     assert(data_seg > 0 || data_seg == -1);
      * @param segkind
      *     The kind of segment this is (UNITSEG, LINKED_INTRINS, etc).
      */
    void uses_interface_push(const rcstring &name, int code_seg, int data_seg,
        int segkind);

    /**
      * The uses_interface_pop method is invoked by the grammar by the
      * magic text inserted by the USES clause, to finish declaring
      * things in the unit of interest.
      */
    void uses_interface_pop(void);

    /**
      * The late_binding_opportunity method is called by the grammar
      * after every type declaration.  This is when forward type
      * declarations get resolved.
      */
    void late_binding_opportunity(void);

protected:
    /**
      * The interface_scope_factory method is used by
      * #uses_interface_push to manufacture a new scope to define
      * interface functions into.
      *
      * @param name
      *     The name of the unit.
      * @param code_seg
      *     The segment number of the unit's code.
      * @param data_seg
      *     The segment number of the unit's data, or -1 if none.
      * @param segkind
      *     The kind of unit segment this is (UNITSEG, LINKED_INTRINS, etc).
      */
    virtual scope::pointer interface_scope_factory(const rcstring &name,
        int code_seg, int data_seg, segkind_t segkind) = 0;

    /**
      * The record_scope_factory method is used to manufacture new
      * translator specific record scope instances.
      *
      * @param is_packed
      *     whether or not this record is a packed record.
      */
    virtual scope::pointer record_scope_factory(bool is_packed) = 0;

    /**
      * The uses_interface_notify_push method is called by the
      * #uses_interface_push method once it has completed all its other
      * housekeeping.
      *
      * @param sp
      *     The scope being pushed.
      */
    virtual void uses_interface_notify_push(const scope::pointer &sp);

    /**
      * The uses_interface_notify_pop method is called by the
      * #uses_interface_pop method once it has completed all its other
      * housekeeping.
      *
      * @param sp
      *     The scope being popped.
      */
    virtual void uses_interface_notify_pop(const scope::pointer &sp);

public:
    /**
      * The for_statement method is used to create a new for statement
      * instance in memory, after routine checks are performed.
      *
      * @param var
      *     The variable to modify for each loop iteration.
      * @param limit1
      *     The initial valude of the variable.
      * @param descending
      *     The direction of the loop.
      * @param limit2
      *     The final valude of the variable.
      * @param body
      *     The statement to perform once per iteration.
      */
    statement::pointer for_statement(const expression::pointer &var,
        const expression::pointer &limit1, bool descending,
        const expression::pointer &limit2, const statement::pointer &body);

protected:
    /**
      * The for_statement_factory method is used to create a new for
      * statement instance in memory.  This is caleld by #for_statement
      * after routine checks.
      *
      * @param var
      *     The variable to modify for each loop iteration.
      * @param limit1
      *     The initial valude of the variable.
      * @param descending
      *     The direction of the loop.
      * @param limit2
      *     The final valude of the variable.
      * @param body
      *     The statement to perform once per iteration.
      */
    virtual statement::pointer for_statement_factory(
        const expression::pointer &var, const expression::pointer &limit1,
        bool descending, const expression::pointer &limit2,
        const statement::pointer &body) = 0;

public:
    /**
      * The case_stack_push method is used by the grammar to indicate
      * the a case statement has commenced.  There is a stack because
      * case statements can be nested.
      *
      * @param ep
      *     The control expression.
      */
    expression::pointer case_stack_push(const expression::pointer &ep);

    /**
      * The case_stack_type method is used by the grammar when it is
      * checking case values.
      */
    type::pointer case_stack_type(void) const;

    /**
      * The case_stack_pop method is used by the grammar to indicate
      * that a case statement has completed.
      */
    void case_stack_pop(void);

    /**
      * The case_stack_check method is used by the grammar to verify
      * that a case value is of the correct type.
      *
      * @param ep
      *     The value of the label.  It's supposed to be the same type
      *     as the control expression.  It's supposed to be constant.
      */
    bool case_stack_check(const expression::pointer &ep) const;

    /**
      * The case_statement_factory method is used by the grammar to
      * turn the parts of a case statament into a new case statement
      * instance.
      *
      * @param control
      *     The control expression, used to select the case actually
      *     executed.
      * @param cases
      *     The set of values and statements of this case statement.
      * @param otherwise
      *     The statement to be executed, if none of the other cases
      *     match.  NULL if nothing is to be done.
      */
    virtual statement::pointer case_statement_factory(
        const expression::pointer &control, const case_list &cases,
        const statement::pointer &otherwise) = 0;

    /**
      * The label_declaration method is used to declare a label.
      *
      * @param names
      *     The name of the labels (even if they are numbers).
      */
    void label_declaration(const variable_name_list &names);

    /**
      * The label_statement statement is used to define a label.  This
      * acts as a destination for GOTO statements.  It is an error if
      * the same label is defined more than once.  It is an error if the
      * label has not previously appeared in a LABEL declaration in the
      * same scope.
      *
      * @param name
      *     The name of the label being defined.
      * @param body
      *     The statament following the label definition.
      */
    statement::pointer label_statement(const variable_name &name,
        const statement::pointer &body);

    /**
      * The goto_statement method is used to construct a goto statement
      * that immediately branches to the given label.  It is an error if
      * the label is never defined.  It is an error if the label has not
      * previously appeared in a LABEL declaration in the same scope.
      *
      * @param name
      *     The name of the label being referenced.
      */
    statement::pointer goto_statement(const variable_name &name);

private:
    /**
      * The goto_statement_factory method is called by the
      * goto_statement method to manufacture a new goto_statement
      * instance, once routine checks have been performed.
      */
    virtual statement::pointer goto_statement_factory(
        const symbol::pointer &sym) = 0;

    /**
      * The label_statement_factory method is used to construct a label
      * statement.  Error checking has already been performed.
      *
      * @param sym
      *     The symbol representing the label being referenced.
      * @param body
      *     The statement following the label.
      */
    virtual statement::pointer label_statement_factory(
        const symbol::pointer &sym, const statement::pointer &body) = 0;

public:
    /**
      * The goto_statement method is used to construct a got statament
      */
    statement::pointer goto_statement(const variable_name &name,
        const statement::pointer &body);

    /**
      * The in_expression method is used to validate an "e1
      * IN e2" expression (set inclusion test) and then call
      * #in_expression_factory to have the actual expression node
      * created.
      *
      * @param lhs
      *     The left hand side of the IN expression (the value to be tested)
      * @param rhs
      *     The right hand side of the IN expression (the set).
      * @returns
      *     pointer to a new expression instance in dynamic memory
      */
    expression::pointer in_expression(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    /**
      * The in_expression_factory method is used by #in_expression to
      * build a new expression node, after the parameters have been
      * confirmed as valid.
      *
      * @param lhs
      *     The left hand side of the IN expression (the value to be tested)
      * @param rhs
      *     The right hand side of the IN expression (the set).
      * @returns
      *     pointer to a new expression instance in dynamic memory
      */
    virtual expression::pointer in_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The pointer_eq_pointer_expression method is used to check for
      * validity before calling pointer_eq_pointer_expression_factory to
      * actually create the expression.
      *
      * @param lhs
      *     The left hand side of the equality expression
      * @param rhs
      *     The right hand side of the equality expression
      * @returns
      *     pointer to a new expression instance in dynamic memory
      */
    expression::pointer pointer_eq_pointer_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

protected:
    /**
      * The pointer_eq_pointer_expression_factory method is used
      * create a new pointer equality test expression node.
      *
      * @param lhs
      *     The left hand side of the equality expression
      * @param rhs
      *     The right hand side of the equality expression
      * @returns
      *     pointer to a new expression instance in dynamic memory
      */
    virtual expression::pointer pointer_eq_pointer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The pointer_ne_pointer_expression method is used to check for
      * validity before calling pointer_ne_pointer_expression_factory to
      * actually create the expression.
      *
      * @param lhs
      *     The left hand side of the inequality expression
      * @param rhs
      *     The right hand side of the inequality expression
      * @returns
      *     pointer to a new expression instance in dynamic memory
      */
    expression::pointer pointer_ne_pointer_expression(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The compound_scope_begin method is used by the parser to tell
      * the translator that a compound statement has begun.  This
      * creates a transparent variable scope, for temporary variables
      * needed by the parser.
      */
    void compound_scope_begin(void);

    /**
      * The compound_scope_end method is used by the parser to tell
      * the translator that a compound statement has ended.
      */
    void compound_scope_end(void);

    /**
      * The var_statement method is used by the grammar to have the
      * translator build a VAR statement.  (This isn't standard Pascal.)
      * Calls the #var_statement_factory after routine checks.
      *
      * @param names
      *     The names of the variables to be declared.
      * @param tp
      *     The type of the variables.
      */
    statement::pointer var_statement(const variable_name_list &names,
        const type::pointer &tp);

    /**
      * The var_statement method is used by the grammar to have the
      * translator build a VAR statement.  (This isn't standard Pascal.)
      * Calls the #var_statement_factory after routine checks.
      *
      * @param name
      *     The names of the variable to be declared.
      * @param value
      *     The expression to be used to initialize the variable, and it
      *     implies the variable type as well.
      */
    statement::pointer var_statement(const variable_name &name,
        const expression::pointer &value);

    /**
      * The char_from_string_of_length_one method is used to convert a
      * string constant of length one into a char constant.
      *
      * @param ep
      *     The expression node to try to convert.
      * @returns
      *     a new expression treem or NULL if the conversion can't be
      *     performed (not a string, not constant, not length one, etc).
      */
    expression::pointer char_from_string_of_length_one(
        const expression::pointer &ep);

    /**
      * The allocate_temporary_variable method is used to allocate
      * anonymous temporary variables for calculations.
      *
      * @param tp
      *     The type of the temporary variable
      */
    symbol::pointer allocate_temporary_variable(const type::pointer &tp);

    /**
      * The allocate_temporary_variable method is used to allocate
      * temporary variables for calculations.
      *
      * @param name
      *     The name of the temporary variable
      * @param tp
      *     The type of the temporary variable
      */
    symbol::pointer allocate_temporary_variable(const rcstring &name,
        const type::pointer &tp);

protected:
    /**
      * The var_statement_factory method is used by the grammar to have the
      * translator build a VAR statement.
      *
      * @param names
      *     The names of the variables to be declared.
      * @param tp
      *     The type of the variables.
      */
    virtual statement::pointer var_statement_factory(
        const variable_name_list &names, const type::pointer &tp) = 0;

    /**
      * The var_statement method is used by the grammar to have the
      * translator build a VAR statement.
      *
      * @param name
      *     The names of the variable to be declared.
      * @param value
      *     The expression to be used to initialize the variable, and it
      *     implies the variable type as well.
      */
    virtual statement::pointer var_statement_factory(const variable_name &name,
        const expression::pointer &value) = 0;

    /**
      * The pointer_ne_pointer_expression_factory method is used
      * create a new pointer inequality test expression node.
      *
      * @param lhs
      *     The left hand side of the inequality expression
      * @param rhs
      *     The right hand side of the inequality expression
      * @returns
      *     pointer to a new expression instance in dynamic memory
      */
    virtual expression::pointer pointer_ne_pointer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs) = 0;

public:
    /**
      * The scan_expression_factory method is used by the grammar to obtain
      * an instance of a SCAN function call.  The scan fucntion can't
      * be folded into the ordinary function call syntax because of
      * the revolting condition buried in the call before the second
      * parameter.  Bletch.
      *
      * @param maxdisp
      *     How far to scan; backwards if negative, forwards if positive.
      * @param mask
      *     Scan for equality if 0, inequality if 1.
      * @param value
      *     The character value to scan for.
      * @param start
      *     Expression evaluating to a byte pointer (reference), scan
      *     starting position.
      */
    virtual expression::pointer scan_expression_factory(
        const expression::pointer &maxdisp, int mask,
        const expression::pointer &value, const expression::pointer &start) = 0;

public:
    /**
      * The warnings_are_silent method is used by the lexer to
      * determine whether or not warnings are silent (suppressed)8.
      *
      * @returns
      *     true if warnings are not to be printed, false if warnings
      *     are to be printed.
      */
    bool warnings_are_silent(void) const { return warnings_are_silent_flag; }

    /**
      * The warnings_are_errors method is used by the lexer to determine
      * whether or not warnings are to be treated as errors (unless
      * suppressed).
      *
      * @returns
      *     true if warnings are to be treated as fatal errors, false if
      *     warnings are to be treated as normal.
      */
    bool
    warnings_are_errors(void)
        const
    {
        return warnings_are_errors_flag && !warnings_are_silent_flag;
    }

    /**
      * The complain_about_hex_constant method is used to issue a
      * warning about hexadecimal constants appearing in the source
      * code.
      */
    void complain_about_hex_constant(void);

    /**
      * The complain_about_binary_constant method is used to issue a
      * warning about binary constants appearing in the source code.
      */
    void complain_about_binary_constant(void);

private:
    /**
      * The debug instance variable is used to remember whether or
      * not to turn on the yacc/bison debug output during the parse.
      * Defaults to false.
      */
    bool debug;

    /**
      * The scopes instance variable is used to remember the current
      * stack of symbol look-up scopes.
      */
    scope_stack scopes;

    /**
      * The builtin instance variable is used to remember the scope
      * which contains the builtin symbols.
      */
    scope::pointer builtins;

    /**
      * The syscomp instance variable is used to remember if we are
      * compiling a system program (true) or a user program (false).
      * The name comes from the variable with the same job in the UCSD
      * Pascal compiler sources, file procs.a.text, line 266.
      *
      *   (*$U-*) sets syscomp = true,
      *   (*$U+*) sets syscomp = false (this is the default).
      */
    bool syscomp;

    /**
      * The no_more_codefile_options instance variable is used to
      * remember if code has been generated to the coefile yet.  Once
      * it has, it is no longer possible to use many of the control
      * comments.
      */
    bool no_more_codefile_options;

public:
    /**
      * The pascal_id funtion is used to translate a normal variable name
      * into the pascal variable name used in the symbol table.
      *
      * (a) variable name are case insensitive, so we always drop them
      *     to lower case,
      * (b) underscores are not significant
      *     ...but see #feature_underscore_significant
      * (c) only the first 8 characters are significant.
      *     ...but see #max_id_len
      *
      * @note
      *     The UCSD folks made up these hinky rules, not me!
      */
    static rcstring pascal_id(const rcstring &name);

    /**
      * The control_comment_too_late method is used by the
      * tcontrol_too_late class to be able to issue errors when steeing
      * appear too late in a sourec file.
      */
    bool
    control_comment_too_late(void)
        const
    {
        return no_more_codefile_options;
    }

    /**
      * The warning_set_comparison method is used to ask whether or not
      * to to issue a warning if set<set and set>set comparisons appear
      * in the source code.
      */
    bool warning_set_comparisons(void);

    /**
      * The feature_extra_set_comparison method is used to ask whether
      * or not to use LES POWR and GRT POWR opcodes.
      */
    bool feature_extra_set_comparisons(void) const;

    /**
      * The feature_string_escapes method is used to ask whether or not
      * to use C-style escape characters in string constants.
      */
    bool feature_string_escapes(void) const;

    /**
      * The warning_constant_branch method is used to ask whether or not
      * to issue a warning when a branch expression (of an IF, WHILE or
      * REPEAT UNTIL statement) is a constant.
      */
    bool warning_constant_branch(void) const;

    /**
      * The scope_top_of_stack_list_symbols method is used to have a
      * list of symbols inserted into the listing.
      */
    void scope_top_of_stack_list_symbols(code_source_map &) const;

    /**
      * The feature_efj_nfj method is used to determine whether or not
      * to use the EFJ and NFJ opcodes.
      */
    bool feature_efj_nfj(void) const { return feature_efj_nfj_flag; }

private:
    /**
      * The iocheck instance variable is used to remember whether or not
      * to generate code to check the IORESULT function after each I/O
      * operation.  Defaults to true.  Controlled by the (*$I-*) comment.
      */
    bool iocheck;

    /**
      * The breakpoints_flag instance variable is used to remember
      * whether or not to issue breakpoint instructions into the
      * codefile during the course of the compilation in order that the
      * interactive Debugger can be used more effectively.  Controlled
      * by the (*$D+*) comment.
      */
    bool breakpoints_flag;

    /**
      * The goto_warning instance variable is used to remember whether
      * or not to issue compiler warnings when GOTO and labels appear in
      * a source files.
      */
    bool goto_warning;

    /**
      * The feature_range_check_flag instance variable is used to
      * remember whether or not to issue opcodes to perform checking on
      * array subscripts and assignments to variables of subrange types.
      */
    bool feature_range_check_flag;

    /**
      * The feature_chr_range_check_flag instance variable is used to remember
      * whether or not to issue opcodes to perform checking on function CHR
      * parameters (subject also the the #feature_range_check_flag setting).
      */
    bool feature_chr_range_check_flag;

    /**
      * The current_enum instance variable is used to remember the
      * current enumeration type, when and enumerated type is being
      * declared.
      */
    type_enumerated::pointer current_enum;

    /**
      * The empty_parens_warning instance variable is used to remember
      * whether or not empty parentheses in function calls and
      * declarations are acceptable.  Defaults to true.
      */
    bool empty_parens_warning;

    /**
      * The op_addition instance variable is used to remember all of the
      * possible legal addition operand combinations.
      */
    expression_dispatcher_binary op_addition;

    /**
      * The op_subtraction instance variable is used to remember all of
      * the possible legal subtraction operand combinations.
      */
    expression_dispatcher_binary op_subtraction;

    /**
      * The op_multiplication instance variable is used to remember all
      * of the possible legal multiplication operand combinations.
      */
    expression_dispatcher_binary op_multiplication;

    /**
      * The op_div instance variable is used to remember all of the
      * possible legal integer division operand combinations.
      */
    expression_dispatcher_binary op_div;

    /**
      * The op_division instance variable is used to remember all of the
      * possible legal real division operand combinations.
      */
    expression_dispatcher_binary op_division;

    /**
      * The op_eq instance variable is used to remember all of the
      * possible legal equal to operand combinations.
      */
    expression_dispatcher_binary op_eq;

    /**
      * The op_ne instance variable is used to remember all of the
      * possible legal not equal to operand combinations.
      */
    expression_dispatcher_binary op_ne;

    /**
      * The op_eq instance variable is used to remember all of the
      * possible legal less than to operand combinations.
      */
    expression_dispatcher_binary op_lt;

    /**
      * The op_ne instance variable is used to remember all of the
      * possible legal less than or equal to operand combinations.
      */
    expression_dispatcher_binary op_le;

    /**
      * The op_eq instance variable is used to remember all of the
      * possible legal greater than to operand combinations.
      */
    expression_dispatcher_binary op_gt;

    /**
      * The op_ne instance variable is used to remember all of the
      * possible legal greater than or equal to operand combinations.
      */
    expression_dispatcher_binary op_ge;

    /**
      * The op_negate instance variable is used to remember all of the
      * possible legal negate operand combinations.
      */
    expression_dispatcher_unary op_negate;

    /**
      * The op_and instance variable is used to remember all of the
      * possible legal AND operand combinations.
      */
    expression_dispatcher_binary op_and;

    /**
      * The op_or instance variable is used to remember all of the
      * possible legal OR operand combinations.
      */
    expression_dispatcher_binary op_or;

    /**
      * The op_not instance variable is used to remember all of the
      * possible legal NOT operand combinations.
      */
    expression_dispatcher_unary op_not;

    /**
      * The address_of_flag instance variable is used to remember
      * whether or not the address-of operator (the at sign) is
      * permitted.  Defaults to true.
      */
    bool address_of_warning;

    typedef std::vector<variant::pointer> variants_t;

    /**
      * The varinats instance variable is used to rememebr the current
      * stack of variant decision branches when parsing variant record
      * types.
      */
    variants_t variants;

    /**
      * This appears to disable some of the builtins, in order to make more room
      * for user code in the compiler.
      *
      * READLN, PRED, SUCC, SQR, UNITREAD, INSERT, DELETE, COPY, POS,
      * SEEK, GET, PUT, PAGE, STR, GOTOXY
      */
    bool tiny;

    typedef std::vector<expression::pointer> case_stack_t;

    case_stack_t case_stack;

    /**
      * The short_case_flag is used to remember the setting of the
      * {$short-case} control.  Defaults to false.
      */
    bool short_case_flag;

    /**
      * The warnings_are_silent_flag instance variable is used to
      * remember whether or not warnings are silent (true) or printed
      * (false).  Defaults to false.
      */
    bool warnings_are_silent_flag;

    /**
      * The warnings_are_errors_flag instance variable is used to
      * remember whether or not warnings are errors (true) or treated
      * normally (false).  Default to false.
      */
    bool warnings_are_errors_flag;

    /**
      * The warning_named_label_flag instance variable is used to
      * remember whether or not to issue warnings about named labels.
      */
    bool warning_named_label_flag;

    /**
      * The warning_shadow_flag instance variable is used to remember
      * whether or not warnings are to be issued when symbols are
      * shadowed by later symbols.
      */
    bool warning_shadow_flag;

    /**
      * The warning_hex_constants_flag instance variable is used
      * to remember whether or not warnings are to be issued when
      * hexadecimal constants ($XX) appear in the source code.
      */
    bool warning_hex_constants_flag;

    /**
      * The warning_binary_constants_flag instance variable is used
      * to remember whether or not warnings are to be issued when
      * binary constants (%01010) appear in the source code.
      */
    bool warning_binary_constants_flag;

    /**
      * The define_builtin_symbols_non_syscomp method is called by the
      * #define_builtin_symbols method, but only if (*$U+*).
      */
    void define_builtin_symbols_non_syscomp(void);

public:
    /**
      * The ternary_expression method is called by the grammar when
      * it sees a C ternary expression (e1 ? e2 : e3).  After routine
      * checks, the #ternary_expression_factory method is called to
      * actually create the new expression instance.
      * It may issue a warning, as well.
      */
    expression::pointer ternary_expression(const expression::pointer &cond,
        const expression::pointer &then_branch,
        const expression::pointer &else_branch);

protected:
    /**
      * The ternary_expression_factory method is called by the
      * #ternary_expression method.
      */
    virtual expression::pointer ternary_expression_factory(
        const expression::pointer &cond, const expression::pointer &then_branch,
        const expression::pointer &else_branch) = 0;

    /**
      * The warning_unreachable method is used to determine whether
      * or not to issue a warning for unreachable statements.  It can
      * be disabled using the (*$warning unreachable false*) control
      * comment.
      */
    bool warning_unreachable(void) const { return warning_unreachable_flag; }

    /**
      * The top_scope method is used to obtain the scope on the top of
      * the scope stack.  Use sparingly.
      */
    scope::pointer top_scope(void) const;

public:
    /**
      * The optional_paoc_from_string method is used to quietly promot
      * string constants to packed array of char when needed.  If the
      * expression is not a string constant, it is returned unchanged.
      * When the type is not packed array of char, the expression is
      * return unchanged.
      *
      * @param tap
      *     The array type of interest.
      * @param ep
      *     The expression to be promoted.
      */
    expression::pointer optional_paoc_from_string(
        const type_array::pointer &tap, const expression::pointer &ep);

    /**
      * The feature_long_integer_constants method may be used to determine
      * whether or not to evaluate constant long integer expressions, and
      * insert long integer constants into the code.
      */
    bool feature_long_integer_constants(void) const;

    /**
      * The feature_long_integer_extensions method may be used to
      * determine whether or not to allow the use of INTEGER[*]
      * operations like ABS, MOD, ODD and SQR.
      */
    bool feature_long_integer_extensions(void) const;

    /**
      * The unit_is_separate method is called by the grammar when a
      * SEPARATE UNIT declaration is seen.
      */
    void unit_is_separate(void);

private:
    /**
      * The complain_about_goto method is used to issue a warning about
      * the use of the goto statement.
      *
      * @param where
      *     location information for the error message
      */
    void complain_about_goto(const location &where);

    /**
      * The max_id_len class variable is used to remember the length to
      * which identifier names are cropped.  Must always be eight or
      * more.  Must not be changed once things get going.
      */
    static int max_id_len;

    /**
      * The feature_underscore_significant class variable is used to
      * remember whether or not the underscore character is significant
      * in identifiers.  Defaults to false, for compatibility with the
      * UCSD native compiler.  Set to true for ISO 10206 compatibility.
      */
    static bool feature_underscore_significant;

    /**
      * The warning_set_comparisons_flag instance variable is used to
      * remember whether or not to to issue a warning if set&lt;set and
      * set&gt;set comparisons appear in the source code.  Defaults to
      * true, because the UCSD compiler did not allow them.
      */
    bool warning_set_comparisons_flag;

    /**
      * The feature_extra_set_comparisons_flag instance variable is used
      * to remember whether or not to use LES POWR and GRT POWR opcodes
      * (work-arounds will be used, if necessary).  Defaults to false
      * because the original p-machine did not implement them.
      */
    bool feature_extra_set_comparisons_flag;

    /**
      * The warning_constant_branch_flag instance variable is used
      * to remember whether or not to issue a warning when a branch
      * expression (of an IF, WHILE or REPEAT UNTIL statement) is a
      * constant.  Defaults to true.
      */
    bool warning_constant_branch_flag;

    /**
      * The warning_parameter_string_size_flag is used to enable or
      * disable the printing of warnings when string parameters are
      * declared with a size.  The UCSD native compiler did not permit
      * this.  Defaults to true.
      */
    bool warning_parameter_string_size_flag;

    /**
      * The warning_otherwise_flag instance variable is used to rememebr
      * whether or not to issue a warning when an OTHERWISE clause is
      * seen attached to a CASE statement.  This is common in mordern
      * Pascal implementations, but was not present in UCSD Pascal.
      * Defaults to true.
      */
    bool warning_otherwise_flag;

    /**
      * The warning_ternary_expression_flag instance variable is used
      * to remember whether or not to issues a warning when a C-style
      * ternary expression (e1?e2:e3) is encountered.  Defaults to true.
      */
    bool warning_ternary_expression_flag;

    /**
      * The warning_unreachable_flag instance variable is used to
      * remember whether or not to issue a warning for unreachable
      * statements.  It can be disabled using the (*$warning unreachable
      * false*) control comment.
      */
    bool warning_unreachable_flag;

    /**
      * The compiler always checks for procedures and functions that were
      * declared forward, but were not later defined, and issues a fatal
      * error for each such symbol.  This option tells the compiler to ignore
      * procedures from segment zero that were declared forward, but were not
      * defined.  This is only of use to system utilities.
      *
      * Defaults to false.
      *
      * The system sources are all very hackish.  Many of them actually have
      * a segment zero, which is ignored, just so they can get at system
      * internals at lex level -1.  Surely it would have been cleaner to have
      * a USES variant that gave access to these symbols.  That way, only
      * the operating system itself would need (*$U-*) and all other system
      * utilities, including the compiler, could be a simple (*$U+*) use
      * program.
      */
    bool feature_ignore_undefined_segment_zero_flag;

    /**
      * The feature_inline_math_flag instance variable is used
      * to remember whethr or not to define built-in math and
      * trig functions that correspond to CSP p-code standard
      * procedure calls.  These were not in the Apple Pascal p-Machine
      * (presumably) due to size constratints, but the p-machine spec
      * still defines them.
      *
      * This flag defaults to false, meaning you have to use the
      * TRANSCENDENTAL unit if you want math functions.
      */
    bool feature_inline_math_flag;

    /**
      * The feature_string_escapes_flag instance variable is used to
      * remember whether or not C-like escapes in string constants.
      */
    bool feature_string_escapes_flag;

    /**
      * The feature_short_with_flag instance variable is used to
      * remember whether or not to use the base expression of WITH
      * unchanged, if that would generate the same or less code than
      * using a temporary pointer variable.  Defaults to true.
      */
    bool feature_short_with_flag;

    /**
      * The feature_efj_nfj_flag instance variable is used to remember
      * whether or not to use the EFJ and NFJ opcodes.  These opcodes
      * make integer equality and inequanity branches more efficient.
      * Defaults to true.
      *
      * These opcodes are present and functional in the Apple Pascal
      * p-machine, but were not used by the Apple Pascal compiler.
      * They are also present and functional in the Klebsch p-machine
      * implementation (the ucsd-psystem-vm project).
      */
    bool feature_efj_nfj_flag;

    /**
      * The feature_long_integer_constants_flag instance variable is used to
      * remember whether or not to evaluate constant long integer expressions,
      * and insert long integer constants into the code.
      */
    bool feature_long_integer_constants_flag;

    /**
      * The feature_long_integer_extensions_flag instance variable is
      * used to remember whether or not to allow the use of INTEGER[*]
      * operations like MOD, ABS, ADD and SQR.
      */
    bool feature_long_integer_extensions_flag;

    /**
      * The feature_swapping_flag is used to remember whether or not the
      * user as used the (*$S+*) control comment.  While this compiler
      * doesn't do anything special when it is set, it is used to issue
      * a warning in places where the UCSD native compiler will barf if
      * (*$S+*) is missing.
      */
    bool feature_swapping_flag;

    /**
      * The unit_segnum_code instance variable is used to remember the
      * segment number for the code of the soon-to-be-created UNIT segment.
      * Set by the #unit_intrinsic_code method.
      */
    int unit_segnum_code;

    /**
      * The unit_segnum_data instance variable is used to remember the
      * segment number for the data of the soon-to-be-created UNIT segment.
      * Set by the #unit_intrinsic_data method.
      */
    int unit_segnum_data;

    typedef std::vector<rcstring> unit_libraries_t;

    /**
      * The unit_libraries instance variable is used to remember the
      * list of paths of library codefiles to be searched for USES
      * modules.
      */
    unit_libraries_t unit_libraries;

    /**
      * The unit_library_init method is used to initialize the search
      * path for UNIT codefiles.  It is safe to call this more than once.
      */
    void unit_library_init(void);

    /**
      * The unit_library_add method is used to looks down the library
      * search path for the named file, and add it to the libraries to
      * be searched for library UNIT code segments.
      *
      * @param filename
      *     The name of the file to be searched for, via the
      *     #unit_library_adder method.
      * @param insist
      *     whether or not the library file is required to exist.
      */
    void unit_library_add(const rcstring &filename, bool insist);

    /**
      * The unit_library_adder method is used to add the given file to
      * the search list for library UNITs.  If the file itself is not
      * found, it is also tried with ".code" and ".library" file name
      * extensions.  Called by the #unit_library_add method.
      *
      * @param filename
      *     The name of the file to be used.
      * @returns
      *     bool; true if the file was (eventually) found, false if the
      *     file could not be found, even after trying various file name
      *     extensions.
      */
    bool unit_library_adder(const rcstring &filename);

    /**
      * The uses_unitseg method is used to map a non-intrinsic unit
      * number to an available segment number.
      *
      * In a compiler (or something that must behave like a compiler)
      * we have to use a different segment number if the given segment
      * number has been used.
      *
      * As a default, return the same segment number.
      *
      * @param segname
      *     The name of the unit's segment.
      * @param segnum
      *     The number of the unit's segment.
      */
    virtual int uses_unitseg(const rcstring &segname, int segnum);

    /**
      * The unit_segkind instance variable is used to remember which
      * kind of segment to create in response to a UNIT declaration.
      *
      * UNITSEG
      *     This is a plain vanilla unit
      * SEPRTSEG
      *     This is a separate unit (a II.0 feature, not a II.1 feature)
      *     like those produced by the assembler.
      * LINKED_INTRINS
      *     This is an intrinsic unit.
      */
    segkind_t unit_segkind;

    /**
      * The library_search_path instance variable is used to remember
      * the ordered list of directories to search for library code files.
      */
    static rcstring_list library_search_path;

    /**
      * The library_search_path_env instance variable is used to
      * remember whether or not the #library_search_path instance
      * variable has been initialised (with eht directories from the
      * environment variable, and the system paths).
      */
    static bool library_search_path_env;

    /**
      * The copy constructor.  Do not use.
      */
    translator(const translator &);

    /**
      * The assignment operator.  Do not use.
      */
    translator &operator=(const translator &);
};

#endif // LIB_TRANSLATOR_H
// vim: set ts=8 sw=4 et :
