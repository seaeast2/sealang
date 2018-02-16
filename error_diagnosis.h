#ifndef _error_diagnosis_h_
#define _error_diagnosis_h_

#include <iostream>
using namespace std;

namespace ErrorDiag {
  enum ErrorKind {
    Err_Lexer_UnexpectedEof,

    Err_Parser_NoIdentifier,
    Err_Parser_NoSemicolon
  };

  class Diagnosis {
    public:
      Diagnosis() {}
      virtual ~Diagnosis() {}
  };

  class LexerDiag : public Diagnosis {
    public:
      LexerDiag() {}
      ~LexerDiag() {}

      void Print(ErrorKind ekind, int line, int col);
  };

  class ParserDiag : public Diagnosis {
    public:
      ParserDiag() {}
      ~ParserDiag() {}

      void Print(ErrorKind ekind, int line, int col);
  };
};

#endif
