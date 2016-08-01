
#include "error_diagnosis.h"

namespace ErrorDiag {

void LexerDiag::Print(ErrorKind ekind, int line, int col) {
  if (ekind == Err_Lexer_UnexpectedEof) {
    cout << "line:"<<line<<" col:"<<col<<" Lexer meet unexpected eof."<< endl;
  }
}


void ParserDiag::Print(ErrorKind ekind, int line, int col) {

  switch(ekind) {
    case Err_Parser_NoIdentifier:
      cout << "line:"<<line<<" col:"<<col<<" No Identifier token."<< endl;
      break;
    case Err_Parser_NoSemicolon:
      cout << "line:"<<line<<" col:"<<col<<" No Semicolon."<< endl;
      break;
  }
}

};

