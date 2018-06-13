#ifndef _parser_h_
#define _parser_h_

#include <string>
#include "common.h"
#include "core/stack.h"
#include "core/simple_vector.h"
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
#define MAX_SUB_RULES 15
  enum RuleName {
    // Token
    
    // Rule
    compilation_unit = TokEnd,
    import_stmts,
    import_stmt,
    name,
    top_defs,
    typeref_base,
      seq_void,         // <VOID>
      seq_char,         // <CHAR>
      seq_short,        // <SHORT>
      seq_int,          // <INT>
      seq_long,         // <LONG>
      seq_unsigned_char, // <UNSIGNED> <CHAR>
      seq_unsigned_short, // <UNSIGNED> <SHORT>
      seq_unsigned_int, // <UNSIGNED> <INT>
      seq_unsigned_long, // <UNSIGNED> <LONG>
      seq_float,        // <FLOAT>
      seq_double,       // <DOUBLE>
      seq_class_identifier, // <CLASS> <IDENTIFIER>
    defvars, // storage type name ["=" expr] [("," name ["=" expr])*] ";" 
      opt_var_initialize, // ["=" expr]
      opt_rep_var_initialize, // [("," name ["=" expr])*]
        rep_var_initialize, // ("," name ["=" expr])
    storage,
    type,
    defconst,
    deffunc,
    params,
      opt_vararg, // ["," "..."] 
    fixedparams,
    param,
    defvar_list,
    block,
    defclass,
    member_list,
    class_member,
    typedef_, // to avoid keyword name crash with C++, '_' is added.
    typeref,
      rep_sel_arry_ptr_fnptr,
        sel_arry_ptr_fnptr,
          seq_unassigned_array, // "[""]              // unassigned array type
          seq_assigned_array,   // "["<INTEGER>"]"    // assigned array type
          seq_ptr,              // *                  // pointer type
          seq_func,         // "(" param_typerefs ")" // function pointer type
    param_typerefs,
      seq_param_void,       // <VOID>
      seq_param_type_list,  // type ("," type)* ["," "..."] 
        rep_param_comma_type,     // ("," type)*
        opt_vararg_type,// ["," "..."] 
    param_type,
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
      seq_assign_value, // term "=" expr  
      seq_opassign_value, // term opassign_op expr 
    opassign_op,
    expr10,
      opt_ternaryop,// ["?" expr ":" expr10] 
    expr9,
      rep_or_expr8, //("||" expr8)* 
    expr8,
      rep_and_expr7,// ("&&" expr7)* 
    expr7,
      rep_op_expr6,
        sel_op_expr6,
          seq_gr_expr6, // ">" expr6  
          seq_ls_expr6, // "<" expr6 
          seq_geq_expr6,// ">=" expr6 
          seq_leq_expr6,// "<=" expr6 
          seq_eq_expr6, // "==" expr6 
          seq_neq_expr6,// "!=" expr6 
    expr6, // expr5 ("|" expr5)* 
      rep_bitor_expr5, // ("|" expr5)* 
    expr5, // expr4 ("^" expr4)* 
      rep_bitxor_expr4,// ("^" expr4)* 
    expr4,// expr3 ("&" expr3)* 
      rep_bitand_expr3,// ("&" expr3)* 
    expr3,// expr2 ( ">>" expr2 | "<<" expr2)* 
      rep_shift_expr2,      // ( ">>" expr2 | "<<" expr2)*
        sel_shift_expr2,    // ">>" expr2 | "<<" expr2
          seq_rshft_expr2,  // ">>" expr2
          seq_lshft_expr2,  // "<<" expr2
    expr2,
      rep_sumsub_expr1,// ( "+" expr1 | "-" expr1)* 
        sel_sumsub_expr1,// "+" expr1 | "-" expr1 
          seq_sum_expr1,// "+" expr1   
          seq_sub_expr1,// "-" expr1
    expr1,
    term,
      seq_type_term, //"(" type ")" term
    unary,
      seq_preinc_unary,// "++" unary
      seq_predec_unary,// "--" unary
      seq_pos_term,// "+" term 
      seq_neg_term,// "-" term
      seq_not_term,// "!" term 
      seq_bitnot_term,// "~" term 
      seq_ptr_term,// "*" term
      seq_adr_term,// "&" term 
      seq_sizeof_type,// <SIZEOF> "(" type ")"
      seq_sizeof_unary,// <SIZEOF> unary
    postfix,
      rep_reffunc, //("++" | "--" | "[" expr "]" | "." name | "->" name | "(" args ")")*
        sel_reffunc, // "++" | "--" | "[" expr "]" | "." name | "->" name | "(" args ")"
          seq_post_inc, // "++"
          seq_post_dec, // "--"
          seq_array_reference, //"[" expr "]"
          seq_dot_name, //"." name
          seq_arrow_name, //"->" name
          seq_fncall, //"(" args ")"
    args,
      seq_args_expr, // expr
      rep_args_expr, // ("," expr)*
    primary,
      seq_po_expr_pc,// "(" expr ")" 

    // sub rule
    rep_dot_name, // ("," name)*
    sel_fun_var_const_class_typedef, // deffunc | defvars | defconst | defclass | typedef
    seq_fixedparams_vararg, // fixedparams ["," "..."] 
    rep_comma_param, // ("," param)* 
    rep_class_member_semicolon, // (class_member ";")
    seq_type_name, // type name
    seq_expr_semicolon, // expr ";"
    opt_else_stmt, // [<ELSE> stmt]
    opt_expr, // [expr]
    seq_return_semicolon, // <RETURN> ";"
    seq_return_expr_semicolon, // <RETURN> expr ";"
    rep_muldivmod_term,
    sel_muldivmod_term,
    seq_mul_term,// "*" term  
    seq_div_term,// "/" term 
    seq_mod_term,// "%" term
    rep_case_clause,// (case_clause)*
    opt_default_clause,// [default_clause]

    nil, // nil

    // BNF Action
    Repeat, // ()*
    Select, // |
    Sequence, // rule1 rule2
    Options, // []
    Terminal
  };

  struct Rule {
    int action_; // repeat, select, sequence, terminal
    int sub_rules_[15];
  };


  struct ParseInfo {
    enum RawDataType {
      Identifier,
      Integer,
      Character,
      String,
      ASTNode,
      ASTType,
      TokenType,
      TypeList,
      StorageInfo,
      VarDeclList,
    };

    union RawData {
      bool boolean_;
      char character_;
      const char* cstr_;
      long integer_;
      AST::BaseNode* node_;
      AST::Type* type_;
      Lexer::TokenType tok_type_;
      SimpleVector<AST::Type*>* types_;
      SimpleVector<AST::VariableDecl*>* vardecls_;
    };

    RawDataType type_;
    RawData data_;
    int cstr_len_; // string length;
    int token_idx_; // token index
    RuleName rule_name_;

    ParseInfo() {
      memset(&data_, 0, sizeof(data_));
    }
    ~ParseInfo() {
      if (type_ == Character) {
        if (data_.cstr_)
          delete data_.cstr_;
      }
    }
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
    SimpleArrayStack<ParseInfo> parse_stack_;

    typedef eResult (SyntaxAnalyzer::*fnRuleAction) (void);
    fnRuleAction rule_actions_[MAX_RULES];
    
    public:
      SyntaxAnalyzer(SyntaxAction* sa, Tokenizer* tk, ErrorDiag::Diagnosis* ed);
      ~SyntaxAnalyzer();

      void InitBasicRule();
      void InitRuleAction();
      eResult TraverseRule(int entry);
      eResult TestRule(int entry);

      eResult DoNothing(void) { return True; } // Dummy function for function pointer array
      eResult CompilationUnit(void); // compilation_unit
      
      eResult ImportStmts(void); // import_stmts
        eResult ImportStmt(void); // import_stmt

      eResult TopDefs(void); // top_defs : Top definitions 
      eResult DefFunc(void); // deffunc : function definition
      eResult DefVars(void); // defvars : variable definition
      eResult DefVarList(void); // defvar_list

      eResult Name(void); // name : check if identifier
      eResult Storage(void); // storage : check if storage keyword
      
      eResult Type(void); // type
      eResult TypeRef(void); // typeref
        eResult Act_seq_unassigned_array(void); // unassigned array type
        eResult Act_seq_assigned_array(void); // assigned array type
        eResult Act_seq_ptr(void); // pointer type
        eResult Act_seq_func(void); // function type


      eResult TypeRefBase(void); // typeref_base
        eResult Act_seq_void(void); 
        eResult Act_seq_char(void);
        eResult Act_seq_short(void);
        eResult Act_seq_int(void);
        eResult Act_seq_long(void);
        eResult Act_seq_unsigned_char(void);
        eResult Act_seq_unsigned_short(void);
        eResult Act_seq_unsigned_int(void);
        eResult Act_seq_unsigned_long(void);
        eResult Act_seq_float(void);
        eResult Act_seq_double(void);
        eResult Act_seq_class_identifier(void);

      eResult TypeDef(void); // typedef
      
      eResult ParamTypeRefs(void); // param_typerefs
        eResult Act_seq_param_void(void); // <VOID> 
        eResult Act_seq_param_type_list(void); // param_type ("," type)* ["," "..."]
          eResult Act_rep_param_comma_type(void); // ("," type)*
          eResult Act_opt_vararg_type(void); // ["," "..."] 
      eResult ParamType(void); // param_type

      eResult Params(void); // params
      eResult FixedParams(void); // fixedparams
      eResult Param(void); // param
        eResult Act_opt_vararg(void); //opt_vararg

      eResult Block(void); // block
      eResult Term(void); // term
        eResult Act_seq_type_term(void);//"(" type ")" term  // typecasting
      eResult Unary(void); // unary
        eResult Act_seq_preinc_unary(void);// "++" unary
        eResult Act_seq_predec_unary(void);// "--" unary
        eResult Act_seq_pos_term(void);// "+" term 
        eResult Act_seq_neg_term(void);// "-" term
        eResult Act_seq_not_term(void);// "!" term 
        eResult Act_seq_bitnot_term(void);// "~" term 
        eResult Act_seq_ptr_term(void);// "*" term
        eResult Act_seq_adr_term(void);// "&" term 
        eResult Act_seq_sizeof_type(void);// <SIZEOF> "(" type ")"
        eResult Act_seq_sizeof_unary(void);// <SIZEOF> unary
      eResult Postfix(void); // postfix
        eResult Act_seq_post_inc(void);
        eResult Act_seq_post_dec(void);
        eResult Act_seq_array_reference(void);
        eResult Act_seq_dot_name(void);
        eResult Act_seq_arrow_name(void);
        eResult Act_seq_fncall(void);
      eResult Args(void); // args
        eResult Act_seq_args_expr(void);
        eResult Act_rep_args_expr(void);
      eResult Primary(void); // primary
        eResult Act_seq_po_expr_pc(void);

      eResult Expr(void); // expr
        eResult Act_seq_assign_value(void);
        eResult Act_seq_opassign_value(void);

      eResult OpAssignOp(void); // opassign_op
      
      eResult Expr10(void); // expr10
        eResult Act_opt_ternaryop(void); // ["?" expr ":" expr10]

      eResult Expr9(void); // expr9
        eResult Act_rep_or_expr8(void);//("||" expr8)* 

      eResult Expr8(void); // expr8
        eResult Act_rep_and_expr7(void);// ("&&" expr7)* 

      eResult Expr7(void); // expr7
        eResult Act_seq_gr_expr6(void); // ">" expr6  
        eResult Act_seq_ls_expr6(void); // "<" expr6 
        eResult Act_seq_geq_expr6(void);// ">=" expr6 
        eResult Act_seq_leq_expr6(void);// "<=" expr6 
        eResult Act_seq_eq_expr6(void); // "==" expr6 
        eResult Act_seq_neq_expr6(void);// "!=" expr6 
      eResult Expr6(void); // expr6
        eResult Act_rep_bitor_expr5(void);// ("|" expr5)* 
      eResult Expr5(void); // expr5
        eResult Act_rep_bitxor_expr4(void);
      eResult Expr4(void); // expr4
        eResult Act_rep_bitand_expr3(void);
      eResult Expr3(void); // expr3
        eResult Act_seq_rshft_expr2(void);
        eResult Act_seq_lshft_expr2(void);
      eResult Expr2(void); // expr2
        eResult Act_seq_sum_expr1(void);
        eResult Act_seq_sub_expr1(void);
      eResult Expr1(void); // expr1

      eResult Stmts(void); // stmts
      eResult Stmt(void); // stmt
      eResult LabeledStmt(void); // labeled_stmt
      eResult IfStmt(void); // if_stmt
      eResult WhileStmt(void); // while_stmt
      eResult DoWhileStmt(void); // dowhile_stmt 
      eResult ForStmt(void); // for_stmt 
      eResult BreakStmt(void); // break_stmt 
      eResult ContinueStmt(void); // continue_stmt 
      eResult GotoStmt(void); // goto_stmt 
      eResult ReturnStmt(void); // return_stmt 

      // Token Actions
      eResult ActTokIntegerLiteral(void);
      eResult ActTokCharacterLiteral(void);
      eResult ActTokStringLiteral(void);
      eResult ActIdentifier(void);

      // Utils
      void PushType(AST::Type* type, RuleName rname = RuleName::nil);
      void PushToken(int pos_offset = 0, RuleName rname = RuleName::nil); // Push Token to ParseInfo stack.
      void PushNode(AST::BaseNode* node, RuleName rname = RuleName::nil);
      void PushTypeList(SimpleVector<AST::Type*>* ty_list, RuleName rname = RuleName::nil);
      void PushVarDecls(SimpleVector<AST::VariableDecl*>* var_list, RuleName rname = RuleName::nil);
      void SetRuleNameForPI(RuleName rname);
      void DebugPrint();
  };
}
#endif
