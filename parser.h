#ifndef _parser_h_
#define _parser_h_

#include "common.h"
#include "lexer.h"
#include "error_diagnosis.h"
#include "parser_actions.h"


using namespace Lexer;

namespace Parser {  
  enum RuleName {
    // Token
    
    // Rule
    compilation_unit = TokEnd,
    import_stmts,
    import_stmt,
    rep_dot_name, // ("," name)*
    name,
    top_defs,
    typeref_base,

    seq_unsigned_char, // <UNSIGNED> <CHAR>
    seq_unsigned_short, // <UNSIGNED> <SHORT>
    seq_unsigned_int, // <UNSIGNED> <INT>
    seq_unsigned_long, // <UNSIGNED> <LONG>
    seq_class_identifier, // <CLASS> <IDENTIFIER>
    // Action
    Repeat,
    Select,
    Sequence,
    Terminal,
    Nonterminal
  };

  struct Rule {
    int action_; // repeat, select, sequence, terminal, nonterminal
    int sub_rules_[20];
  };


  // check grammar
  // eResult : True - matching
  //           False - unmatching
  //           Error - Syntax error
  class SyntaxAnalyzer {
    Tokenizer* tokenizer_;
    SyntaxAction* action_;
    ErrorDiag::Diagnosis* err_diag_;

    Rule rules_[200];

    typedef void (SyntaxAnalyzer::*fnRuleAction) (void);
    fnRuleAction rule_actions_[200];
    
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
