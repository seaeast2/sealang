#include <assert.h>
#include "parser.h"

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
    rules_[compilation_unit] = {Sequence, {import_stmts, top_defs, TokEof}};

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
    rules_[typeref_base] = {Select, { TokVoid, 
                                      TokChar,
                                      TokShort, 
                                      TokInt, 
                                      TokLong, 
                                      seq_unsigned_char,
                                      seq_unsigned_short,
                                      seq_unsigned_int,
                                      seq_unsigned_long,
                                      TokFloat,
                                      TokDouble,
                                      seq_class_identifier,
                                      }};
      rules_[seq_unsigned_char] = {Sequence, {TokUnsigned, TokChar}};
      rules_[seq_unsigned_short] = {Sequence, {TokUnsigned, TokShort}};
      rules_[seq_unsigned_int] = {Sequence, {TokUnsigned, TokInt}};
      rules_[seq_unsigned_long] = {Sequence, {TokUnsigned, TokLong}};
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
    rules_[name] = {Nonterminal, {TokIdentifier}};

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
      rules_[seq_fixparms_dot_dotdotdot] = {Sequence, {fixedparams, opt_dot_dotdotdot}};
        // ["," "..."] 
        rules_[opt_dot_dotdotdot] = {Options, {TokDot, TokDotDotDot}};

    // fixedparams // fixed parameter definition 
    //   : param ("," param)* 
    rules_[fixedparams] = {Sequence, {param, rep_dot_param}};
      // ("," param)* 
      rules_[rep_dot_param] = {Repeat, {TokDot, param}};
      
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
        rules_[sel_arry_ptr_fnptr] = {Select, {seq_brckopn_brckcls, seq_brckopn_int_brckcls, TokMul, seq_prenopn_paramty_prencls}};
          // "[""]
          rules_[seq_brckopn_brckcls] = {Sequence, {TokBracketOpen, TokBracketClose}};
          // "["<INTEGER>"]"
          rules_[seq_brckopn_int_brckcls] = {Sequence, {TokBracketOpen, TokIntegerLiteral, TokBracketClose}};
          // "(" param_typerefs ")"
          rules_[seq_prenopn_paramty_prencls] = {Sequence, {TokParenOpen, param_typerefs, TokParenClose}};

    // param_typerefs // function pointer param type definition 
    //   : <VOID> 
    //   | type ("," type)* ["," "..."] 
    rules_[param_typerefs] = {Select, {TokVoid, seq_type_rep_type_dot}};
      // type ("," type)* ["," "..."] 
      rules_[seq_type_rep_type_dot] = {Sequence, {type, rep_dot_type, opt_dot_dotdotdot}};
        // ("," type)*
        rules_[rep_dot_type] = {Repeat, {TokDot, type}};

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
      rules_[rep_shift_expr2] = {Repeat, {seq_rshft_expr2, seq_lshft_expr2}};
        // ">>" expr2
        rules_[seq_rshft_expr2] = {Sequence, {TokBitShiftR, expr2}};
        // "<<" expr2
        rules_[seq_lshft_expr2] = {Sequence, {TokBitShiftL, expr2}};
     
    // expr2 
    //   : expr1 ( "+" expr1 | "-" expr1)* 
     
    // expr1 
    //   : term (  "*" term  
    //           | "/" term 
    //           | "%" term 
    //           )* 
     
    // term 
    //   : "(" type ")" term          // type casting 
    //   | unary 
     
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
     
    // postfix 
    //   : primary ("++"               // post ++ 
    //             |"--")              // post -- 
    //   | primary ("[" expr "]"       // array reference 
    //             |"." name           // class member reference 
    //             |"->" name          // class member pointer reference 
    //             |"(" args ")"       // function call 
    //             )* 
     
    // args 
    //   : [expr ("," expr)*] 
     
    // primary 
    //   : <INTEGER> 
    //   |<CHARACTER> 
    //   |<STRING> 
    //   |<IDENTIFIER> 
    //   |"(" expr ")" 
  }
  
  void InitRuleAction() {
    //actions_[0] = &SyntaxAnalyzer::test;
    //(this->*actions_[0])();
  }

  eResult SyntaxAnalyzer::TraverseRule(int entry) {
    Rule rule = rules_[entry];
    eResult res;
    while(true) {
      switch(rule.action_) {
        case Repeat:
          {
            bool found_matching = false;
            while(true) {
              int matching_count = 0;
              int i = 0;
              for (; rule.sub_rules_[i] > -1; i++) {
                res = TraverseRule(rule.sub_rules_[i]);
                if (res == True) {
                  matching_count++;
                }
                else if (res == False) {
                  if (rules_[rule.sub_rules_[i]].action_ == Options) {
                    matching_count++;
                  }
                }
                else
                  return Error;
              }
              if (matching_count != i)
                break;
              found_matching = true;
            };

            if (found_matching)
              return True;
            return False;
          }
          break;

        case Select:
          {
            for (int i = 0; rule.sub_rules_[i] > -1; i++) {
              res = TraverseRule(rule.sub_rules_[i]);
              if (res == True)
                return True; // found matching
              else if (res == False) {
                if (rules_[rule.sub_rules_[i]].action_ == Options) {
                  return True;
                }
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
            for (; rule.sub_rules_[i] > -1; i++) {
              res = TraverseRule(rule.sub_rules_[i]);
              if (res == True) {
                matching_count++;
              }
              else if (res == False) { // False
                if (rules_[rule.sub_rules_[i]].action_ == Options) {
                  matching_count++;
                }
              }
              else
                return Error;
            }
            if (matching_count == i) {
              // TODO : run matching action here
              // Action[entry].Run();
              return True;
            }
            return False; // unmatching
          }
          break;

        case Options:
          {
            int matching_count = 0;
            int i = 0;
            for (; rule.sub_rules_[i] > -1; i++) {
              res = TraverseRule(rule.sub_rules_[i]);
              if (res == True) {
                matching_count++;
              }
              else
                return Error;
            }
            if (matching_count == i)
              return True;
            return False; // unmatching
          }
          break;
        case Terminal:
          {
            if(tokenizer_->isToken(0, Lexer::TokenType(rule.sub_rules_[0])))
              return True;
            return False;
          }
          break;
        case Nonterminal:
          {
            return TraverseRule(rule.sub_rules_[0]);
          }
          break;
        default:
          assert("Error wrong rule action.");
          break;
      }
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

  eResult SyntaxAnalyzer::CompilationUnit() {
    return True;
  }

  eResult SyntaxAnalyzer::ImportStmts() {
    return True;
  }

  eResult SyntaxAnalyzer::TopDefs() {
    return True;
  }

  eResult SyntaxAnalyzer::Name() {
    return False;
  }

  eResult SyntaxAnalyzer::Storage() {
    return False;
  }

  eResult SyntaxAnalyzer::Type() {
    return TypeRef();
  }

  eResult SyntaxAnalyzer::TypeRef() {
    return False; // can't find matching
  }

  eResult SyntaxAnalyzer::TypeRefBase() {
    return False;
  }

  eResult SyntaxAnalyzer::TypeDef() {
    return True;
  }

  eResult SyntaxAnalyzer::ParamTypeRefs() {
    return True;
  }

  eResult SyntaxAnalyzer::Block() {
    return True;
  }

  eResult SyntaxAnalyzer::Param() {
    return False;
  }

  eResult SyntaxAnalyzer::FixedParams() {
    return False;
  }

  eResult SyntaxAnalyzer::Params() {
    return True;
  }

  eResult SyntaxAnalyzer::DefFunc() {
    return True;
  }

  eResult SyntaxAnalyzer::DefVars() {
    return True;
  }

  eResult SyntaxAnalyzer::DefVarList() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr() {
    return True;
  }

  eResult SyntaxAnalyzer::Term() {
    return True;
  }

  eResult SyntaxAnalyzer::Unary() {
    return False;
  }

  eResult SyntaxAnalyzer::Postfix() {
    return False;
  }

  eResult SyntaxAnalyzer::Primary() {
    return False;
  }

  eResult SyntaxAnalyzer::Args() {
    return True;
  }

  eResult SyntaxAnalyzer::OpAssignOp() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr10() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr9() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr8() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr7() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr6() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr5() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr4() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr3() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr2() {
    return True;
  }

  eResult SyntaxAnalyzer::Expr1() {
    return True;
  }

  eResult SyntaxAnalyzer::Stmts() {
    return True;
  }

  eResult SyntaxAnalyzer::Stmt() {
    return True;
  }

  eResult SyntaxAnalyzer::LabeledStmt() {
    return True;
  }

  eResult SyntaxAnalyzer::IfStmt() {
    return True;
  }

  eResult SyntaxAnalyzer::WhileStmt() {
    return True;
  }

  eResult SyntaxAnalyzer::DoWhileStmt() {
    return True;
  }

  eResult SyntaxAnalyzer::ForStmt() {
    return True;
  }

  eResult SyntaxAnalyzer::BreakStmt() {
    return True;
  }

  eResult SyntaxAnalyzer::ContinueStmt() {
    return True;
  }

  eResult SyntaxAnalyzer::GotoStmt() {
    return True;
  }

  eResult SyntaxAnalyzer::ReturnStmt() {
    return True;
  }

  void SyntaxAnalyzer::DebugPrint() {
  }

}
