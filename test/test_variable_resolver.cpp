
#include "lexer.h"
#include "parser.h"
#include "ast_printer.h"


char* src =  
"void test(void) { \
    long hoho = 20; \
    int hoho; \
    { \
      char hoho = 30; \
      { \
        hoho = hoho + 100; \
        short hoho = 40; \
      } \
\
      { \
        hoho = 20;\
      } \
    }\
    \
    return;\
 }\n";

int main() {
  Lexer::Tokenizer* tk = new Lexer::Tokenizer(src);
  tk->CompileTokens();

  ErrorDiag::ParserDiag* parser_err = new ErrorDiag::ParserDiag();
  AST::ASTContext* ac = new AST::ASTContext();
  Parser::SyntaxAnalyzer* s = new Parser::SyntaxAnalyzer(ac, tk, parser_err);

  s->StartParser();
  ac->ResolveVar();

  //ac->PrintAST();

  delete s;
  delete parser_err;
  delete ac;
  delete tk;

  return 0;
}
