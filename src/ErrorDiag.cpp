#include "ErrorDiag.h"

using namespace std;

// Error diagnosis system package
namespace ErrorDiag {

  void Diagnosis::Print(ErrorKind ekind, int line, int col, char* errMsg) {
      cout << "line:"<<line<<" col:"<<col<<" Unidentified error detected."<< endl;
  }

  void LexerDiag::Print(ErrorKind ekind, int line, int col, char* errMsg) {
    if (ekind == ErrorKind::Err_Lexer_UnexpectedEof) {
      cout << "line:"<<line<<" col:"<<col<<" Lexer meet unexpected eof."<< endl;
    }
    if (ekind == ErrorKind::Err_Lexer_WrongCharLiteral) {
      cout << "line:"<<line<<" col:"<<col<<" Lexer : Wrong Charactorliteral."<< endl;
    }
  }


  void ParserDiag::Print(ErrorKind ekind, int line, int col, char* errMsg) {

    switch(ekind) {
      case Err_Parser_NoIdentifier:
        cout << "line:"<<line<<" col:"<<col<<" No Identifier token."<< errMsg << endl;
        break;
      case Err_Parser_NoSemicolon:
        cout << "line:"<<line<<" col:"<<col<<" No Semicolon."<< errMsg << endl;
        break;
      default:
        break;
    }
  }

};

