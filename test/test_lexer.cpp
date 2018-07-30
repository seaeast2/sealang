
#include "lexer.h"


using namespace Lexer;

char * test_src = {
  //"!! ddd \n < > if else while struct void real string <= > !"
  "a = 10;"
};

int main() {
  Tokenizer tk(test_src);
  tk.CompileTokens();

  Token tok;
  while (true) {
    tok = tk.GetCurToken(0);
    tk.ConsumeToken();
    PrintToken(tok);
    if(tok.type == TokEof)
      break;
  }
  return 0;
}
