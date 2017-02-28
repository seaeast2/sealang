#ifndef _parser_h_
#define _parser_h_

#include "lexer.h"
#include "astcontext.h"
#include "error_diagnosis.h"

using namespace Lexer;

// ��Ģ : �Լ� ���ο��� ���� ������ ���� ���� false ����
// ����� ���� ���� ������ �������� true ����
// todo : false �� ���� �� �� Error �� ������ �ľ� �� �� �ִ�
// �߰����� ���� ����
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