#include "parser.h"

namespace Parser {
  Rule SyntaxAnalyzer::RuleSetter(RuleName action, int lookahead,
        RuleName sub01, RuleName sub02, RuleName sub03, 
        RuleName sub04, RuleName sub05, RuleName sub06, 
        RuleName sub07, RuleName sub08, RuleName sub09, 
        RuleName sub10, RuleName sub11, RuleName sub12, 
        RuleName sub13, RuleName sub14, RuleName sub15) {
    Rule new_rule;
    new_rule.action_ = action;
    new_rule.lookahead_ = lookahead;
    new_rule.sub_rules_[0] = sub01;
    new_rule.sub_rules_[1] = sub02;
    new_rule.sub_rules_[2] = sub03;
    new_rule.sub_rules_[3] = sub04;
    new_rule.sub_rules_[4] = sub05;
    new_rule.sub_rules_[5] = sub06;
    new_rule.sub_rules_[6] = sub07;
    new_rule.sub_rules_[7] = sub08;
    new_rule.sub_rules_[8] = sub09;
    new_rule.sub_rules_[9] = sub10;
    new_rule.sub_rules_[10] = sub11;
    new_rule.sub_rules_[11] = sub12;
    new_rule.sub_rules_[12] = sub13;
    new_rule.sub_rules_[13] = sub14;
    new_rule.sub_rules_[14] = sub15;
    return new_rule;
  }

  void SyntaxAnalyzer::InitBasicRule() {
    // Initialize token rules as Terminal.
    for (int i = 0; i < TokEnd; i++) {
      rules_[i] = RuleSetter(Terminal, 0, (RuleName)i);
    }

    // compilation_unit 
    //    : import_stmts top_defs <EOF> 
    rules_[compilation_unit] = RuleSetter(Sequence, 0, import_stmts, top_defs, TokEof);

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
    rules_[typeref_base] = RuleSetter(Select, 0, 
              seq_void, 
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
              seq_class_identifier);
      rules_[seq_void] = RuleSetter(Sequence, 0, TokVoid);
      rules_[seq_char] = RuleSetter(Sequence, 0, TokChar);
      rules_[seq_short] = RuleSetter(Sequence, 0, TokShort);
      rules_[seq_int] = RuleSetter(Sequence, 0, TokInt);
      rules_[seq_long] = RuleSetter(Sequence, 0, TokLong);
      rules_[seq_unsigned_char] = RuleSetter(Sequence, 0, TokUnsigned, TokChar);
      rules_[seq_unsigned_short] = RuleSetter(Sequence, 0, TokUnsigned, TokShort);
      rules_[seq_unsigned_int] = RuleSetter(Sequence, 0, TokUnsigned, TokInt);
      rules_[seq_unsigned_long] = RuleSetter(Sequence, 0, TokUnsigned, TokLong);
      rules_[seq_float] = RuleSetter(Sequence, 0, TokFloat);
      rules_[seq_double] = RuleSetter(Sequence, 0, TokDouble);
      rules_[seq_class_identifier] = RuleSetter(Sequence, 0, TokClass, TokIdentifier);
    
    // import_stmts  
    //   : (import_stmt)* 
    rules_[import_stmts] = RuleSetter(RepeatStar, 0, import_stmt);
      // import_stmt 
      //   : <IMPORT> name ("." name)* ";" 
      rules_[import_stmt] = RuleSetter(Sequence, 0, TokImport, name, rep_dot_name, TokSemiColon);
        // ("." name)*
        rules_[rep_dot_name] = RuleSetter(RepeatStar, 0, TokDot, name);

    // name 
    //   : <IDENTIFIER> 
    rules_[name] = RuleSetter(Sequence, 0, TokIdentifier);

    // top_defs // top definitions 
    //   : ( deffunc 
    //     | defvars 
    //     | defconst 
    //     | defclass 
    //     | typedef_ )* 
    rules_[top_defs] = RuleSetter(RepeatStar, 0, sel_fun_var_const_class_typedef);
      rules_[sel_fun_var_const_class_typedef] = RuleSetter(Select, 0, deffunc, defvars, defconst, defclass, typedef_ );

    // defvars // variable definition. ex) int a = 0, b=19; 
    //   : LOOKAHEAD(storage type name) ["=" expr] [("," name ["=" expr])*] ";" 
    rules_[defvars] = RuleSetter(Sequence, 3, storage, type, name, opt_var_initialize, opt_rep_var_initialize, TokSemiColon );
      rules_[opt_var_initialize] = RuleSetter(Options, 0, TokAssign, expr); // ["=" expr]
      // [("," name ["=" expr])*]
      rules_[opt_rep_var_initialize] = RuleSetter(Options, 0, rep_var_initialize); 
        // ("," name ["=" expr])*
        rules_[rep_var_initialize] = RuleSetter(RepeatStar, 0, TokComma, name, opt_var_initialize);
        
    // defconst
    //   : <CONST> type name "=" expr ";"
    rules_[defconst] = RuleSetter(Sequence, 0, TokConst, type, name, TokAssign, expr, TokSemiColon);

    // storage 
    //   : [<STATIC>] 
    rules_[storage] = RuleSetter(Options, 0, TokStatic);
    
    // type 
    //   : typeref 
    rules_[type] = RuleSetter(Sequence, 0, typeref);

    // deffunc // function definition 
    //   : LOOKAHEAD(storage typeref name "(") params ")" block 
    rules_[deffunc] = RuleSetter(Sequence, 4, storage, typeref, name, TokParenOpen, params, TokParenClose, block );

    // params // parameter definition 
    //   : <VOID> 
    //   | fixedparams ["," "..."] 
    rules_[params] = RuleSetter(Select, 0, seq_param_void, seq_param_list);
      // <VOID>
      rules_[seq_param_void] = RuleSetter(Sequence, 0, TokVoid);
      // fixedparams ["," "..."] 
      rules_[seq_param_list] = RuleSetter(Sequence, 0, fixedparams, opt_vararg);
        // ["," "..."] 
        rules_[opt_vararg] = RuleSetter(Options, 0, TokComma, TokDotDotDot);

    // fixedparams // fixed parameter definition 
    //   : param ("," param)* 
    rules_[fixedparams] = RuleSetter(Sequence, 0, param, rep_comma_param);
      // ("," param)* 
      rules_[rep_comma_param] = RuleSetter(RepeatStar, 0, TokComma, param);
      
    // param 
    //   : type name 
    rules_[param] = RuleSetter(Sequence, 0, type, name);

    // defvar_list 
    //   : (defvars)*
    rules_[defvar_list] = RuleSetter(RepeatStar, 0, defvars);

    // block 
    //   : "{" defvar_list stmts "}" 
    rules_[block] = RuleSetter(Sequence, 0, TokBraceOpen, defvar_list, stmts, TokBraceClose);

    // defclass // class definition 
    //   : <CLASS> name class_member_list ";" 
    rules_[defclass] = RuleSetter(Sequence, 0, TokClass, name, class_member_list, TokSemiColon);
      // class_member_list //  
      //   : "{" (class_member ";")* "}" 
      rules_[class_member_list] = RuleSetter(Sequence, 0, TokBraceOpen, rep_class_member, TokBraceClose);
        // (class_member)*
        rules_[rep_class_member] = RuleSetter(RepeatStar, 0, class_member);
          // class_member // class member definition 
          //   : defvars 
          //   | deffunc
          rules_[class_member] = RuleSetter(Select, 0, seq_class_member_variable, seq_class_member_function);
            // defvars 
            rules_[seq_class_member_variable] = RuleSetter(Sequence, 0, defvars);
            // deffunc
            rules_[seq_class_member_function] = RuleSetter(Sequence, 0, deffunc);

    // typedef // ex) typedef int i32; 
    //   : <TYPEDEF> typeref <IDENTIFIER> ";" 
    rules_[typedef_] = RuleSetter(Sequence, 0, TokTypeDef, typeref, TokIdentifier, TokSemiColon);

    // typeref 
    //   : typeref_base  ( "[""]"                    // unassigned array
    //                   | "["<INTEGER>"]"           // assigned array
    //                   | "*"                       // pointer 
    //                   | "(" param_typerefs ")")*  // function type
    rules_[typeref] = RuleSetter(Sequence, 0, typeref_base, rep_sel_arry_ptr_fnptr);
      //   ( "[""]"                    // unassigned array
      //   | "["<INTEGER>"]"           // assigned array
      //   | "*"                       // pointer 
      //   | "(" param_typerefs ")")*  // function type 
      rules_[rep_sel_arry_ptr_fnptr] = RuleSetter(RepeatStar, 0, sel_arry_ptr_fnptr);
        //   "[""]"                    // unassigned array
        //   | "["<INTEGER>"]"           // assigned array
        //   | "*"                       // pointer 
        //   | "(" param_typerefs ")"  // function type
        rules_[sel_arry_ptr_fnptr] = RuleSetter(Select, 0, seq_unassigned_array, seq_assigned_array, seq_ptr, seq_func);
          // "[""]
          rules_[seq_unassigned_array] = RuleSetter(Sequence, 0, TokBracketOpen, TokBracketClose);
          // "["<INTEGER>"]"
          rules_[seq_assigned_array] = RuleSetter(Sequence, 0, TokBracketOpen, TokIntegerLiteral, TokBracketClose);
          // "*"
          rules_[seq_ptr] = RuleSetter(Sequence, 0, TokMul);
          // "(" param_typerefs ")"
          rules_[seq_func] = RuleSetter(Sequence, 0, TokParenOpen, param_typerefs, TokParenClose);

    // param_typerefs // function pointer param type definition 
    //   : <VOID> 
    //   | typeref ("," typeref)* ["," "..."] 
    rules_[param_typerefs] = RuleSetter(Select, 0, seq_param_type_void, seq_param_type_list);
      // <VOID>
      rules_[seq_param_type_void] = RuleSetter(Sequence, 0, TokVoid);
      // typeref ("," typeref)* ["," "..."] 
      rules_[seq_param_type_list] = RuleSetter(Sequence, 0, typeref, rep_param_comma_type, opt_vararg_type);
        // ("," typeref)*
        rules_[rep_param_comma_type] = RuleSetter(RepeatStar, 0, TokComma, typeref);
        // ["," "..."] 
        rules_[opt_vararg_type] = RuleSetter(RepeatStar, 0, TokComma, TokDotDotDot);

    // stmts 
    //   : (stmt)* 
    rules_[stmts] = RuleSetter(RepeatStar, 0, stmt);

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
    rules_[stmt] = RuleSetter(Select, 0, TokSemiColon, 
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
                                      return_stmt);

    // labeled_stmt
    //   : LOOKAHEAD(<IDENTIFIER> ":") stmt
    rules_[labeled_stmt] = RuleSetter(Sequence, 2, TokIdentifier, TokColon, stmt);

    // expr_stmt
    //  : expr ";"
    rules_[expr_stmt] = RuleSetter(Sequence, 0, expr, TokSemiColon);
     
    // if_stmt  
    //   : <IF> "(" expr ")" stmt [<ELSE> stmt] 
    rules_[if_stmt] = RuleSetter(Sequence, 0, TokIf, TokParenOpen, expr, TokParenClose, stmt, opt_else_stmt);
      rules_[opt_else_stmt] = RuleSetter(Options, 0, TokElse, stmt); // [<ELSE> stmt] 
     
    // while_stmt 
    //   : <WHILE> "(" expr ")" stmt 
    rules_[while_stmt] = RuleSetter(Sequence, 0, TokWhile, TokParenOpen, expr, TokParenClose, stmt);
     
    // dowhile_stmt
    //   : <DO> stmt <WHILE> "(" expr ")" ";"
    rules_[dowhile_stmt] = RuleSetter(Sequence, 0, TokDo, stmt, TokWhile, TokParenOpen, expr, TokParenClose, TokSemiColon);

    // for_stmt 
    //   : <FOR> "(" [expr] ";" [expr] ";" [expr] ")" stmt 
    rules_[for_stmt] = RuleSetter(Sequence, 0, TokFor, TokParenOpen, opt_for_init_expr, TokSemiColon, opt_for_cond_expr, TokSemiColon, opt_for_inc_expr, TokParenClose, stmt );
      // [expr]
      rules_[opt_for_init_expr] = RuleSetter(Options, 0, expr);
      // [expr]
      rules_[opt_for_cond_expr] = RuleSetter(Options, 0, expr);
      // [expr]
      rules_[opt_for_inc_expr] = RuleSetter(Options, 0, expr);

    // switch_stmt
    //   : <SWITCH> "(" expr ")" "{" case_clauses "}"
    rules_[switch_stmt] = RuleSetter(Sequence, 0, TokSwitch, TokParenOpen, expr, TokParenClose, TokBraceOpen, case_clauses, TokBraceClose);

    // case_clauses
    //   : (case_clause)* [default_clause]
    rules_[case_clauses] = RuleSetter(Sequence, 0, rep_case_clause, opt_default_clause);
      // (case_clause)*
      rules_[rep_case_clause] = RuleSetter(RepeatStar, 0, case_clause);
      // [default_clause]
      rules_[opt_default_clause] = RuleSetter(Options, 0, default_clause);
    
    // case_clause
    //   : case_list case_body
    rules_[case_clause] = RuleSetter(Sequence, 0, case_list, case_body);
     
    // case_list
    //   : (<CASE> primary ":")+
    rules_[case_list] = RuleSetter(RepeatDagger, 0, TokCase, primary, TokColon);
    
    // default_clause
    //   : <DEFAULT> ":" case_body
    rules_[default_clause] = RuleSetter(Sequence, 0, TokDefault, TokColon, case_body);
   
    // case_body
    //   : stmt
    rules_[case_body] = RuleSetter(Sequence, 0, stmt);

    // break_stmt 
    //   : <BREAK> ";" 
    rules_[break_stmt] = RuleSetter(Sequence, 0, TokBreak, TokSemiColon);

    // continue_stmt
    //   : <CONTINUE> ";"
    rules_[continue_stmt] = RuleSetter(Sequence, 0, TokContinue, TokSemiColon);
     
    // goto_stmt
    //   : <GOTO> <IDENTIFIER> ";"
    rules_[goto_stmt] = RuleSetter(Sequence, 0, TokGoto, TokIdentifier, TokSemiColon);
     
    // return_stmt 
    //   : LOOKAHEAD(<RETURN> ";")
    //   | <RETURN> expr ";" 
    rules_[return_stmt] = RuleSetter(Select, 0, seq_return, seq_return_expr);
      // <RETURN> ";"
      rules_[seq_return] = RuleSetter(Sequence, 2, TokReturn, TokSemiColon);
      // <RETURN> expr ";"
      rules_[seq_return_expr] = RuleSetter(Sequence, 0, TokReturn, expr, TokSemiColon);
     
    // expr 
    //   : LOOKAHEAD(term "=") expr 
    //   | LOOKAHEAD(term opassign_op) expr 
    //   | expr10 
    rules_[expr] = RuleSetter(Select, 0, seq_assign_value, seq_opassign_value, expr10);
      // term "=" expr 
      rules_[seq_assign_value] = RuleSetter(Sequence, 2, term, TokAssign, expr);
      // term opassign_op expr 
      rules_[seq_opassign_value] = RuleSetter(Sequence, 2, term, opassign_op, expr);
     
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
    rules_[opassign_op] = RuleSetter(Select, 0, TokComAdd, 
                                    TokComSub,
                                    TokComMul,
                                    TokComDiv,
                                    TokComMod,
                                    TokComBitAnd,
                                    TokComBitOr,
                                    TokComBitXor,
                                    TokComBitShiftL,
                                    TokComBitShiftR);
     
    // expr10 
    //   : expr9 ["?" expr ":" expr10] 
    rules_[expr10] = RuleSetter(Sequence, 0, expr9, opt_ternaryop);
      // ["?" expr ":" expr10] 
      rules_[opt_ternaryop] = RuleSetter(Options, 0, TokQuestion, expr, TokColon, expr10);
     
    // expr9 
    //   : expr8 ("||" expr8)* 
    rules_[expr9] = RuleSetter(Sequence, 0, expr8, rep_or_expr8);
      //("||" expr8)* 
      rules_[rep_or_expr8] = RuleSetter(RepeatStar, 0, TokConOr, expr8);
     
    // expr8 
    //   : expr7 ("&&" expr7)* 
    rules_[expr8] = RuleSetter(Sequence, 0, expr7, rep_and_expr7);
      // ("&&" expr7)* 
      rules_[rep_and_expr7] = RuleSetter(RepeatStar, 0, TokConAnd, expr7);
     
    // expr7  
    //   : expr6 ( ">" expr6  
    //           | "<" expr6 
    //           | ">=" expr6 
    //           | "<=" expr6 
    //           | "==" expr6 
    //           | "!=" expr6 )* 
    //
    rules_[expr7] = RuleSetter(Sequence, 0, expr6, rep_op_expr6);
      //(sel_op_expr6)*
      rules_[rep_op_expr6] = RuleSetter(RepeatStar, 0, sel_op_expr6);
        // ">" expr6  
        //| "<" expr6 
        //| ">=" expr6 
        //| "<=" expr6 
        //| "==" expr6 
        //| "!=" expr6
        rules_[sel_op_expr6] = RuleSetter(Select, 0, seq_gr_expr6, 
                                          seq_ls_expr6,
                                          seq_geq_expr6,
                                          seq_leq_expr6,
                                          seq_eq_expr6,
                                          seq_neq_expr6 );
          // ">" expr6  
          rules_[seq_gr_expr6] = RuleSetter(Sequence, 0, TokGreatorThan, expr6);
          // "<" expr6 
          rules_[seq_ls_expr6] = RuleSetter(Sequence, 0, TokLessThan, expr6);
          // ">=" expr6 
          rules_[seq_geq_expr6] = RuleSetter(Sequence, 0, TokGreatorThenEqual, expr6);
          // "<=" expr6 
          rules_[seq_leq_expr6] = RuleSetter(Sequence, 0, TokLessThanEqual, expr6);
          // "==" expr6 
          rules_[seq_eq_expr6] = RuleSetter(Sequence, 0, TokEqual, expr6);
          // "!=" expr6 
          rules_[seq_neq_expr6] = RuleSetter(Sequence, 0, TokNotEqual, expr6);
     
    // expr6 
    //   : expr5 ("|" expr5)* 
    rules_[expr6] = RuleSetter(Sequence, 0, expr5, rep_bitor_expr5);
      // ("|" expr5)* 
      rules_[rep_bitor_expr5] = RuleSetter(RepeatStar, 0, TokBitOr, expr5);

    // expr5 
    //   : expr4 ("^" expr4)* 
    rules_[expr5] = RuleSetter(Sequence, 0, expr4, rep_bitxor_expr4);
      // ("^" expr4)* 
      rules_[rep_bitxor_expr4] = RuleSetter(RepeatStar, 0, TokBitXor, expr4);
     
    // expr4 
    //   : expr3 ("&" expr3)* 
    rules_[expr4] = RuleSetter(Sequence, 0, expr3, rep_bitand_expr3);
      // ("&" expr3)* 
      rules_[rep_bitand_expr3] = RuleSetter(RepeatStar, 0, TokBitAnd, expr3);

    // expr3 
    //   : expr2 ( ">>" expr2 | "<<" expr2)* 
    rules_[expr3] = RuleSetter(Sequence, 0, expr2, rep_shift_expr2);
      // ( ">>" expr2 | "<<" expr2)*
      rules_[rep_shift_expr2] = RuleSetter(RepeatStar, 0, sel_shift_expr2);
        // ">>" expr2 | "<<" expr2
        rules_[sel_shift_expr2] = RuleSetter(Select, 0, seq_rshft_expr2, seq_lshft_expr2);
          // ">>" expr2
          rules_[seq_rshft_expr2] = RuleSetter(Sequence, 0, TokBitShiftR, expr2);
          // "<<" expr2
          rules_[seq_lshft_expr2] = RuleSetter(Sequence, 0, TokBitShiftL, expr2);
     
    // expr2 
    //   : expr1 ( "+" expr1 | "-" expr1)* 
    rules_[expr2] = RuleSetter(Sequence, 0, expr1, rep_sumsub_expr1);
      // ( "+" expr1 | "-" expr1)* 
      rules_[rep_sumsub_expr1] = RuleSetter(RepeatStar, 0, sel_sumsub_expr1);
        // "+" expr1 | "-" expr1 
        rules_[sel_sumsub_expr1] = RuleSetter(Select, 0, seq_sum_expr1, seq_sub_expr1);
          // "+" expr1
          rules_[seq_sum_expr1] = RuleSetter(Sequence, 0, TokAdd, expr1);
          // "-" expr1
          rules_[seq_sub_expr1] = RuleSetter(Sequence, 0, TokSub, expr1);
     
    // expr1 
    //   : term (  "*" term  
    //           | "/" term 
    //           | "%" term 
    //           )* 
    rules_[expr1] = RuleSetter(Sequence, 0, term, rep_muldivmod_term);
      //("*" term 
      //| "/" term 
      //| "%" term)* 
      rules_[rep_muldivmod_term] = RuleSetter(RepeatStar, 0, sel_muldivmod_term);
        //"*" term  
        //| "/" term 
        //| "%" term
        rules_[sel_muldivmod_term] = RuleSetter(Select, 0, seq_mul_term, seq_div_term, seq_mod_term);
          // "*" term  
          rules_[seq_mul_term] = RuleSetter(Sequence, 0, TokMul, term);
          // "/" term 
          rules_[seq_div_term] = RuleSetter(Sequence, 0, TokDiv, term);
          // "%" term
          rules_[seq_mod_term] = RuleSetter(Sequence, 0, TokMod, term);
     
    // term 
    //   : "(" type ")" term          // type casting 
    //   | unary 
    rules_[term] = RuleSetter(Select, 0, seq_type_term, unary);
      //"(" type ")" term
      rules_[seq_type_term] = RuleSetter(Sequence, 0, TokParenOpen, type, TokParenClose, term);
     
    // unary 
    //   : "++" unary                 // pre ++ 
    //   | "--" unary                 // pre -- 
    //   | "+" term                   // unary +, positive 
    //   | "-" term                   // unary -, negative 
    //   | "!" term                   // Logical negation 
    //   | "~" term                   // bit negation 
    //   | "*" term                   // Pointer reference 
    //   | "&" term                   // adress operator 
    //   | LOOKAHEAD(<SIZEOF> "(") type ")"      // sizeof(type) 
    //   | <SIZEOF> unary             // sizeof unary 
    //   | postfix                    // postfix  
    rules_[unary] = RuleSetter(Select, 0, seq_preinc_unary, 
                              seq_predec_unary,
                              seq_pos_term,
                              seq_neg_term,
                              seq_not_term,
                              seq_bitnot_term,
                              seq_ptr_term,
                              seq_adr_term,
                              seq_sizeof_type,
                              seq_sizeof_unary,
                              postfix);
      // "++" unary
      rules_[seq_preinc_unary] = RuleSetter(Sequence, 0, TokUnaryInc, unary);
      // "--" unary
      rules_[seq_predec_unary] = RuleSetter(Sequence, 0, TokUnaryDec, unary);
      // "+" term 
      rules_[seq_pos_term] = RuleSetter(Sequence, 0, TokAdd, term);
      // "-" term
      rules_[seq_neg_term] = RuleSetter(Sequence, 0, TokSub, term);
      // "!" term 
      rules_[seq_not_term] = RuleSetter(Sequence, 0, TokBitNot, term);
      // "~" term 
      rules_[seq_bitnot_term] = RuleSetter(Sequence, 0, TokConNot, term);
      // "*" term
      rules_[seq_ptr_term] = RuleSetter(Sequence, 0, TokMul, term);
      // "&" term 
      rules_[seq_adr_term] = RuleSetter(Sequence, 0, TokBitAnd, term);
      // <SIZEOF> "(" type ")"
      rules_[seq_sizeof_type] = RuleSetter(Sequence, 2, TokSizeOf, TokParenOpen, type, TokParenClose);
      // <SIZEOF> unary
      rules_[seq_sizeof_unary] = RuleSetter(Sequence, 0, TokSizeOf, unary);
     
    // postfix 
    //   : primary ("++"               // post ++ 
    //             |"--"               // post -- 
    //             |"[" expr "]"       // array reference 
    //             |"." name           // class member reference 
    //             |"->" name          // class member pointer reference 
    //             |"(" args ")"       // function call 
    //             )* 
    rules_[postfix] = RuleSetter(Sequence, 0, primary, rep_reffunc);
      //("++" | "--" | "[" expr "]" | "." name | "->" name | "(" args ")")*
      rules_[rep_reffunc] = RuleSetter(RepeatStar, 0, sel_reffunc);
        // "++" | "--" | "[" expr "]" | "." name | "->" name | "(" args ")"
        rules_[sel_reffunc] = RuleSetter(Select, 0, seq_post_inc, seq_post_dec, seq_array_reference, seq_dot_name, seq_arrow_name, seq_fncall);
          // "++"
          rules_[seq_post_inc] = RuleSetter(Sequence, 0, TokUnaryInc);
          // "--"
          rules_[seq_post_dec] = RuleSetter(Sequence, 0, TokUnaryDec);
          //"[" expr "]"
          rules_[seq_array_reference] = RuleSetter(Sequence, 0, TokBracketOpen, expr, TokBracketClose);
          //"." name
          rules_[seq_dot_name] = RuleSetter(Sequence, 0, TokDot, name);
          //"->" name
          rules_[seq_arrow_name] = RuleSetter(Sequence, 0, TokRightArrow, name);
          //"(" args ")"
          rules_[seq_fncall] = RuleSetter(Sequence, 0, TokParenOpen, args, TokParenClose);
     
    // args 
    //   : [expr ("," expr)*] 
    rules_[args] = RuleSetter(Options, 0, seq_args_expr, rep_args_expr);
      // expr
      rules_[seq_args_expr] = RuleSetter(Sequence, 0, expr);
      // ("," expr)*
      rules_[rep_args_expr] = RuleSetter(RepeatStar, 0, TokComma, expr);
     
    // primary 
    //   : <INTEGER> 
    //   | <CHARACTER> 
    //   | <STRING> 
    //   | <IDENTIFIER> 
    //   | "(" expr ")" 
    rules_[primary] = RuleSetter(Select, 0, TokIntegerLiteral, TokCharactorLiteral, TokStringLiteral, TokIdentifier, seq_po_expr_pc);
      // "(" expr ")" 
      rules_[seq_po_expr_pc] = RuleSetter(Sequence, 0, TokParenOpen, expr, TokParenClose);

  }

  void SyntaxAnalyzer::InitSuccessAction() {
    /*for (int i = 0; i < MAX_RULES; i++) {
      rule_actions_[i] = &SyntaxAnalyzer::ReturnTrue;
    }*/
    for (int i = 0; i < MAX_RULES; i++) {
      rule_actions_[i] = nullptr;
    }


    rule_actions_[compilation_unit] = &SyntaxAnalyzer::CompilationUnit;

    rule_actions_[import_stmts] = &SyntaxAnalyzer::ImportStmts;
      rule_actions_[import_stmt] = &SyntaxAnalyzer::ImportStmt;
        rule_actions_[rep_dot_name] = &SyntaxAnalyzer::Act_rep_dot_name;

    rule_actions_[top_defs] = &SyntaxAnalyzer::TopDefs;

    rule_actions_[deffunc] = &SyntaxAnalyzer::DefFunc;

    rule_actions_[params] = &SyntaxAnalyzer::Params;
      rule_actions_[seq_param_void] = &SyntaxAnalyzer::Act_seq_param_void;
      rule_actions_[seq_param_list] = &SyntaxAnalyzer::Act_seq_param_list;
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
    rule_actions_[TokBraceOpen] = &SyntaxAnalyzer::ActTokBraceOpen;
  }

  void SyntaxAnalyzer::InitFailAction() {
    /*for (int i = 0; i < MAX_RULES; i++) {
      rule_actions_[i] = nullptr;
    }*/
  }
}
