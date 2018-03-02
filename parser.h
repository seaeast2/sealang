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


    ErrorDiag::ParserDiag err_diag_;
    
    public:
      SyntaxAnalyzer(SyntaxAction* sa, Tokenizer* tk);
      ~SyntaxAnalyzer();

      bool CompilationUnit(); // compilation_unit
      
      bool ImportStmts(); // import_stmts
      bool ImportStmt(); // import_stmt

      bool TopDefs(); // Top defintions 
      bool DefFunc(); // function defintion

      bool Name(unsigned int look); // name : check if identifier
      bool Storage(); // storage : check if storage keyword

      void DebugPrint();
  };
}
#endif
