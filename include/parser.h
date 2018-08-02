#ifndef _parser_h_
#define _parser_h_

#include <string>
#include "common.h"
#include "core/stack.h"
#include "core/simple_vector.h"
#include "lexer.h"
#include "error_diagnosis.h"
#include "astcontext.h"
#include "ast_node.h"

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
    TokUnknown = 0,
    TokEof,
    TokImport, // Keywords
    TokStatic,
    TokIf,
    TokElse,
    TokDo,
    TokWhile,
    TokReturn,
    TokFor,
    TokBreak,
    TokTypeDef,
    TokConst,
    TokSwitch,
    TokCase,
    TokDefault,
    TokContinue,
    TokGoto,
    TokSizeOf,
    TokSigned,    // unsigned  // Types : byte int string double void struct
    TokUnsigned,  // signed
    TokChar,      // char
    TokShort,     // short
    TokInt,       // int
    TokLong,      // long
    TokFloat,     // float
    TokDouble,    // double
    TokVoid,      // void
    TokClass,     // class
    TokComBitShiftL,  // <<= // Operators
    TokComBitShiftR,  // >>=
    TokDotDotDot,     // ... // Triple operator
    TokBitShiftL,     // << // Double operator
    TokBitShiftR,     // >>
    TokUnaryInc,      // ++
    TokUnaryDec,      // --
    TokComAdd,        // +=
    TokComSub,        // -= 
    TokComMul,        // *=
    TokComDiv,        // /=
    TokComMod,        // %=
    TokComBitAnd,     // &=
    TokComBitOr,      // |=
    TokComBitXor,     // ^=
    TokEqual,         // ==
    TokNotEqual,      // !=
    TokLessThanEqual, // <=
    TokGreatorThenEqual,// >=
    TokConAnd,        // &&
    TokConOr,         // ||
    TokRightArrow,    // ->
    TokAdd,           // + // Single operators
    TokSub,           // -
    TokMul,           // *
    TokDiv,           // /
    TokMod,           // %
    TokAssign,        // =
    TokBitAnd,        // &
    TokBitOr,         // |
    TokBitXor,        // ^
    TokBitNot,        // ~
    TokLessThan,      // < 
    TokGreatorThan,   // >
    TokConNot,        // !
    TokQuestion,      // ?
    TokParenOpen,     // ( //  Parentheses
    TokParenClose,    // )
    TokBraceOpen,     // {
    TokBraceClose,    // }
    TokBracketOpen,   // [
    TokBracketClose,  // ]
    TokComma,         // , /* Etc : , . ; */
    TokDot,           // .
    TokColon,         // :
    TokSemiColon,     // ;
    TokDoubleQuot,    // "  // Quoto
    TokSingleQuot,    // '
    TokStarRemark,    // /* /* Remark */
    TokLineRemark,    // //
    TokIdentifier,    // ex) abcd012  /* Identifier */
    TokIntegerLiteral, // ex) 12345
    TokStringLiteral, // ex) "test"
    TokCharactorLiteral, // ex) 'A'
    TokEnd,
    // Rule
    compilation_unit, 
    import_stmts,
      import_stmt,
        rep_dot_name, // ("," name)*
    name,
    top_defs,
      sel_fun_var_const_class_typedef, // deffunc | defvars | defconst | defclass | typedef
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
      seq_param_void, // <VOID>
      seq_param_list, // fixedparams ["," "..."] 
      opt_vararg, // ["," "..."] 
    fixedparams,
      rep_comma_param, // ("," param)* 
    param,
    defvar_list,
    block,
    defclass,
      class_member_list,
        rep_class_member, // (class_member ";")
          class_member,
            seq_class_member_variable, // type name
            seq_class_member_function, // deffunc
    typedef_, // to avoid keyword name crash with C++, '_' is added.
    typeref,
      rep_sel_arry_ptr_fnptr,
        sel_arry_ptr_fnptr,
          seq_unassigned_array, // "[""]              // unassigned array type
          seq_assigned_array,   // "["<INTEGER>"]"    // assigned array type
          seq_ptr,              // *                  // pointer type
          seq_func,         // "(" param_typerefs ")" // function pointer type
    param_typerefs,
      seq_param_type_void,  // <VOID>
      seq_param_type_list,  // type ("," type)* ["," "..."] 
        rep_param_comma_type,     // ("," type)*
        opt_vararg_type,// ["," "..."] 
    param_type,
    stmts,
    stmt,
      labeled_stmt,
      expr_stmt, // expr ";"
      if_stmt,
        opt_else_stmt, // [<ELSE> stmt]
      while_stmt,
      dowhile_stmt,
      for_stmt,
        opt_for_init_expr, // [expr]
        opt_for_cond_expr, // [expr]
        opt_for_inc_expr, // [expr]
      switch_stmt,
        case_clauses,
          rep_case_clause,// (case_clause)*
            case_clause,
          opt_default_clause,// [default_clause]
            default_clause,
          case_body,
          case_list,
      break_stmt,
      continue_stmt,
      goto_stmt,
      return_stmt,
        seq_return, // <RETURN> ";"
        seq_return_expr, // <RETURN> expr ";"
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
      rep_muldivmod_term,
        sel_muldivmod_term,
          seq_mul_term,// "*" term  
          seq_div_term,// "/" term 
          seq_mod_term,// "%" term
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
      seq_po_expr_pc,// "(" expr ")"  // 234

    nil, // nil                   // 235

    // BNF Action
    RepeatStar,   // ()*          // 236
    RepeatDagger, // ()+          // 237
    Select,       // |            // 238
    Sequence,     // rule1 rule2  // 239
    Options,      // []           // 240
    Terminal                      // 241
  };

  struct Rule {
    RuleName action_; // repeat, select, sequence, terminal
    int lookahead_; // 0: no look ahead 1~15: look ahead number
    RuleName sub_rules_[15];
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
      TypeNodeList,
      StorageInfo,
      VarDeclList,
      ParamNodeList,
      ExprNodeList,
      CaseNodeList,
      StmtNodeList,
      Types,
    };

    union RawData {
      bool                  boolean_;   // Storageinfo
      char                  character_; // Character
      const char*           cstr_;      // String, Identifier
      long                  integer_;   // Integer
      AST::BaseNode*        node_;      // ASTNode
      AST::Type*            type_;      // ASTType
      Lexer::TokenType      tok_type_;  // TokenType
      AST::TypeNodes*       type_nodes_;// TypeNodeList
      AST::VariableDecls*   vardecls_;  // VarDeclList
      AST::ParamNodes*      param_nodes_;// ParamNodeList
      AST::ExprNodes*       expr_nodes_;// ExprNodeList
      AST::CaseNodes*       case_nodes_;// CaseNodeList
      AST::StmtNodes*       stmt_nodes_;// StmtNodeList
      AST::Types*           types_;     // Types
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
    Lexer::Tokenizer* tokenizer_;
    AST::ASTContext* ac_;
    ErrorDiag::Diagnosis* err_diag_;

    Rule rules_[MAX_RULES];
    SimpleArrayStack<ParseInfo> parse_stack_;

    typedef eResult (SyntaxAnalyzer::*fnRuleAction) (void);
    fnRuleAction rule_actions_[MAX_RULES];
    
    public:
      SyntaxAnalyzer(AST::ASTContext* ac, Lexer::Tokenizer* tk, ErrorDiag::Diagnosis* ed);
      ~SyntaxAnalyzer();

      Rule RuleSetter(RuleName action, int lookahead, 
          RuleName sub01 = TokUnknown, RuleName sub02 = TokUnknown, 
          RuleName sub03 = TokUnknown, RuleName sub04 = TokUnknown, 
          RuleName sub05 = TokUnknown, RuleName sub06 = TokUnknown, 
          RuleName sub07 = TokUnknown, RuleName sub08 = TokUnknown, 
          RuleName sub09 = TokUnknown, RuleName sub10 = TokUnknown, 
          RuleName sub11 = TokUnknown, RuleName sub12 = TokUnknown, 
          RuleName sub13 = TokUnknown, RuleName sub14 = TokUnknown, 
          RuleName sub15 = TokUnknown);

      void InitBasicRule();
      void InitRuleAction();
      eResult TraverseRule(RuleName entry);
      eResult TestRule(RuleName entry); // Do not run action. just run test

      // Entry point of parsing process
      eResult StartParser();

      eResult DoNothing(void) { return True; } // Dummy function for function pointer array
      eResult CompilationUnit(void); // compilation_unit
      
      eResult ImportStmts(void); // import_stmts
        eResult ImportStmt(void); // import_stmt
          eResult Act_rep_dot_name(void);//rep_dot_name

      eResult TopDefs(void); // top_defs : Top definitions 
      eResult DefFunc(void); // deffunc : function definition
      eResult DefVars(void); // defvars : variable definition
      eResult DefConst(void); // defconst : constant
      eResult DefVarList(void); // defvar_list

      eResult Name(void); // name : check if identifier
      eResult Storage(void); // storage : check if storage keyword

      eResult Type(void); // type

      eResult TypeDef(void); // typedef_

      eResult TypeRef(void); // typeref
        eResult Act_seq_unassigned_array(void); // seq_unassigned_array
        eResult Act_seq_assigned_array(void); // seq_assigned_array
        eResult Act_seq_ptr(void); // seq_ptr
        eResult Act_seq_func(void); // seq_func

      eResult TypeRefBase(void); // typeref_base
        eResult Act_seq_void(void); // seq_void
        eResult Act_seq_char(void); // seq_char
        eResult Act_seq_short(void); // seq_short
        eResult Act_seq_int(void); // seq_int
        eResult Act_seq_long(void); // seq_long
        eResult Act_seq_unsigned_char(void); // seq_unsigned_char
        eResult Act_seq_unsigned_short(void); // seq_unsigned_short
        eResult Act_seq_unsigned_int(void); // seq_unsigned_int
        eResult Act_seq_unsigned_long(void); // seq_unsigned_long
        eResult Act_seq_float(void); // seq_float
        eResult Act_seq_double(void); // seq_double
        eResult Act_seq_class_identifier(void); // seq_class_identifier
      
      eResult ParamTypeRefs(void); // param_typerefs
        eResult Act_seq_param_type_void(void); // seq_param_type_void
        eResult Act_seq_param_type_list(void); // seq_param_type_list
          eResult Act_rep_param_comma_type(void); // rep_param_comma_type
          eResult Act_opt_vararg_type(void); // opt_vararg_type

      eResult Params(void); // params
        eResult Act_seq_param_void(void);// seq_param_void
        eResult Act_seq_param_list(void);// seq_param_list
        eResult Act_opt_vararg(void); // opt_vararg
      eResult FixedParams(void); // fixedparams
        eResult Act_rep_comma_param(void); // rep_comma_param
      eResult Param(void); // param

      eResult Block(void); // block

      eResult DefClass(void); // defclass
        eResult Act_seq_class_member_variable(void);// defvars
        eResult Act_seq_class_member_function(void);// deffunc

      eResult Term(void); // term
        eResult Act_seq_type_term(void);// seq_type_term
      eResult Unary(void); // unary
        eResult Act_seq_preinc_unary(void);// seq_preinc_unary
        eResult Act_seq_predec_unary(void);// seq_predec_unary
        eResult Act_seq_pos_term(void);// seq_pos_term
        eResult Act_seq_neg_term(void);// seq_neg_term
        eResult Act_seq_not_term(void);// seq_not_term
        eResult Act_seq_bitnot_term(void);// seq_bitnot_term
        eResult Act_seq_ptr_term(void);// seq_ptr_term
        eResult Act_seq_adr_term(void);// seq_adr_term
        eResult Act_seq_sizeof_type(void);// seq_sizeof_type
        eResult Act_seq_sizeof_unary(void);// seq_sizeof_unary
      eResult Postfix(void); // postfix
        eResult Act_seq_post_inc(void); // seq_post_inc
        eResult Act_seq_post_dec(void); // seq_post_dec
        eResult Act_seq_array_reference(void); // seq_array_reference
        eResult Act_seq_dot_name(void); // seq_dot_name
        eResult Act_seq_arrow_name(void); // seq_arrow_name
        eResult Act_seq_fncall(void); // seq_fncall
      eResult Args(void); // args
        eResult Act_seq_args_expr(void); // seq_args_expr
        eResult Act_rep_args_expr(void); // rep_args_expr
      eResult Primary(void); // primary
        eResult Act_seq_po_expr_pc(void); // seq_po_expr_pc

      eResult Expr(void); // expr
        eResult Act_seq_assign_value(void); // seq_assign_value
        eResult Act_seq_opassign_value(void); // seq_opassign_value

      eResult OpAssignOp(void); // opassign_op
      
      eResult Expr10(void); // expr10
        eResult Act_opt_ternaryop(void); // opt_ternaryop

      eResult Expr9(void); // expr9
        eResult Act_rep_or_expr8(void);// rep_or_expr8

      eResult Expr8(void); // expr8
        eResult Act_rep_and_expr7(void);// rep_and_expr7

      eResult Expr7(void); // expr7
        eResult Act_seq_gr_expr6(void); // seq_gr_expr6
        eResult Act_seq_ls_expr6(void); // seq_ls_expr6
        eResult Act_seq_geq_expr6(void);// seq_geq_expr6
        eResult Act_seq_leq_expr6(void);// seq_leq_expr6
        eResult Act_seq_eq_expr6(void); // seq_eq_expr6
        eResult Act_seq_neq_expr6(void);// seq_neq_expr6
      eResult Expr6(void); // expr6
        eResult Act_rep_bitor_expr5(void);// rep_bitor_expr5
      eResult Expr5(void); // expr5
        eResult Act_rep_bitxor_expr4(void); // rep_bitxor_expr4
      eResult Expr4(void); // expr4
        eResult Act_rep_bitand_expr3(void); // rep_bitand_expr3
      eResult Expr3(void); // expr3
        eResult Act_seq_rshft_expr2(void); // seq_rshft_expr2
        eResult Act_seq_lshft_expr2(void); // seq_lshft_expr2
      eResult Expr2(void); // expr2
        eResult Act_seq_sum_expr1(void); // seq_sum_expr1
        eResult Act_seq_sub_expr1(void); // seq_sub_expr1
      eResult Expr1(void); // expr1
        eResult Act_seq_mul_term(void); // seq_mul_term
        eResult Act_seq_div_term(void); // seq_div_term
        eResult Act_seq_mod_term(void); // seq_mod_term

      eResult Stmts(void); // stmts
      eResult Stmt(void); // stmt
      eResult LabeledStmt(void); // labeled_stmt
      eResult ExprStmt(void); // epxr_stmt 
      eResult IfStmt(void); // if_stmt
        eResult Act_opt_else_stmt(void); // opt_else_stmt
      eResult WhileStmt(void); // while_stmt
      eResult DoWhileStmt(void); // dowhile_stmt 
      eResult ForStmt(void); // for_stmt 
        eResult Act_opt_for_init_expr(void); // opt_for_init_expr 
        eResult Act_opt_for_cond_expr(void); // opt_for_cond_expr
        eResult Act_opt_for_inc_expr(void); // opt_for_inc_expr
      eResult SwitchStmt(void); // switch_stmt 
        eResult CaseClauses(void);// case_clauses
          eResult DefaultClause(void); // default_clause
          eResult CaseClause(void); // case_clause
            eResult CaseList(void); // case_list
            eResult CaseBody(void); // case_body
      eResult BreakStmt(void); // break_stmt 
      eResult ContinueStmt(void); // continue_stmt 
      eResult GotoStmt(void); // goto_stmt 
      eResult ReturnStmt(void); // return_stmt 
        eResult Act_seq_return(void); // seq_return
        eResult Act_seq_return_expr(void); // seq_return_expr

      // Token Actions
      eResult ActTokIntegerLiteral(void);
      eResult ActTokCharacterLiteral(void);
      eResult ActTokStringLiteral(void);
      eResult ActTokIdentifier(void);

      // Utils
      void PushType(AST::Type* type, RuleName rname = RuleName::nil);
      void PushToken(int pos_offset = 0, RuleName rname = RuleName::nil); // Push Token to ParseInfo stack.
      void PushNode(AST::BaseNode* node, RuleName rname = RuleName::nil);
      void PushTypeNodes(AST::TypeNodes* type_nodes, RuleName rname = RuleName::nil);
      void PushVarDecls(AST::VariableDecls* var_decls, RuleName rname = RuleName::nil);
      void PushParamNodes(AST::ParamNodes* param_nodes, RuleName rname = RuleName::nil);
      void PushExprNodes(AST::ExprNodes* expr_nodes, RuleName rname = RuleName::nil);
      void PushCaseNodes(AST::CaseNodes* case_nodes, RuleName rname = RuleName::nil);
      void PushStmtNodes(AST::StmtNodes* stmt_nodes, RuleName rname = RuleName::nil);
      void PushTypes(AST::Types* types, RuleName rname = RuleName::nil);

      void SetRuleNameForPI(RuleName rname);
      void DebugPrint();
  };
}
#endif
