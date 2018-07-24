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

  ErrorDiag::ParserDiag* parser_err = new ErrorDiag::ParserDiag();
  AST::ASTContext* ac = new AST::ASTContext();
  Parser::SyntaxAnalyzer* s = new Parser::SyntaxAnalyzer(ac, tk, parser_err);

  s->StartParse();

  delete s;
  delete parser_err;
  delete ac;
  delete tk;

  return 0;
}
