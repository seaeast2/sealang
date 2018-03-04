#ifndef _parser_h_
#define _parser_h_

#include "common.h"
#include "lexer.h"
#include "error_diagnosis.h"
#include "parser_actions.h"


using namespace Lexer;

namespace Parser {  
  // check grammar
  // eResult : True - matching
  //           False - unmatching
  //           Error - Syntax error
  class SyntaxAnalyzer {
    Tokenizer* tokenizer_;
    SyntaxAction* action_;
    ErrorDiag::Diagnosis* err_diag_;
    
    public:
      SyntaxAnalyzer(SyntaxAction* sa, Tokenizer* tk, ErrorDiag::Diagnosis* ed);
      ~SyntaxAnalyzer();

      eResult CompilationUnit(); // compilation_unit
      
      eResult ImportStmts(); // import_stmts
      eResult ImportStmt(); // import_stmt

      eResult TopDefs(); // top_defs : Top definitions 
      eResult DefFunc(); // deffunc : function definition
      eResult DefVars(); // defvars : variable definition <<== Working

      eResult Name(); // name : check if identifier
      eResult Storage(); // storage : check if storage keyword
      
      eResult Type(); // type
      eResult TypeRef(); // typeref
      eResult TypeRefBase(); // typeref_base
      
      eResult ParamTypeRefs(); // param_typerefs
      eResult Params(); // params
      eResult FixedParams(); // fixedparams
      eResult Param(); // param

      eResult Block(); // block <<== Working
      eResult Expr(); // expr <<== Working

      void DebugPrint();
  };
}
#endif
