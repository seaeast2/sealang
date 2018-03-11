#ifndef _parser_h_
#define _parser_h_

#include <string.h>
#include "common.h"
#include "core/stack.h"
#include "lexer.h"
#include "error_diagnosis.h"
#include "parser_actions.h"
#include "ast_node.h"



using namespace Lexer;

namespace Parser {  
  // default 3 rusult.
  enum eResult {
    False = 0,
    True,
    Error
  };

#define MAX_RULES 300
  enum RuleName {
    // Token
    
    // Rule
    compilation_unit = TokEnd,
    import_stmts,
    import_stmt,
    name,
    top_defs,
    typeref_base,
    defvars,
    storage,
    type,
    defconst,
    deffunc,
    params,
    fixedparams,
    param,
    defvar_list,
    block,
    defclass,
    member_list,
    class_member,
    typedef_, // to avoid type crash, '_' is added.
    typeref,
    param_typerefs,
    stmts,
    stmt,
    labeled_stmt,
    if_stmt,
    while_stmt,
    dowhile_stmt,
    for_stmt,
    switch_stmt,
    case_clauses,
    case_clause,
    case_body,
    default_clause,
    cases,
    break_stmt,
    continue_stmt,
    goto_stmt,
    return_stmt,
    expr,
    opassign_op,
    expr10,
    expr9,
    expr8,
    expr7,
    expr6,
    expr5,
    expr4,
    expr3,
    expr2,
    expr1,
    term,
    unary,
    postfix,
    args,
    primary,

    // sub rule
    seq_unsigned_char, // <UNSIGNED> <CHAR>
    seq_unsigned_short, // <UNSIGNED> <SHORT>
    seq_unsigned_int, // <UNSIGNED> <INT>
    seq_unsigned_long, // <UNSIGNED> <LONG>
    seq_class_identifier, // <CLASS> <IDENTIFIER>
    rep_dot_name, // ("," name)*
    sel_fun_var_const_class_typedef, // deffunc | defvars | defconst | defclass | typedef
    opt_eq_expr, // ["=" expr]
    opt_rep_cm_name_dot_eq_expr, // [("," name ["=" expr])*]
    rep_cm_name_dot_eq_expr, // ("," name ["=" expr])
    seq_fixparms_dot_dotdotdot, // fixedparams ["," "..."] 
    opt_dot_dotdotdot, // ["," "..."] 
    rep_dot_param, // ("," param)* 
    rep_class_member_semicolon, // (class_member ";")
    seq_type_name, // type name
    rep_sel_arry_ptr_fnptr,
    sel_arry_ptr_fnptr,
    seq_bo_bc, // "[""]
    seq_bo_int_bc, // "["<INTEGER>"]"
    seq_po_paramty_pc, // "(" param_typerefs ")"
    seq_type_rep_type_dot, // type ("," type)* ["," "..."] 
    rep_dot_type, // ("," type)*
    seq_expr_semicolon, // expr ";"
    opt_else_stmt, // [<ELSE> stmt]
    opt_expr, // [expr]
    seq_return_semicolon, // <RETURN> ";"
    seq_return_expr_semicolon, // <RETURN> expr ";"
    seq_term_eq_expr, // term "=" expr  
    seq_term_ops_expr, // term opassign_op expr 
    opt_ternaryop,// ["?" expr ":" expr10] 
    rep_or_expr8, //("||" expr8)* 
    rep_and_expr7,// ("&&" expr7)* 
    rep_op_expr6,
    sel_op_expr6,
    seq_gr_expr6, // ">" expr6  
    seq_ls_expr6, // "<" expr6 
    seq_geq_expr6,// ">=" expr6 
    seq_leq_expr6,// "<=" expr6 
    seq_eq_expr6, // "==" expr6 
    seq_neq_expr6,// "!=" expr6 
    rep_bitor_expr5, // ("|" expr5)* 
    rep_bitxor_expr4,// ("^" expr4)* 
    rep_bitand_expr3,// ("&" expr3)* 
    rep_shift_expr2,// ( ">>" expr2 | "<<" expr2)*
    sel_shift_expr2,// ">>" expr2 | "<<" expr2
    seq_rshft_expr2,// ">>" expr2
    seq_lshft_expr2, // "<<" expr2
    rep_sumsub_expr1,// ( "+" expr1 | "-" expr1)* 
    sel_sumsub_expr1,// "+" expr1 | "-" expr1 
    seq_sum_expr1,// "+" expr1   
    seq_sub_expr1,// "-" expr1
    rep_muldivmod_term,
    sel_muldivmod_term,
    seq_mul_term,// "*" term  
    seq_div_term,// "/" term 
    seq_mod_term,// "%" term
    seq_type_term, //"(" type ")" term
    seq_preinc_unary,// "++" unary
    seq_predec_unary,// "--" unary
    seq_pos_term,// "+" term 
    seq_neg_term,// "-" term
    seq_not_term,// "!" term 
    seq_ptr_term,// "*" term
    seq_adr_term,// "&" term 
    seq_sizeof_type,// <SIZEOF> "(" type ")"
    seq_sizeof_unary,// <SIZEOF> unary
    seq_primary_incdec,// primary ("++"|"--")
    sel_incdec,// "++"|"--"
    seq_primary_reffunc, // primary ("[" expr "]" | "." name | "->" name | "(" args ")")*
    rep_reffunc, //("[" expr "]" | "." name | "->" name | "(" args ")")*
    sel_reffunc, //"[" expr "]" | "." name | "->" name | "(" args ")"
    seq_bo_expr_bc, //"[" expr "]"
    seq_dot_name, //"." name
    seq_arrow_name, //"->" name
    seq_po_args_pc, //"(" args ")"
    rep_dot_expr, // ("," expr)*
    seq_po_expr_pc,// "(" expr ")" 
    rep_case_clause,// (case_clause)*
    opt_default_clause,// [default_clause]

    // BNF Action
    Repeat, // ()*
    Select, // |
    Sequence, // rule1 rule2
    Options, // []
    Terminal,
    Nonterminal
  };

  struct Rule {
    int action_; // repeat, select, sequence, terminal, nonterminal
    int sub_rules_[15];
  };


  // check grammar
  // eResult : True - matching
  //           False - unmatching
  //           Error - Syntax error
  class SyntaxAnalyzer {
    Tokenizer* tokenizer_;
    SyntaxAction* action_;
    ErrorDiag::Diagnosis* err_diag_;

    Rule rules_[MAX_RULES];
    SimpleArrayStack<AST::BaseNode*> node_stack_;

    typedef eResult (SyntaxAnalyzer::*fnRuleAction) (void);
    fnRuleAction rule_actions_[MAX_RULES];

    
    public:
      SyntaxAnalyzer(SyntaxAction* sa, Tokenizer* tk, ErrorDiag::Diagnosis* ed);
      ~SyntaxAnalyzer();

      void InitBasicRule();
      void InitRuleAction();
      eResult TraverseRule(int entry);

      //void DoNothing(void) {} // Dummy function for function pointer array
      eResult CompilationUnit(); // compilation_unit
      
      eResult ImportStmts(); // import_stmts
      eResult ImportStmt(); // import_stmt

      eResult TopDefs(); // top_defs : Top definitions 
      eResult DefFunc(); // deffunc : function definition
      eResult DefVars(); // defvars : variable definition
      eResult DefVarList(); // defvar_list

      eResult Name(); // name : check if identifier
      eResult Storage(); // storage : check if storage keyword
      
      eResult Type(); // type
      eResult TypeRef(); // typeref
      eResult TypeRefBase(); // typeref_base
      eResult TypeDef(); // typedef
      
      eResult ParamTypeRefs(); // param_typerefs
      eResult Params(); // params
      eResult FixedParams(); // fixedparams
      eResult Param(); // param

      eResult Block(); // block <<== Working
      eResult Expr(); // expr <<== Working
      eResult Term(); // term <<== Working
      eResult Unary(); // unary <<== Working
      eResult Postfix(); // postfix
      eResult Primary(); // primary
      eResult Args(); // args
      eResult OpAssignOp(); // opassign_op <<== Working
      eResult Expr10(); // expr10 <<== Working
      eResult Expr9(); // expr9 <<== Working
      eResult Expr8(); // expr8 <<== Working
      eResult Expr7(); // expr7 <<== Working
      eResult Expr6(); // expr6 <<== Working
      eResult Expr5(); // expr5 <<== Working
      eResult Expr4(); // expr4 <<== Working
      eResult Expr3(); // expr3 <<== Working
      eResult Expr2(); // expr2 <<== Working
      eResult Expr1(); // expr1 <<== Working

      eResult Stmts(); // stmts <<== Working
      eResult Stmt(); // stmt <<== Working
      eResult LabeledStmt(); // labeled_stmt <<== Working
      eResult IfStmt(); // if_stmt <<== Working
      eResult WhileStmt(); // while_stmt <<== Working
      eResult DoWhileStmt(); // dowhile_stmt <<== Working
      eResult ForStmt(); // for_stmt <<== Working
      eResult BreakStmt(); // break_stmt <<== Working
      eResult ContinueStmt(); // continue_stmt <<==Working
      eResult GotoStmt(); // goto_stmt <<==Working
      eResult ReturnStmt(); // return_stmt <<==Working



      void DebugPrint();
  };
}
#endif
