#include "parser.h"

namespace Parser {
  void SyntaxAnalyzer::InitBasicRule() {

    for (int i = 0; i < MAX_RULES; i++) {
      for (int j = 0; j < MAX_SUB_RULES; j++) {
        rules_[i].sub_rules_[j] = -1;
      }
    }

    // Initialize rules as Terminal.
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
    rules_[import_stmts] = {RepeatStar, {import_stmt}};
      // import_stmt 
      //   : <IMPORT> name ("." name)* ";" 
      rules_[import_stmt] = {Sequence, {TokImport, name, rep_dot_name, TokSemiColon}};
        // ("." name)*
        rules_[rep_dot_name] = {RepeatStar, {TokDot, name}};

    // name 
    //   : <IDENTIFIER> 
    rules_[name] = {Sequence, {TokIdentifier}};

    // top_defs // top definitions 
    //   : ( deffunc 
    //     | defvars 
    //     | defconst 
    //     | defclass 
    //     | typedef )* 
    rules_[top_defs] = {RepeatStar, {sel_fun_var_const_class_typedef}};
      rules_[sel_fun_var_const_class_typedef] = {Select, {deffunc, defvars, defconst, defclass, typedef_ }};

    // defvars // variable definition. ex) int a = 0, b=19; 
    //   : storage type name ["=" expr] [("," name ["=" expr])*] ";" 
    rules_[defvars] = {Sequence, {storage, type, name, opt_var_initialize, opt_rep_var_initialize, TokSemiColon }};
      rules_[opt_var_initialize] = {Options, {TokAssign, expr}}; // ["=" expr]
      // [("," name ["=" expr])*]
      rules_[opt_rep_var_initialize] = {Options, {rep_var_initialize}}; 
        // ("," name ["=" expr])*
        rules_[rep_var_initialize] = {RepeatStar, {TokComma, name, opt_var_initialize}};
        
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
    rules_[params] = {Select, {seq_param_void, seq_param_list}};
      // <VOID>
      rules_[seq_param_void] = {Sequence, {TokVoid}};
      // fixedparams ["," "..."] 
      rules_[seq_param_list] = {Sequence, {fixedparams, opt_vararg}};
        // ["," "..."] 
        rules_[opt_vararg] = {Options, {TokComma, TokDotDotDot}};

    // fixedparams // fixed parameter definition 
    //   : param ("," param)* 
    rules_[fixedparams] = {Sequence, {param, rep_comma_param}};
      // ("," param)* 
      rules_[rep_comma_param] = {RepeatStar, {TokComma, param}};
      
    // param 
    //   : type name 
    rules_[param] = {Sequence, {type, name}};

    // defvar_list 
    //   : (defvars)*
    rules_[defvar_list] = {RepeatStar, {defvars}};

    // block 
    //   : "{" defvar_list stmts "}" 
    rules_[block] = {Sequence, {TokBraceOpen, defvar_list, stmts, TokBraceClose}};

    // defclass // class definition 
    //   : <CLASS> name class_member_list ";" 
    rules_[defclass] = {Sequence, {TokClass, name, class_member_list, TokSemiColon}};
      // class_member_list //  
      //   : "{" (class_member ";")* "}" 
      rules_[class_member_list] = {Sequence, {TokBraceOpen, rep_class_member, TokBraceClose}};
        // (class_member ";")*
        rules_[rep_class_member] = {RepeatStar, {class_member, TokSemiColon}};
          // class_member // class member definition 
          //   : defvars 
          //   | deffunc
          rules_[class_member] = {Select, {seq_class_member_variable, seq_class_member_function}};
            // defvars 
            rules_[seq_class_member_variable] = {Sequence, {defvars}};
            // deffunc
            rules_[seq_class_member_function] = {Sequence, {deffunc}};

    // typedef // ex) typedef int i32; 
    //   : <TYPEDEF> typeref <IDENTIFIER> ";" 
    rules_[typedef_] = {Sequence, {TokTypeDef, typeref, TokIdentifier, TokSemiColon}};

    // typeref 
    //   : typeref_base  ( "[""]"                    // unassigned array
    //                   | "["<INTEGER>"]"           // assigned array
    //                   | "*"                       // pointer 
    //                   | "(" param_typerefs ")")*  // function type
    rules_[typeref] = {Sequence, {typeref_base, rep_sel_arry_ptr_fnptr}};
      //   ( "[""]"                    // unassigned array
      //   | "["<INTEGER>"]"           // assigned array
      //   | "*"                       // pointer 
      //   | "(" param_typerefs ")")*  // function type 
      rules_[rep_sel_arry_ptr_fnptr] = {RepeatStar, {sel_arry_ptr_fnptr}};
        //   "[""]"                    // unassigned array
        //   | "["<INTEGER>"]"           // assigned array
        //   | "*"                       // pointer 
        //   | "(" param_typerefs ")"  // function type
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
    //   | typeref ("," typeref)* ["," "..."] 
    rules_[param_typerefs] = {Select, {seq_param_type_void, seq_param_type_list}};
      // <VOID>
      rules_[seq_param_type_void] = {Sequence, {TokVoid}};
      // typeref ("," typeref)* ["," "..."] 
      rules_[seq_param_type_list] = {Sequence, {typeref, rep_param_comma_type, opt_vararg_type}};
        // ("," typeref)*
        rules_[rep_param_comma_type] = {RepeatStar, {TokComma, typeref}};
        // ["," "..."] 
        rules_[opt_vararg_type] = {RepeatStar, {TokComma, TokDotDotDot}};

    // stmts 
    //   : (stmt)* 
    rules_[stmts] = {RepeatStar, {stmt}};

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
                              expr_stmt,
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

    // labeled_stmt
    //   : <IDENTIFIER> ":" stmt
    rules_[labeled_stmt] = {Sequence, {TokIdentifier, TokColon, stmt}};

    // expr_stmt
    //  : expr ";"
    rules_[expr_stmt] = {Sequence, {expr, TokSemiColon}};
     
    // if_stmt  
    //   : <IF> "(" expr ")" stmt [<ELSE> stmt] 
    rules_[if_stmt] = {Sequence, {TokIf, TokParenOpen, expr, TokParenClose, stmt, opt_else_stmt}};
      rules_[opt_else_stmt] = {Options, {TokElse, stmt}}; // [<ELSE> stmt] 
     
    // while_stmt 
    //   : <WHILE> "(" expr ")" stmt 
    rules_[while_stmt] = {Sequence, {TokWhile, TokParenOpen, expr, TokParenClose, stmt}};
     
    // dowhile_stmt
    //   : <DO> stmt <WHILE> "(" expr ")" ";"
    rules_[dowhile_stmt] = {Sequence, {TokDo, stmt, TokWhile, TokParenOpen, expr, TokParenClose, TokSemiColon}};

    // for_stmt 
    //   : <FOR> "(" [expr] ";" [expr] ";" [expr] ")" stmt 
    rules_[for_stmt] = {Sequence, {TokFor, TokParenOpen, opt_for_init_expr, TokSemiColon, opt_for_cond_expr, TokSemiColon, opt_for_inc_expr, TokParenClose, stmt }};
      // [expr]
      rules_[opt_for_init_expr] = {Options, {expr}};
      // [expr]
      rules_[opt_for_cond_expr] = {Options, {expr}};
      // [expr]
      rules_[opt_for_inc_expr] = {Options, {expr}};

    // switch_stmt
    //   : <SWITCH> "(" expr ")" "{" case_clauses "}"
    rules_[switch_stmt] = {Sequence, {TokSwitch, TokParenOpen, expr, TokParenClose, TokBraceOpen, case_clauses, TokBraceClose}};

    // case_clauses
    //   : (case_clause)* [default_clause]
    rules_[case_clauses] = {Sequence, {rep_case_clause, opt_default_clause}};
      // (case_clause)*
      rules_[rep_case_clause] = {RepeatStar, {case_clause}};
      // [default_clause]
      rules_[opt_default_clause] = {Options, {default_clause}};
    
    // case_clause
    //   : case_list case_body
    rules_[case_clause] = {Sequence, {case_list, case_body}};
     
    // case_list
    //   : (<CASE> primary ":")+
    rules_[case_list] = {RepeatDagger, {TokCase, primary, TokColon}};
    
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
    rules_[goto_stmt] = {Sequence, {TokGoto, TokIdentifier, TokSemiColon}};
     
    // return_stmt 
    //   : <RETURN> ";" 
    //   | <RETURN> expr ";" 
    rules_[return_stmt] = {Select, {seq_return, seq_return_expr}};
      // <RETURN> ";"
      rules_[seq_return] = {Sequence, {TokReturn, TokSemiColon}};
      // <RETURN> expr ";"
      rules_[seq_return_expr] = {Sequence, {TokReturn, expr, TokSemiColon}};
     
    // expr 
    //   : term "=" expr 
    //   | term opassign_op expr 
    //   | expr10 
    rules_[expr] = {Select, {seq_assign_value, seq_opassign_value, expr10}};
      // term "=" expr 
      rules_[seq_assign_value] = {Select, {term, TokAssign, expr}};
      // term opassign_op expr 
      rules_[seq_opassign_value] = {Select, {term, opassign_op, expr}};
     
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
      rules_[rep_or_expr8] = {RepeatStar, {TokConOr, expr8}};
     
    // expr8 
    //   : expr7 ("&&" expr7)* 
    rules_[expr8] = {Sequence, {expr7, rep_and_expr7}};
      // ("&&" expr7)* 
      rules_[rep_and_expr7] = {RepeatStar, {TokConAnd, expr7}};
     
    // expr7  
    //   : expr7 ( ">" expr6  
    //           | "<" expr6 
    //           | ">=" expr6 
    //           | "<=" expr6 
    //           | "==" expr6 
    //           | "!=" expr6 )* 
    //
    rules_[expr7] = {Sequence, {expr7, rep_op_expr6}};
      //(sel_op_expr6)*
      rules_[rep_op_expr6] = {RepeatStar, {sel_op_expr6}};
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
      rules_[rep_bitor_expr5] = {RepeatStar, {TokBitOr, expr5}};

    // expr5 
    //   : expr4 ("^" expr4)* 
    rules_[expr5] = {Sequence, {expr4, rep_bitxor_expr4}};
      // ("^" expr4)* 
      rules_[rep_bitxor_expr4] = {RepeatStar, {TokBitXor, expr4}};
     
    // expr4 
    //   : expr3 ("&" expr3)* 
    rules_[expr4] = {Sequence, {expr3, rep_bitand_expr3}};
      // ("&" expr3)* 
      rules_[rep_bitand_expr3] = {RepeatStar, {TokBitAnd, expr3}};

    // expr3 
    //   : expr2 ( ">>" expr2 | "<<" expr2)* 
    rules_[expr3] = {Sequence, {expr2, rep_shift_expr2}};
      // ( ">>" expr2 | "<<" expr2)*
      rules_[rep_shift_expr2] = {RepeatStar, {sel_shift_expr2}};
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
      rules_[rep_sumsub_expr1] = {RepeatStar, {sel_sumsub_expr1}};
        // "+" expr1 | "-" expr1 
        rules_[sel_sumsub_expr1] = {RepeatStar, {seq_sum_expr1, seq_sub_expr1}};
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
      rules_[rep_muldivmod_term] = {RepeatStar, {sel_muldivmod_term}};
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
    //   | "~" term                   // bit negation 
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
                              seq_bitnot_term,
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
      rules_[seq_not_term] = {Sequence, {TokBitNot, term}};
      // "~" term 
      rules_[seq_bitnot_term] = {Sequence, {TokConNot, term}};
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
      rules_[rep_reffunc] = {RepeatStar, {sel_reffunc}};
        // "++" | "--" | "[" expr "]" | "." name | "->" name | "(" args ")"
        rules_[sel_reffunc] = {Select, {seq_post_inc, seq_post_dec, seq_array_reference, seq_dot_name, seq_arrow_name, seq_fncall}};
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
          rules_[seq_fncall] = {Sequence, {TokParenOpen, args, TokParenClose}};
     
    // args 
    //   : [expr ("," expr)*] 
    rules_[args] = {Options, {seq_args_expr, rep_args_expr}};
      // expr
      rules_[seq_args_expr] = {Sequence, {expr}};
      // ("," expr)*
      rules_[rep_args_expr] = {RepeatStar, {TokComma, expr}};
     
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

    rule_actions_[compilation_unit] = &SyntaxAnalyzer::CompilationUnit;

    rule_actions_[import_stmts] = &SyntaxAnalyzer::ImportStmts;
      rule_actions_[import_stmt] = &SyntaxAnalyzer::ImportStmt;
        rule_actions_[rep_dot_name] = &SyntaxAnalyzer::Act_rep_dot_name;

    rule_actions_[top_defs] = &SyntaxAnalyzer::TopDefs;

    rule_actions_[deffunc] = &SyntaxAnalyzer::DefFunc;

    rule_actions_[params] = &SyntaxAnalyzer::Params;
      rule_actions_[seq_param_void] = &SyntaxAnalyzer::Act_seq_param_void;
      rule_actions_[opt_vararg] = &SyntaxAnalyzer::Act_opt_vararg;

    rule_actions_[fixedparams] = &SyntaxAnalyzer::FixedParams;
      rule_actions_[rep_comma_param] = &SyntaxAnalyzer::Act_rep_comma_param;

    rule_actions_[block] = &SyntaxAnalyzer::Block;

    rule_actions_[defclass] = &SyntaxAnalyzer::DefClass;
      rule_actions_[seq_class_member_variable] = &SyntaxAnalyzer::Act_seq_class_member_variable;
      rule_actions_[seq_class_member_function] = &SyntaxAnalyzer::Act_seq_class_member_function;

    rule_actions_[param] = &SyntaxAnalyzer::Param;

    rule_actions_[defvars] = &SyntaxAnalyzer::DefVars;

    rule_actions_[defvar_list] = &SyntaxAnalyzer::DefVarList;

    rule_actions_[defconst] = &SyntaxAnalyzer::DefConst;

    rule_actions_[storage] = &SyntaxAnalyzer::Storage;

    rule_actions_[type] = &SyntaxAnalyzer::Type;

    rule_actions_[typedef_] = &SyntaxAnalyzer::TypeDef;

    rule_actions_[typeref] = &SyntaxAnalyzer::TypeRef;
      rule_actions_[seq_unassigned_array] = &SyntaxAnalyzer::Act_seq_unassigned_array; 
      rule_actions_[seq_assigned_array] = &SyntaxAnalyzer::Act_seq_assigned_array; 
      rule_actions_[seq_ptr] = &SyntaxAnalyzer::Act_seq_ptr; 
      rule_actions_[seq_func] = &SyntaxAnalyzer::Act_seq_func; 

      // parameter list
    rule_actions_[param_typerefs] = &SyntaxAnalyzer::ParamTypeRefs;
      rule_actions_[seq_param_type_void] = &SyntaxAnalyzer::Act_seq_param_type_void;
      rule_actions_[seq_param_type_list] = &SyntaxAnalyzer::Act_seq_param_type_list;
        rule_actions_[rep_param_comma_type] = &SyntaxAnalyzer::Act_rep_param_comma_type;
        rule_actions_[opt_vararg_type] = &SyntaxAnalyzer::Act_opt_vararg_type;
    
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
      rule_actions_[seq_bitnot_term] = &SyntaxAnalyzer::Act_seq_bitnot_term;
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
      rule_actions_[seq_fncall] = &SyntaxAnalyzer::Act_seq_fncall;

    rule_actions_[args] = &SyntaxAnalyzer::Args;
      rule_actions_[seq_args_expr] = &SyntaxAnalyzer::Act_seq_args_expr;
      rule_actions_[rep_args_expr] = &SyntaxAnalyzer::Act_rep_args_expr;

    rule_actions_[primary] = &SyntaxAnalyzer::Primary;
      rule_actions_[seq_po_expr_pc] = &SyntaxAnalyzer::Act_seq_po_expr_pc;

    rule_actions_[expr] = &SyntaxAnalyzer::Expr;
      rule_actions_[seq_assign_value] = &SyntaxAnalyzer::Act_seq_assign_value;
      rule_actions_[seq_opassign_value] = &SyntaxAnalyzer::Act_seq_opassign_value;

    rule_actions_[opassign_op] = &SyntaxAnalyzer::OpAssignOp;

    rule_actions_[expr10] = &SyntaxAnalyzer::Expr10;
      rule_actions_[opt_ternaryop] = &SyntaxAnalyzer::Act_opt_ternaryop;

    rule_actions_[expr9] = &SyntaxAnalyzer::Expr9;
      rule_actions_[rep_or_expr8] = &SyntaxAnalyzer::Act_rep_or_expr8;

    rule_actions_[expr8] = &SyntaxAnalyzer::Expr8;
      rule_actions_[rep_and_expr7] = &SyntaxAnalyzer::Act_rep_and_expr7;

    rule_actions_[expr7] = &SyntaxAnalyzer::Expr7;
      rule_actions_[seq_gr_expr6] = &SyntaxAnalyzer::Act_seq_gr_expr6;
      rule_actions_[seq_ls_expr6] = &SyntaxAnalyzer::Act_seq_ls_expr6;
      rule_actions_[seq_geq_expr6] = &SyntaxAnalyzer::Act_seq_geq_expr6;
      rule_actions_[seq_leq_expr6] = &SyntaxAnalyzer::Act_seq_leq_expr6;
      rule_actions_[seq_eq_expr6] = &SyntaxAnalyzer::Act_seq_eq_expr6;
      rule_actions_[seq_neq_expr6] = &SyntaxAnalyzer::Act_seq_neq_expr6;

    rule_actions_[expr6] = &SyntaxAnalyzer::Expr6;
      rule_actions_[rep_bitor_expr5] = &SyntaxAnalyzer::Act_rep_bitor_expr5;

    rule_actions_[expr5] = &SyntaxAnalyzer::Expr5;
      rule_actions_[rep_bitxor_expr4] = &SyntaxAnalyzer::Act_rep_bitxor_expr4;

    rule_actions_[expr4] = &SyntaxAnalyzer::Expr4;
      rule_actions_[rep_bitand_expr3] = &SyntaxAnalyzer::Act_rep_bitand_expr3;

    rule_actions_[expr3] = &SyntaxAnalyzer::Expr3;
      rule_actions_[seq_rshft_expr2] = &SyntaxAnalyzer::Act_seq_rshft_expr2;
      rule_actions_[seq_lshft_expr2] = &SyntaxAnalyzer::Act_seq_lshft_expr2;

    rule_actions_[expr2] = &SyntaxAnalyzer::Expr2;
      rule_actions_[seq_sum_expr1] = &SyntaxAnalyzer::Act_seq_sum_expr1;
      rule_actions_[seq_sub_expr1] = &SyntaxAnalyzer::Act_seq_sub_expr1;

    rule_actions_[expr1] = &SyntaxAnalyzer::Expr1;
      rule_actions_[seq_mul_term] = &SyntaxAnalyzer::Act_seq_mul_term;
      rule_actions_[seq_div_term] = &SyntaxAnalyzer::Act_seq_div_term;
      rule_actions_[seq_mod_term] = &SyntaxAnalyzer::Act_seq_mod_term;

    rule_actions_[stmts] = &SyntaxAnalyzer::Stmts;

    rule_actions_[stmt] = &SyntaxAnalyzer::Stmt;
      rule_actions_[labeled_stmt] = &SyntaxAnalyzer::LabeledStmt;
      rule_actions_[expr_stmt] = &SyntaxAnalyzer::ExprStmt;
      rule_actions_[if_stmt] = &SyntaxAnalyzer::IfStmt;
        rule_actions_[opt_else_stmt] = &SyntaxAnalyzer::Act_opt_else_stmt;
      rule_actions_[while_stmt] = &SyntaxAnalyzer::WhileStmt;
      rule_actions_[dowhile_stmt] = &SyntaxAnalyzer::DoWhileStmt;
      rule_actions_[for_stmt] = &SyntaxAnalyzer::ForStmt;
        rule_actions_[opt_for_init_expr] = &SyntaxAnalyzer::Act_opt_for_init_expr;
        rule_actions_[opt_for_cond_expr] = &SyntaxAnalyzer::Act_opt_for_cond_expr;
        rule_actions_[opt_for_inc_expr] = &SyntaxAnalyzer::Act_opt_for_inc_expr;
      rule_actions_[switch_stmt] = &SyntaxAnalyzer::SwitchStmt;
        rule_actions_[case_clauses] = &SyntaxAnalyzer::CaseClauses;
          rule_actions_[default_clause] = &SyntaxAnalyzer::DefaultClause;
          rule_actions_[case_clause] = &SyntaxAnalyzer::CaseClause;
            rule_actions_[case_list] = &SyntaxAnalyzer::CaseList;
            rule_actions_[case_body] = &SyntaxAnalyzer::CaseBody;

      rule_actions_[break_stmt] = &SyntaxAnalyzer::BreakStmt;

      rule_actions_[continue_stmt] = &SyntaxAnalyzer::ContinueStmt;

      rule_actions_[goto_stmt] = &SyntaxAnalyzer::GotoStmt;

      rule_actions_[return_stmt] = &SyntaxAnalyzer::ReturnStmt;
        rule_actions_[seq_return] = &SyntaxAnalyzer::Act_seq_return;
        rule_actions_[seq_return_expr] = &SyntaxAnalyzer::Act_seq_return_expr;

    rule_actions_[TokIntegerLiteral] = &SyntaxAnalyzer::ActTokIntegerLiteral;
    rule_actions_[TokCharactorLiteral] = &SyntaxAnalyzer::ActTokCharacterLiteral;
    rule_actions_[TokStringLiteral] = &SyntaxAnalyzer::ActTokStringLiteral;
    rule_actions_[TokIdentifier] = &SyntaxAnalyzer::ActTokIdentifier;
  }
}
