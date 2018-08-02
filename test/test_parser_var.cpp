#include "lexer.h"
#include "parser.h"
#include "ast_printer.h"

char* src =  
"int a, b = 10;\n";

/*char* src =  
"void test(void) { \
    return;\
 }";*/

int main() {
  Lexer::Tokenizer* tk = new Lexer::Tokenizer(src);
  tk->CompileTokens();

  ErrorDiag::ParserDiag* parser_err = new ErrorDiag::ParserDiag();
  AST::ASTContext* ac = new AST::ASTContext();
  Parser::SyntaxAnalyzer* s = new Parser::SyntaxAnalyzer(ac, tk, parser_err);

  s->StartParser();
  ac->PrintAST();

  delete s;
  delete parser_err;
  delete ac;
  delete tk;

  return 0;
}
