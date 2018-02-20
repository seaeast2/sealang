#include "lexer.h"
#include "parser.h"

char* src =  
" \
import test.aaa.bbb.ccc; \n\
void test() {\n\
  int a = 10;\n\
}";

int main() {
  Tokenizer* tk = new Tokenizer(src);
  tk->CompileTokens();
  Parser::SyntaxAnalyzer s(tk);

  s.CompilationUnit();
  s.DebugPrint();

  delete tk;
  return 0;
}
