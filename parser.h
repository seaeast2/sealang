#ifndef _parser_h_
#define _parser_h_

#include "lexer.h"
#include "ast.h"
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

      bool CompilationUnit(); // compilation_unit
      
      bool ImportStmts(); // import_stmts
      bool ImportStmt(); // import_stmt

      bool TopDefs(AST::RootNode* pRND); // Top defintions 

      bool Name(int look); // name : check if identifier

      void DebugPrint();
  };
}
#endif
