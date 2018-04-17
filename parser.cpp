#include <assert.h>
#include "parser.h"
#include <stdlib.h>

using namespace Lexer;

namespace Parser {
  void SyntaxAnalyzer::InitBasicRule() {

    for (int i = 0; i < MAX_RULES; i++) {
      memset(rules_[i].sub_rules_, -1, sizeof(rules_[i].sub_rules_));
    }

    for (int i = TokEof; i < TokEnd; i++) {
      rules_[i] = {Terminal, {i}};
    }
    // compilation_unit 
    //    : import_stmts top_defs <EOF> 
    rules_[compilation_unit] = {Sequence, {import_stmts, top_defs, TokEof,}};

    //  typeref_base  
    //    : <VOID> 
    //      | <CHAR> 
    //      | <SHORT> 
    //      | <INT> 
    //      | <LONG> 
    //      | <UNSIGNED> <CHAR> 
    //      | <UNSIGNED> <SHORT> 
    //      | <UNSIGNED> <INT> 
    //      | <UNSIGNED> <LONG> 
    //      | <FLOAT> 
    //      | <DOUBLE>
    //      | <CLASS> <IDENTIFIER> 
    rules_[typeref_base] = {Select, { seq_void, 
                                      seq_char,
                                      seq_short, 
                                      seq_int, 
                                      seq_long, 
                                      seq_unsigned_char,
                                      seq_unsigned_short,
                                      seq_unsigned_int,
                                      seq_unsigned_long,
                                      seq_float,
                                      seq_double,
                                      seq_class_identifier,
                                      }};
      rules_[seq_void] = {Sequence, {TokVoid}};
      rules_[seq_char] = {Sequence, {TokChar}};
      rules_[seq_short] = {Sequence, {TokShort}};
      rules_[seq_int] = {Sequence, {TokInt}};
      rules_[seq_long] = {Sequence, {TokLong}};
      rules_[seq_unsigned_char] = {Sequence, {TokUnsigned, TokChar}};
      rules_[seq_unsigned_short] = {Sequence, {TokUnsigned, TokShort}};
      rules_[seq_unsigned_int] = {Sequence, {TokUnsigned, TokInt}};
      rules_[seq_unsigned_long] = {Sequence, {TokUnsigned, TokLong}};
      rules_[seq_float] = {Sequence, {TokFloat}};
      rules_[seq_double] = {Sequence, {TokDouble}};
      rules_[seq_class_identifier] = {Sequence, {TokClass, TokIdentifier}};
    
    // import_stmts  
    //   : (import_stmt)* 
    rules_[import_stmts] = {Repeat, {import_stmt}};

    // import_stmt 
    //   : <IMPORT> name ("." name)* ";" 
    rules_[import_stmt] = {Sequence, {TokImport, name, rep_dot_name, TokSemiColon}};
      // ("." name)*
      rules_[rep_dot_name] = {Repeat, {TokDot, name}};

    // name 
    //   : <IDENTIFIER> 
    rules_[name] = {Sequence, {TokIdentifier}};

    // top_defs // top definitions 
    //   : ( deffunc 
    //     | defvars 
    //     | defconst 
    //     | defclass 
    //     | typedef )* 
    rules_[top_defs] = {Repeat, {sel_fun_var_const_class_typedef}};
      rules_[sel_fun_var_const_class_typedef] = {Select, {deffunc, 
                                                          defvars, 
                                                          defconst, 
                                                          defclass}};
    // defvars // variable definition. ex) int a = 0, b=19; 
    //   : storage type name ["=" expr] [("," name ["=" expr])*] ";" 
    rules_[defvars] = {Sequence, {storage, type, name, opt_eq_expr, opt_rep_cm_name_dot_eq_expr, TokSemiColon }};
      rules_[opt_eq_expr] = {Options, {TokAssign, expr}}; // ["=" expr]
      // [("," name ["=" expr])*]
      rules_[opt_rep_cm_name_dot_eq_expr] = {Options, {rep_cm_name_dot_eq_expr}}; 
        // ("," name ["=" expr])*
        rules_[rep_cm_name_dot_eq_expr] = {Repeat, {TokComma, name, opt_eq_expr}};
        
    // defconst
    //   : <CONST> type name "=" expr ";"
    rules_[defconst] = {Sequence, {TokConst, type, name, TokAssign, expr, TokSemiColon}};

    // storage 
    //   : [<STATIC>] 
    rules_[storage] = {Options, {TokStatic}};
    
    // type 
    //   : typeref 
    rules_[type] = {Sequence, {typeref}};

    // deffunc // function definition 
    //   : storage typeref name "(" params ")" block 
    rules_[deffunc] = {Sequence, {storage, typeref, name, TokParenClose, params, TokParenClose, block }};

    // params // parameter definition 
    //   : <VOID> 
    //   | fixedparams ["," "..."] 
    rules_[params] = {Select, {TokVoid, seq_fixparms_dot_dotdotdot}};
      // fixedparams ["," "..."] 
      rules_[seq_fixparms_dot_dotdotdot] = {Sequence, {fixedparams, opt_comma_dotdotdot}};
        // ["," "..."] 
        rules_[opt_comma_dotdotdot] = {Options, {TokComma, TokDotDotDot}};

    // fixedparams // fixed parameter definition 
    //   : param ("," param)* 
    rules_[fixedparams] = {Sequence, {param, rep_comma_param}};
      // ("," param)* 
      rules_[rep_comma_param] = {Repeat, {TokComma, param}};
      
    // param 
    //   : type name 
    rules_[param] = {Sequence, {type, name}};

    // defvar_list 
    //   : (defvars)*
    rules_[defvar_list] = {Repeat, {defvars}};

    // block 
    //   : "{" defvar_list stmts "}" 
    rules_[block] = {Sequence, {TokBraceOpen, defvar_list, stmts, TokBraceClose}};

    // defclass // class definition 
    //   : <CLASS> name member_list ";" 
    rules_[defclass] = {Sequence, {TokClass, name, member_list, TokSemiColon}};

    // member_list //  
    //   : "{" (class_member ";")* "}" 
    rules_[member_list] = {Sequence, {TokBraceOpen, rep_class_member_semicolon, TokBraceClose}};
      // (class_member ";")
      rules_[rep_class_member_semicolon] = {Repeat, {class_member, TokSemiColon}};

    // class_member // class member definition 
    //   : type name 
    //   | deffunc
    rules_[class_member] = {Select, {seq_type_name, deffunc}};
      // type name
      rules_[seq_type_name] = {Sequence, {type, name}};

    // typedef // ex) typedef int i32; 
    //   : <TYPEDEF> typeref <IDENTIFIER> ";" 
    rules_[typedef_] = {Sequence, {TokTypeDef, typeref, TokIdentifier, TokSemiColon}};

    // typeref 
    //   : typeref_base  ( "[""]"                    // unassigned array
    //                   | "["<INTEGER>"]"           // assigned array
    //                   | "*"                       // pointer 
    //                   | "(" param_typerefs ")")*  // function pointer 
    rules_[typeref] = {Sequence, {typeref_base, rep_sel_arry_ptr_fnptr}};
      //   ( "[""]"                    // unassigned array
      //   | "["<INTEGER>"]"           // assigned array
      //   | "*"                       // pointer 
      //   | "(" param_typerefs ")")*  // function pointer 
      rules_[rep_sel_arry_ptr_fnptr] = {Repeat, {sel_arry_ptr_fnptr}};
        //   "[""]"                    // unassigned array
        //   | "["<INTEGER>"]"           // assigned array
        //   | "*"                       // pointer 
        //   | "(" param_typerefs ")"  // function pointer 
        rules_[sel_arry_ptr_fnptr] = {Select, {seq_unassigned_array, seq_assigned_array, seq_ptr, seq_func_ptr}};
          // "[""]
          rules_[seq_unassigned_array] = {Sequence, {TokBracketOpen, TokBracketClose}};
          // "["<INTEGER>"]"
          rules_[seq_assigned_array] = {Sequence, {TokBracketOpen, TokIntegerLiteral, TokBracketClose}};
          rules_[seq_ptr] = {Sequence, {TokMul}};
          // "(" param_typerefs ")"
          rules_[seq_func_ptr] = {Sequence, {TokParenOpen, param_typerefs, TokParenClose}};

    // param_typerefs // function pointer param type definition 
    //   : <VOID> 
    //   | type ("," type)* ["," "..."] 
    rules_[param_typerefs] = {Select, {TokVoid, seq_type_rep_type_dot}};
      // type ("," type)* ["," "..."] 
      rules_[seq_type_rep_type_dot] = {Sequence, {type, rep_comma_type, opt_comma_dotdotdot}};
        // ("," type)*
        rules_[rep_comma_type] = {Repeat, {TokComma, type}};

    // stmts 
    //   : (stmt)* 
    rules_[stmts] = {Repeat, {stmt}};

    // stmt 
    //   :";" 
    //   | labeled_stmt 
    //   | expr ";" 
    //   | block 
    //   | if_stmt 
    //   | while_stmt 
    //   | dowhile_stmt 
    //   | for_stmt 
    //   | switch_stmt 
    //   | break_stmt 
    //   | continue_stmt 
    //   | goto_stmt 
    //   | return_stmt 
    rules_[stmt] = {Select, { TokSemiColon, 
                              labeled_stmt, 
                              seq_expr_semicolon,
                              block,
                              if_stmt,
                              while_stmt,
                              dowhile_stmt,
                              for_stmt,
                              switch_stmt,
                              break_stmt,
                              continue_stmt,
                              goto_stmt,
                              return_stmt}};
      rules_[seq_expr_semicolon] = {Sequence, {expr, TokSemiColon}};

    // labeled_stmt
    //   : <IDENTIFIER> ":" stmt
    rules_[labeled_stmt] = {Sequence, {TokIdentifier, TokColon, stmt}};
     
    // if_stmt  
    //   : <IF> "(" expr ")" stmt [<ELSE> stmt] 
    rules_[if_stmt] = {Sequence, {TokIf, TokParenOpen, expr, TokParenClose, opt_else_stmt}};
      rules_[opt_else_stmt] = {Options, {TokElse, stmt}}; // [<ELSE> stmt] 
     
    // while_stmt 
    //   : <WHILE> "(" expr ")" stmt 
    rules_[while_stmt] = {Sequence, {TokWhile, TokParenOpen, expr, TokParenClose, stmt}};
     
    // dowhile_stmt
    //   : <DO> stmt <WHILE> "(" expr ")" ";"
    rules_[dowhile_stmt] = {Sequence, {TokDo, stmt, TokWhile, TokParenOpen, expr, TokParenClose, TokSemiColon}};

    // for_stmt 
    //   : <FOR> "(" [expr] ";" [expr] ";" [expr] ")" stmt 
    rules_[for_stmt] = {Sequence, {TokFor, TokParenOpen, opt_expr, TokSemiColon, opt_expr, TokSemiColon, opt_expr, TokParenClose, stmt }};
      // [expr]
      rules_[opt_expr] = {Options, {expr}};

    // switch_stmt
    //   : <SWITCH> "(" expr ")" "{" case_clauses "}"
    rules_[switch_stmt] = {Sequence, {TokSwitch, TokParenOpen, expr, TokParenClose, TokBraceOpen, case_clauses, TokBraceClose}};

    // case_clauses
    //   : (case_clause)* [default_clause]
    rules_[case_clauses] = {Sequence, {rep_case_clause, opt_default_clause}};
      // (case_clause)*
      rules_[rep_case_clause] = {Repeat, {case_clause}};
      // [default_clause]
      rules_[opt_default_clause] = {Options, {default_clause}};
    
    // case_clause
    //   : cases case_body
    rules_[case_clause] = {Sequence, {cases, case_body}};
     
    // cases
    //   : <CASE> primary ":"
    rules_[cases] = {Sequence, {TokCase, primary, TokColon}};
    
    // default_clause
    //   : <DEFAULT> ":" case_body
    rules_[default_clause] = {Sequence, {TokDefault, TokColon, case_body}};
   
    // case_body
    //   : stmt
    rules_[case_body] = {Sequence, {stmt}};

    // break_stmt 
    //   : <BREAK> ";" 
    rules_[break_stmt] = {Sequence, {TokBreak, TokSemiColon}};

    // continue_stmt
    //   : <CONTINUE> ";"
    rules_[continue_stmt] = {Sequence, {TokContinue, TokSemiColon}};
     
    // goto_stmt
    //   : <GOTO> <IDENTIFIER> ";"
    rules_[continue_stmt] = {Sequence, {TokContinue, TokSemiColon}};
     
    // return_stmt 
    //   : <RETURN> ";" 
    //   | <RETURN> expr ";" 
    rules_[return_stmt] = {Select, {seq_return_semicolon, seq_return_expr_semicolon}};
      // <RETURN> ";"
      rules_[seq_return_semicolon] = {Sequence, {TokReturn, TokSemiColon}};
      // <RETURN> expr ";"
      rules_[seq_return_expr_semicolon] = {Sequence, {TokReturn, expr, TokSemiColon}};
     
    // expr 
    //   : term "=" expr 
    //   | term opassign_op expr 
    //   | expr10 
    rules_[expr] = {Select, {seq_term_eq_expr, seq_term_ops_expr, expr10}};
      // term "=" expr 
      rules_[seq_term_eq_expr] = {Select, {term, TokAssign, expr}};
      // term opassign_op expr 
      rules_[seq_term_ops_expr] = {Select, {term, opassign_op, expr}};
     
    // opassign_op 
    //   : "+=" 
    //   | "-=" 
    //   | "*=" 
    //   | "/=" 
    //   | "%=" 
    //   | "&=" 
    //   | "|=" 
    //   | "^=" 
    //   | "<<=" 
    //   | ">>=" 
    rules_[opassign_op] = {Select, {TokComAdd, 
                                    TokComSub,
                                    TokComMul,
                                    TokComDiv,
                                    TokComMod,
                                    TokComBitAnd,
                                    TokComBitOr,
                                    TokComBitXor,
                                    TokComBitShiftL,
                                    TokComBitShiftR}};
     
    // expr10 
    //   : expr9 ["?" expr ":" expr10] 
    rules_[expr10] = {Sequence, {expr9, opt_ternaryop}};
      // ["?" expr ":" expr10] 
      rules_[opt_ternaryop] = {Options, {TokQuestion, expr, TokColon, expr10}};
     
    // expr9 
    //   : expr8 ("||" expr8)* 
    rules_[expr9] = {Sequence, {expr8, rep_or_expr8}};
      //("||" expr8)* 
      rules_[rep_or_expr8] = {Repeat, {TokConOr, expr8}};
     
    // expr8 
    //   : expr7 ("&&" expr7)* 
    rules_[expr8] = {Sequence, {expr7, rep_and_expr7}};
      // ("&&" expr7)* 
      rules_[rep_and_expr7] = {Repeat, {TokConAnd, expr7}};
     
    // expr7  
    //   : expr7 (   ">" expr6  
    //              | "<" expr6 
    //              | ">=" expr6 
    //              | "<=" expr6 
    //              | "==" expr6 
    //              | "!=" expr6 
    //              )* 
    rules_[expr7] = {Sequence, {expr7, rep_op_expr6}};
      //(sel_op_expr6)*
      rules_[rep_op_expr6] = {Repeat, {sel_op_expr6}};
        // ">" expr6  
        //| "<" expr6 
        //| ">=" expr6 
        //| "<=" expr6 
        //| "==" expr6 
        //| "!=" expr6
        rules_[sel_op_expr6] = {Select, { seq_gr_expr6, 
                                          seq_ls_expr6,
                                          seq_geq_expr6,
                                          seq_leq_expr6,
                                          seq_eq_expr6,
                                          seq_neq_expr6 }};
          // ">" expr6  
          rules_[seq_gr_expr6] = {Sequence, {TokGreatorThan, expr6}};
          // "<" expr6 
          rules_[seq_ls_expr6] = {Sequence, {TokLessThan, expr6}};
          // ">=" expr6 
          rules_[seq_geq_expr6] = {Sequence, {TokGreatorThenEqual, expr6}};
          // "<=" expr6 
          rules_[seq_leq_expr6] = {Sequence, {TokLessThanEqual, expr6}};
          // "==" expr6 
          rules_[seq_eq_expr6] = {Sequence, {TokEqual, expr6}};
          // "!=" expr6 
          rules_[seq_neq_expr6] = {Sequence, {TokNotEqual, expr6}};
     
    // expr6 
    //   : expr5 ("|" expr5)* 
    rules_[expr6] = {Sequence, {expr5, rep_bitor_expr5}};
      // ("|" expr5)* 
      rules_[rep_bitor_expr5] = {Repeat, {TokBitOr, expr5}};

    // expr5 
    //   : expr4 ("^" expr4)* 
    rules_[expr5] = {Sequence, {expr4, rep_bitxor_expr4}};
      // ("^" expr4)* 
      rules_[rep_bitxor_expr4] = {Repeat, {TokBitXor, expr4}};
     
    // expr4 
    //   : expr3 ("&" expr3)* 
    rules_[expr4] = {Sequence, {expr3, rep_bitand_expr3}};
      // ("&" expr3)* 
      rules_[rep_bitand_expr3] = {Repeat, {TokBitAnd, expr3}};

    // expr3 
    //   : expr2 ( ">>" expr2 | "<<" expr2)* 
    rules_[expr3] = {Sequence, {expr2, rep_shift_expr2}};
      // ( ">>" expr2 | "<<" expr2)*
      rules_[rep_shift_expr2] = {Repeat, {sel_shift_expr2}};
      // ">>" expr2 | "<<" expr2
      rules_[sel_shift_expr2] = {Select, {seq_rshft_expr2, seq_lshft_expr2}};
        // ">>" expr2
        rules_[seq_rshft_expr2] = {Sequence, {TokBitShiftR, expr2}};
        // "<<" expr2
        rules_[seq_lshft_expr2] = {Sequence, {TokBitShiftL, expr2}};
     
    // expr2 
    //   : expr1 ( "+" expr1 | "-" expr1)* 
    rules_[expr2] = {Sequence, {expr1, rep_sumsub_expr1}};
      // ( "+" expr1 | "-" expr1)* 
      rules_[rep_sumsub_expr1] = {Repeat, {sel_sumsub_expr1}};
      // "+" expr1 | "-" expr1 
      rules_[sel_sumsub_expr1] = {Repeat, {seq_sum_expr1, seq_sub_expr1}};
        // "+" expr1
        rules_[seq_sum_expr1] = {Sequence, {TokAdd, expr1}};
        // "-" expr1
        rules_[seq_sub_expr1] = {Sequence, {TokSub, expr1}};
     
    // expr1 
    //   : term (  "*" term  
    //           | "/" term 
    //           | "%" term 
    //           )* 
    rules_[expr1] = {Sequence, {term, rep_muldivmod_term}};
      //("*" term 
      //| "/" term 
      //| "%" term)* 
      rules_[rep_muldivmod_term] = {Repeat, {sel_muldivmod_term}};
        //"*" term  
        //| "/" term 
        //| "%" term
        rules_[sel_muldivmod_term] = {Select, {seq_mul_term, seq_div_term, seq_mod_term}};
          // "*" term  
          rules_[seq_mul_term] = {Sequence, {TokMul, term}};
          // "/" term 
          rules_[seq_div_term] = {Sequence, {TokDiv, term}};
          // "%" term
          rules_[seq_mod_term] = {Sequence, {TokMod, term}};
     
    // term 
    //   : "(" type ")" term          // type casting 
    //   | unary 
    rules_[term] = {Select, {seq_type_term, unary}};
      //"(" type ")" term
      rules_[seq_type_term] = {Sequence, {TokParenOpen, type, TokParenClose, term}};
     
    // unary 
    //   : "++" unary                 // pre ++ 
    //   | "--" unary                 // pre -- 
    //   | "+" term                   // unary +, positive 
    //   | "-" term                   // unary -, negative 
    //   | "!" term                   // Logical negation 
    //   | "*" term                   // Pointer reference 
    //   | "&" term                   // adress operator 
    //   | <SIZEOF> "(" type ")"      // sizeof(type) 
    //   | <SIZEOF> unary             // sizeof unary 
    //   | postfix                    // postfix  
    rules_[unary] = {Select, {seq_preinc_unary, 
                              seq_predec_unary,
                              seq_pos_term,
                              seq_neg_term,
                              seq_not_term,
                              seq_ptr_term,
                              seq_adr_term,
                              seq_sizeof_type,
                              seq_sizeof_unary}};
      // "++" unary
      rules_[seq_preinc_unary] = {Sequence, {TokUnaryInc, unary}};
      // "--" unary
      rules_[seq_predec_unary] = {Sequence, {TokUnaryDec, unary}};
      // "+" term 
      rules_[seq_pos_term] = {Sequence, {TokAdd, term}};
      // "-" term
      rules_[seq_neg_term] = {Sequence, {TokSub, term}};
      // "!" term 
      rules_[seq_not_term] = {Sequence, {TokConNot, term}};
      // "*" term
      rules_[seq_ptr_term] = {Sequence, {TokMul, term}};
      // "&" term 
      rules_[seq_adr_term] = {Sequence, {TokBitAnd, term}};
      // <SIZEOF> "(" type ")"
      rules_[seq_sizeof_type] = {Sequence, {TokSizeOf, TokParenOpen, type, TokParenClose}};
      // <SIZEOF> unary
      rules_[seq_sizeof_unary] = {Sequence, {TokSizeOf, unary}};
     
    // postfix 
    //   : primary ("++"               // post ++ 
    //             |"--"               // post -- 
    //             |"[" expr "]"       // array reference 
    //             |"." name           // class member reference 
    //             |"->" name          // class member pointer reference 
    //             |"(" args ")"       // function call 
    //             )* 
    rules_[postfix] = {Sequence, {primary, rep_reffunc}};
      //("++" | "--" | "[" expr "]" | "." name | "->" name | "(" args ")")*
      rules_[rep_reffunc] = {Repeat, {sel_reffunc}};
        // "++" | "--" | "[" expr "]" | "." name | "->" name | "(" args ")"
        rules_[sel_reffunc] = {Select, {seq_post_inc, seq_post_dec, seq_array_reference, seq_dot_name, seq_arrow_name, seq_po_args_pc}};
          // "++"
          rules_[seq_post_inc] = {Sequence, {TokUnaryInc}};
          // "--"
          rules_[seq_post_dec] = {Sequence, {TokUnaryDec}};
          //"[" expr "]"
          rules_[seq_array_reference] = {Sequence, {TokBracketOpen, expr, TokBracketClose}};
          //"." name
          rules_[seq_dot_name] = {Sequence, {TokDot, name}};
          //"->" name
          rules_[seq_arrow_name] = {Sequence, {TokRightArrow, name}};
          //"(" args ")"
          rules_[seq_po_args_pc] = {Sequence, {TokParenOpen, args, TokParenClose}};
     
    // args 
    //   : [expr ("," expr)*] 
    rules_[args] = {Options, {seq_args_expr, rep_args_expr}};
      // expr
      rules_[seq_args_expr] = {Sequence, {expr}};
      // ("," expr)*
      rules_[rep_args_expr] = {Repeat, {TokComma, expr}};
     
    // primary 
    //   : <INTEGER> 
    //   | <CHARACTER> 
    //   | <STRING> 
    //   | <IDENTIFIER> 
    //   | "(" expr ")" 
    rules_[primary] = {Select, {TokIntegerLiteral, TokCharactorLiteral, TokStringLiteral, TokIdentifier, seq_po_expr_pc}};
      // "(" expr ")" 
      rules_[seq_po_expr_pc] = {Sequence, {TokParenOpen, expr, TokParenClose,}};
  }
  
  void SyntaxAnalyzer::InitRuleAction() {
    for (int i = 0; i < MAX_RULES; i++) {
      rule_actions_[i] = &SyntaxAnalyzer::DoNothing;
    }
    
    rule_actions_[typeref_base] = &SyntaxAnalyzer::TypeRefBase;
      rule_actions_[seq_void] = &SyntaxAnalyzer::Act_seq_void; 
      rule_actions_[seq_char] = &SyntaxAnalyzer::Act_seq_char;
      rule_actions_[seq_short] = &SyntaxAnalyzer::Act_seq_short;
      rule_actions_[seq_int] = &SyntaxAnalyzer::Act_seq_int;
      rule_actions_[seq_long] = &SyntaxAnalyzer::Act_seq_long;
      rule_actions_[seq_unsigned_char] = &SyntaxAnalyzer::Act_seq_unsigned_char;
      rule_actions_[seq_unsigned_short] = &SyntaxAnalyzer::Act_seq_unsigned_short;
      rule_actions_[seq_unsigned_int] = &SyntaxAnalyzer::Act_seq_unsigned_int;
      rule_actions_[seq_unsigned_long] = &SyntaxAnalyzer::Act_seq_unsigned_long;
      rule_actions_[seq_float] = &SyntaxAnalyzer::Act_seq_float;
      rule_actions_[seq_double] = &SyntaxAnalyzer::Act_seq_double;
      rule_actions_[seq_class_identifier] = &SyntaxAnalyzer::Act_seq_class_identifier;

    rule_actions_[name] = &SyntaxAnalyzer::Name;
    rule_actions_[term] = &SyntaxAnalyzer::Term;
      rule_actions_[seq_type_term] = &SyntaxAnalyzer::Act_seq_type_term;

    rule_actions_[unary] = &SyntaxAnalyzer::Unary;
      rule_actions_[seq_preinc_unary] = &SyntaxAnalyzer::Act_seq_preinc_unary;
      rule_actions_[seq_predec_unary] = &SyntaxAnalyzer::Act_seq_predec_unary;
      rule_actions_[seq_pos_term] = &SyntaxAnalyzer::Act_seq_pos_term;
      rule_actions_[seq_neg_term] = &SyntaxAnalyzer::Act_seq_neg_term;
      rule_actions_[seq_not_term] = &SyntaxAnalyzer::Act_seq_not_term;
      rule_actions_[seq_ptr_term] = &SyntaxAnalyzer::Act_seq_ptr_term;
      rule_actions_[seq_adr_term] = &SyntaxAnalyzer::Act_seq_adr_term;
      rule_actions_[seq_sizeof_type] = &SyntaxAnalyzer::Act_seq_sizeof_type;
      rule_actions_[seq_sizeof_unary] = &SyntaxAnalyzer::Act_seq_sizeof_unary;

    rule_actions_[postfix] = &SyntaxAnalyzer::Postfix;
      rule_actions_[seq_post_inc] = &SyntaxAnalyzer::Act_seq_post_inc;
      rule_actions_[seq_post_dec] = &SyntaxAnalyzer::Act_seq_post_dec;
      rule_actions_[seq_array_reference] = &SyntaxAnalyzer::Act_seq_array_reference;
      rule_actions_[seq_dot_name] = &SyntaxAnalyzer::Act_seq_dot_name;
      rule_actions_[seq_arrow_name] = &SyntaxAnalyzer::Act_seq_arrow_name;
      rule_actions_[seq_po_args_pc] = &SyntaxAnalyzer::Act_seq_po_args_pc;

    rule_actions_[args] = &SyntaxAnalyzer::Args;
      rule_actions_[seq_args_expr] = &SyntaxAnalyzer::Act_seq_args_expr;
      rule_actions_[rep_args_expr] = &SyntaxAnalyzer::Act_rep_args_expr;

    rule_actions_[primary] = &SyntaxAnalyzer::Primary;
      rule_actions_[seq_po_expr_pc] = &SyntaxAnalyzer::Act_seq_po_expr_pc;

    rule_actions_[TokIntegerLiteral] = &SyntaxAnalyzer::ActTokIntegerLiteral;
    rule_actions_[TokCharactorLiteral] = &SyntaxAnalyzer::ActTokCharacterLiteral;
    rule_actions_[TokStringLiteral] = &SyntaxAnalyzer::ActTokStringLiteral;
    rule_actions_[TokIdentifier] = &SyntaxAnalyzer::ActIdentifier;
  }

  eResult SyntaxAnalyzer::TraverseRule(int entry) {
    Rule rule = rules_[entry];
    eResult res;
    int tok_pos;

    switch(rule.action_) {
      case Repeat:
        {
          while(true) {
            tok_pos = tokenizer_->GetTokPos(); // backup token position
            for (int i = 0; rule.sub_rules_[i] > -1; i++) {
              res = TraverseRule(rule.sub_rules_[i]);
              if (res == False)
                if (rules_[rule.sub_rules_[i]].action_ != Options) {
                  tokenizer_->SetTokPos(tok_pos); // restore token position.
                  // Run action
                  (this->*rule_actions_[entry])();
                  return True;
                }
              else if (res == Error)
                return Error;
            }
          };
        }
        break;

      case Select:
        {
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (int i = 0; rule.sub_rules_[i] > -1; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True) {
              // Run action
              (this->*rule_actions_[entry])();
              return True; // found matching
            }
            else if (res == False) {
              tokenizer_->SetTokPos(tok_pos); // restore token position.
              return False;
            }
            else if (res == Error)
              return Error;
          }
          return False;
        }
        break;

      case Sequence:
        {
          int matching_count = 0;
          int i = 0;
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (; rule.sub_rules_[i] > -1; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True) { // matched
              matching_count++;
            }
            else if (res == False) { // not matching
              if (rules_[rule.sub_rules_[i]].action_ == Options) {
                matching_count++;
              }
            }
            else
              return Error;
          }

          if (matching_count == i) {
            // if every rules are matching, run action.
            (this->*rule_actions_[entry])();
            return True;
          }
          tokenizer_->SetTokPos(tok_pos); // restore token position.
          return False; // unmatching
        }
        break;

      case Options:
        {
          int matching_count = 0;
          int i = 0;
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (; rule.sub_rules_[i] > -1; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True) {
              matching_count++;
            }
            else
              return Error;
          }

          if (matching_count == i) {
            // Run action
            (this->*rule_actions_[entry])();
            return True;
          }

          tokenizer_->SetTokPos(tok_pos); // restore token position.
          return False; // unmatching
        }
        break;

      case Terminal:
        {
          if(tokenizer_->isToken(0, Lexer::TokenType(rule.sub_rules_[0]))) {
            // Run action
            (this->*rule_actions_[rule.sub_rules_[0]])();
            tokenizer_->ConsumeToken(1);
            return True;
          }
          return False;
        }
        break;

      default:
        assert("Error wrong rule action.");
        break;
    }
  }

  SyntaxAnalyzer::SyntaxAnalyzer(SyntaxAction* sa, Tokenizer* tk, 
      ErrorDiag::Diagnosis* ed) {
    tokenizer_ = tk;
    action_ = sa;
    err_diag_ = ed;
  }

  SyntaxAnalyzer::~SyntaxAnalyzer() {
  }

  eResult SyntaxAnalyzer::PushToken(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::TokenType;
    pi.data_.tok_type_ = tok.type;
    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::CompilationUnit(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ImportStmts(void) {
    return True;
  }

  eResult SyntaxAnalyzer::TopDefs(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Name(void) {
    // check if stack top is Identifier.
    ParseInfo pi = parse_stack_.Top();
    if(pi.type_ == ParseInfo::Identifier)
      return True;
    return Error;
  }

  eResult SyntaxAnalyzer::Storage(void) {
    return False;
  }

  eResult SyntaxAnalyzer::Type(void) {
    return True;
  }

  eResult SyntaxAnalyzer::TypeRef(void) {
    // <<== working here
    return False;
  }

  eResult SyntaxAnalyzer::TypeRefBase(void) {
    // <<== working here
    return False;
  }

  // create primitive types
  eResult SyntaxAnalyzer::Act_seq_void(void) {
  //    AST::VoidType * ty = new AST::VoidType();
  //
  //    ParseInfo pi_new;
  //    pi_new.type_ = ParseInfo::ASTType;
  //    pi_new.data_.node_ = node;
  //    parse_stack_.Push(pi_new);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_char(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_short(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_int(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_long(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_char(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_short(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_int(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_long(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_float(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_double(void){
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_class_identifier(void){
    return True;
  }

  eResult SyntaxAnalyzer::TypeDef(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ParamTypeRefs(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Block(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Param(void) {
    return False;
  }

  eResult SyntaxAnalyzer::FixedParams(void) {
    return False;
  }

  eResult SyntaxAnalyzer::Params(void) {
    return True;
  }

  eResult SyntaxAnalyzer::DefFunc(void) {
    return True;
  }

  eResult SyntaxAnalyzer::DefVars(void) {
    return True;
  }

  eResult SyntaxAnalyzer::DefVarList(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Term(void) {
    // actually do nothing
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on term casting");
      return Error;
    }
    return True;
  }

  //"(" type ")" term          // type casting 
  eResult SyntaxAnalyzer::Act_seq_type_term(void) {
    // get term
    ParseInfo pi_term = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_term.type_ != ParseInfo::ASTNode || 
        !pi_term.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on term casting");
      return Error;
    }

    // get type
    ParseInfo pi_type = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_type.type_ != ParseInfo::ASTNode || 
        !pi_type.data_.node_->IsKindOf(AST::BaseNode::TypeNodeTy)) {
      assert("Error on term TypeNode");
      return Error;
    }

    AST::CastNode* node = 
      new AST::CastNode((AST::ExprNode*)pi_term.data_.node_,
          (AST::TypeNode*)pi_type.data_.node_);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // unary
  eResult SyntaxAnalyzer::Unary(void) {
    // actually do nothing
    if (parse_stack_.IsEmpty())  {
      assert("Error on Postfix() : Needed parsing info!");
      return Error;
    }

    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode) {
      assert("Error on Postfix() : Needed ASTNode in stack!");
      return Error;
    }

    return True;
  }

  // "++" unary
  eResult SyntaxAnalyzer::Act_seq_preinc_unary(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary prefix increase");
      return Error;
    }

    AST::PrefixOpNode* node = 
      new AST::PrefixOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Inc);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // "--" unary
  eResult SyntaxAnalyzer::Act_seq_predec_unary(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary prefix increase");
      return Error;
    }

    AST::PrefixOpNode* node = 
      new AST::PrefixOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Dec);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // "+" term 
  eResult SyntaxAnalyzer::Act_seq_pos_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::UnaryOpNode* node = 
      new AST::UnaryOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Pos);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // "-" term 
  eResult SyntaxAnalyzer::Act_seq_neg_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::UnaryOpNode* node = 
      new AST::UnaryOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Neg);
    
    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // "!" term 
  eResult SyntaxAnalyzer::Act_seq_not_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::UnaryOpNode* node = 
      new AST::UnaryOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Not);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // "*" term 
  eResult SyntaxAnalyzer::Act_seq_ptr_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::DereferenceNode* node = 
      new AST::DereferenceNode((AST::ExprNode*)pi_expr.data_.node_);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // "&" term 
  eResult SyntaxAnalyzer::Act_seq_adr_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::AddressNode * node = 
      new AST::AddressNode((AST::ExprNode*)pi_expr.data_.node_);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // <SIZEOF> "(" type ")"
  eResult SyntaxAnalyzer::Act_seq_sizeof_type(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::SizeofTypeNode* node = 
      new AST::SizeofTypeNode((AST::ExprNode*)pi_expr.data_.node_);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // <SIZEOF> unary
  eResult SyntaxAnalyzer::Act_seq_sizeof_unary(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::SizeofExprNode* node = 
      new AST::SizeofExprNode((AST::ExprNode*)pi_expr.data_.node_);

    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  eResult SyntaxAnalyzer::Postfix(void) {
    if (parse_stack_.IsEmpty())  {
      assert("Error on Postfix() : Needed parsing info!");
      return Error;
    }

    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode) {
      assert("Error on Postfix() : Needed ASTNode in stack!");
      return Error;
    }

    return True;
  }

  // "++"
  eResult SyntaxAnalyzer::Act_seq_post_inc(void) {
    ParseInfo pi_expr, pi_new;
    pi_expr = parse_stack_.Top(); // get stored Expr
    parse_stack_.Pop();
    if (!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix unary increase");
      return Error;
    }

    // create suffix node
    AST::SuffixOpNode* node = 
      new AST::SuffixOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Inc);
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // "--"
  eResult SyntaxAnalyzer::Act_seq_post_dec(void) {
    ParseInfo pi_expr, pi_new;
    pi_expr = parse_stack_.Top(); // get stored Expr
    parse_stack_.Pop();
    
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix unary decrease");
      return Error;
    }
    
    if (!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix unary decrease");
      return Error;
    }

    AST::SuffixOpNode* node = 
      new AST::SuffixOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Dec);

    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;

    parse_stack_.Push(pi_new);
    return True;
  }

  //"[" expr "]"
  eResult SyntaxAnalyzer::Act_seq_array_reference(void) {
    // get array size expr
    ParseInfo pi_arrsize_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arrsize_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix array reference");
      return Error;
    }
    if(!pi_arrsize_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix array reference");
      return Error;
    }

    // get expr
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix array reference");
      return Error;
    }
    if(!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix array reference");
      return Error;
    }

    AST::ArrayRefNode * arnode = 
      new AST::ArrayRefNode((AST::ExprNode*)pi_expr.data_.node_, 
                            (AST::ExprNode*)pi_arrsize_expr.data_.node_);
    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = (AST::BaseNode*)arnode;
    parse_stack_.Push(pi_new);

    return True;
  }
  
  // "." name
  eResult SyntaxAnalyzer::Act_seq_dot_name(void) {
    // get member name
    ParseInfo pi_name = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_name.type_ != ParseInfo::Identifier) {
      assert("Error on Postfix member reference");
      return Error;
    }

    // get expr
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix array reference");
      return Error;
    }
    if(!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix array reference");
      return Error;
    }

    // create new pi
    ParseInfo pi_new;
    AST::MemberRefNode* memref_node = 
      new AST::MemberRefNode((AST::ExprNode*)pi_expr.data_.node_, pi_name.data_.cstr_);
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = (AST::BaseNode*)memref_node;
    parse_stack_.Push(pi_new);
    return True;
  }

  // "->" name
  eResult SyntaxAnalyzer::Act_seq_arrow_name(void) {
    // get member name
    ParseInfo pi_name = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_name.type_ != ParseInfo::Identifier) {
      assert("Error on Postfix member reference");
      return Error;
    }

    // get expr
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix array reference");
      return Error;
    }
    if(!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix array reference");
      return Error;
    }

    // create new pi
    ParseInfo pi_new;
    AST::PtrMemberRefNode* ptrmemref_node = 
      new AST::PtrMemberRefNode((AST::ExprNode*)pi_expr.data_.node_, pi_name.data_.cstr_);
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = (AST::BaseNode*)ptrmemref_node;
    parse_stack_.Push(pi_new);
    return True;
  }

  //"(" args ")"
  eResult SyntaxAnalyzer::Act_seq_po_args_pc(void) {
    // get args
    ParseInfo pi_args = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_args.type_ != ParseInfo::ASTNode)
      return Error;
    if (!pi_args.data_.node_->IsKindOf(AST::BaseNode::ArgsNodeTy))
      return Error;
    // get function 
    ParseInfo pi_func = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_func.type_ != ParseInfo::ASTNode)
      return Error;
    if (!pi_func.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy))
      return Error;

    // create new pi
    ParseInfo pi_new;
    AST::FuncCallNode* fcall= 
      new AST::FuncCallNode((AST::ExprNode*)pi_func.data_.node_, 
          (AST::ArgsNode*)pi_args.data_.node_);
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = (AST::BaseNode*)fcall;
    parse_stack_.Push(pi_new);

    return True;
  }

  eResult SyntaxAnalyzer::Primary(void) {
    if (parse_stack_.IsEmpty())  {
      assert("Error on Primary() : Need to child data!");
      return Error;
    }
    
    ParseInfo new_pi, top_pi = parse_stack_.Top();
    parse_stack_.Pop();
    if (top_pi.type_ == ParseInfo::Integer) {
      AST::IntegerLiteralNode * node = 
        new AST::IntegerLiteralNode(AST::IntegerLiteralNode::Int, top_pi.data_.integer_);
      new_pi.type_ = ParseInfo::ASTNode;
      new_pi.data_.node_ = node;
      parse_stack_.Push(new_pi);
    }
    else if (top_pi.type_ == ParseInfo::Character) {
      AST::IntegerLiteralNode * node = 
        new AST::IntegerLiteralNode(AST::IntegerLiteralNode::Char, top_pi.data_.integer_);
      new_pi.type_ = ParseInfo::ASTNode;
      new_pi.data_.node_ = node;
      parse_stack_.Push(new_pi);
    }
    else if (top_pi.type_ == ParseInfo::String) {
      AST::StringLiteralNode* node = 
        new AST::StringLiteralNode(top_pi.data_.cstr_, top_pi.cstr_len_);
      new_pi.type_ = ParseInfo::ASTNode;
      new_pi.data_.node_ = node;
      parse_stack_.Push(new_pi);
    }
    else if (top_pi.type_ == ParseInfo::Identifier) {
      AST::VariableNode* node = 
        new AST::VariableNode(top_pi.data_.cstr_, top_pi.cstr_len_);
      new_pi.type_ = ParseInfo::ASTNode;
      new_pi.data_.node_ = node;
      parse_stack_.Push(new_pi);
    }
    else if (top_pi.type_ == ParseInfo::ASTNode &&
        top_pi.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      // Do nothing
    }
    else
      return Error;

    return True;
  }

  // "(" expr ")" 
  eResult SyntaxAnalyzer::Act_seq_po_expr_pc(void) {
    ParseInfo pi = parse_stack_.Top();
    // Check if top has expr ASTNode
    if(pi.type_ == ParseInfo::ASTNode && 
        pi.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy))
      return True;
    return False;
  }

  // args 
  //   : [expr ("," expr)*] 
  eResult SyntaxAnalyzer::Args(void) {
    // get ArgsNode
    ParseInfo pi_arg_node = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_node.type_ == ParseInfo::ASTNode && 
        pi_arg_node.data_.node_->IsKindOf(AST::BaseNode::ArgsNodeTy)) {
      return True;
    }

    // if there isn't appropriate ArgsNode, create new one.
    ParseInfo pi_new;
    AST::ArgsNode* args_node = new AST::ArgsNode();

    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = (AST::BaseNode*)args_node;
    parse_stack_.Push(pi_new);
    return True;
  }
  
  // expr // first function argument
  eResult SyntaxAnalyzer::Act_seq_args_expr(void) {
    // get first argument expr
    ParseInfo pi_arg_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_expr.type_ != ParseInfo::ASTNode || 
        !pi_arg_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on Args first expr");
      return Error;
    }

    // create new pi
    ParseInfo pi_new;
    AST::ArgsNode* args_node = new AST::ArgsNode();
    args_node->Add((AST::ExprNode*)pi_arg_expr.data_.node_);

    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = (AST::BaseNode*)args_node;
    parse_stack_.Push(pi_new);

    return True;
  }

  // ("," expr)*
  eResult SyntaxAnalyzer::Act_rep_args_expr(void) {
    // get argument expr
    ParseInfo pi_arg_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_expr.type_ != ParseInfo::ASTNode || 
        !pi_arg_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on Args first expr");
      return Error;
    }

    // get ArgsNode
    ParseInfo pi_arg_node = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_node.type_ != ParseInfo::ASTNode || 
      !pi_arg_node.data_.node_->IsKindOf(AST::BaseNode::ArgsNodeTy)) {
      assert("Error on Args first expr");
      return Error;
    }

    // create new pi
    ParseInfo pi_new;
    AST::ArgsNode* args_node = (AST::ArgsNode*)pi_arg_node.data_.node_;
    args_node->Add((AST::ExprNode*)pi_arg_expr.data_.node_);

    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = (AST::BaseNode*)args_node;
    parse_stack_.Push(pi_new);
    return True;
  }

  eResult SyntaxAnalyzer::OpAssignOp(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr10(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr9(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr8(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr7(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr6(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr5(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr4(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr3(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr2(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr1(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Stmts(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Stmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::LabeledStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::IfStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::WhileStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::DoWhileStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ForStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::BreakStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ContinueStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::GotoStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ReturnStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ActTokIntegerLiteral(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Integer;
    pi.data_.integer_ = atol(tok.c);
    pi.token_idx_ = tokenizer_->GetTokPos();
    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokCharacterLiteral(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Character;
    pi.data_.character_ = *tok.c;
    pi.token_idx_ = tokenizer_->GetTokPos();
    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokStringLiteral(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::String;
    pi.data_.cstr_ = tok.c;
    pi.cstr_len_ = tok.len; // string length
    pi.token_idx_ = tokenizer_->GetTokPos();
    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActIdentifier(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Identifier;
    pi.data_.cstr_ = tok.c;
    pi.token_idx_ = tokenizer_->GetTokPos();
    pi.cstr_len_ = tok.len; // string length
    parse_stack_.Push(pi);
    return True;
  }


  void SyntaxAnalyzer::DebugPrint(void) {
  }

}
