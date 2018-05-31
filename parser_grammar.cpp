#include "parser.h"

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
        rules_[sel_arry_ptr_fnptr] = {Select, {seq_unassigned_array, seq_assigned_array, seq_ptr, seq_func}};
          // "[""]
          rules_[seq_unassigned_array] = {Sequence, {TokBracketOpen, TokBracketClose}};
          // "["<INTEGER>"]"
          rules_[seq_assigned_array] = {Sequence, {TokBracketOpen, TokIntegerLiteral, TokBracketClose}};
          // "*"
          rules_[seq_ptr] = {Sequence, {TokMul}};
          // "(" param_typerefs ")"
          rules_[seq_func] = {Sequence, {TokParenOpen, param_typerefs, TokParenClose}};

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

    rule_actions_[typeref] = &SyntaxAnalyzer::TypeRef;
      rule_actions_[seq_unassigned_array] = &SyntaxAnalyzer::Act_seq_unassigned_array; 
      rule_actions_[seq_assigned_array] = &SyntaxAnalyzer::Act_seq_assigned_array; 
      rule_actions_[seq_ptr] = &SyntaxAnalyzer::Act_seq_ptr; 
      rule_actions_[seq_func] = &SyntaxAnalyzer::Act_seq_func; 
    
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
}
