#ifndef _error_diagnosis_h_
#define _error_diagnosis_h_

#include <iostream>

namespace ErrorDiag {
  enum ErrorKind {
    Err_Lexer_UnexpectedEof,
    Err_Lexer_WrongCharLiteral,

    Err_Parser_NoIdentifier,
    Err_Parser_NoSemicolon,

  };

  class Diagnosis {
    public:
      Diagnosis() {}
      virtual ~Diagnosis() {}

      void Print(ErrorKind ekind, int line, int col, char* errMsg = "");
  };

  class LexerDiag : public Diagnosis {
    public:
      LexerDiag() {}
      ~LexerDiag() {}

      void Print(ErrorKind ekind, int line, int col, char* errMsg = "");
  };

  class ParserDiag : public Diagnosis {
    public:
      ParserDiag() {}
      ~ParserDiag() {}

      void Print(ErrorKind ekind, int line, int col, char* errMsg = "");
  };
};

#endif
