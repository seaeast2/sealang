#ifndef _parser_h_
#define _parser_h_

#include "lexer.h"
#include "astcontext.h"
#include "error_diagnosis.h"

using namespace Lexer;

// 원칙 : 함수 내부에서 무언가 오류가 있을 때는 false 리턴
// 제대로 문법 범위 내에서 끝났으면 true 리턴
// todo : false 를 리턴 할 때 Error 의 종류를 파악 할 수 있는
// 추가적인 구조 생성
namespace Parser {

  class SyntaxAnalyzer {
    //Tokenizer* tokenizer_;
    WordChecker* wc_;

    ASTContext* astctx_;
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
