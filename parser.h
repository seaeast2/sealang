#ifndef _parser_h_
#define _parser_h_

#include "common.h"
#include "lexer.h"
#include "error_diagnosis.h"
#include "parser_actions.h"


using namespace Lexer;

namespace Parser {  
  // check grammar
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

      eResult TopDefs(); // Top defintions 
      eResult DefFunc(); // function defintion

      eResult Name(); // name : check if identifier
      eResult Storage(); // storage : check if storage keyword
      
      eResult TypeRef(); // typeref
      eResult TypeRefBase(); // typeref_base

      void DebugPrint();
  };
}
#endif
