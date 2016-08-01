
#include "lexer.h"


using namespace Lexer;

char * test_src = {
  "!! ddd \n < > if else while struct void real string <=>!"
};

int main() {
  Tokenizer tk(test_src);

  Token tok;
  while (true) {
    tok = tk.GetToken();
    PrintToken(tok);
    if(tok.type == TokEof || tok.error == TokErr_UnexpectedEof)
      break;
  }
  return 0;
}
