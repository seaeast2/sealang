#ifndef _parser_h_
#define _parser_h_

#include "lexer.h"
#include "astcontext.h"
#include "error_diagnosis.h"

using namespace Lexer;

namespace Parser {  
  class SyntaxAnalyzer {

    Tokenizer* tokenizer_;

    AST::ASTContext* astctx_;
    ErrorDiag::ParserDiag err_diag_;

    
    public:
      SyntaxAnalyzer(Tokenizer* tk);
      ~SyntaxAnalyzer();

      bool CompilationUnit();
      // import
      bool ImportStmts();
      bool ImportStmt();

      bool TopDefs(); // Top defintions 
      bool DefFunc(); // Function definition
      bool DefVars(); // global Variable
      bool DefConst(); // Constant ?
      bool DefStruct(); // Struct
      bool DefUnion(); // Union
      bool DefTypedef();

      void DebugPrint();
  };
}
#endif
