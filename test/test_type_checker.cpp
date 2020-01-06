#include "Lexer.h"
#include "Parser.h"
#include "ASTPrinter.h"


char* src =  
 " \
 class test { \
   long b; \
   char c; \
 }; \
 class test d; \
 ";

/*char* src =  
 " \
 int a = 10; \
 int* b = &a; \
 long[10] c; \
 class test { \
   long b; \
   char c; \
 }; \
\
test d; \
test[] e; \
 ";*/


int main() {
  Lexer::Tokenizer* tk = new Lexer::Tokenizer(src);
  tk->CompileTokens();

  ErrorDiag::ParserDiag* parser_err = new ErrorDiag::ParserDiag();
  AST::ASTContext* ac = new AST::ASTContext();
  Parser::SyntaxAnalyzer* s = new Parser::SyntaxAnalyzer(ac, tk, parser_err);

  s->StartParser();
  ac->CheckLocalVar();
  ac->MakeCompleteType();

  ac->PrintAST();

  delete s;
  delete parser_err;
  delete ac;
  delete tk;

  return 0;
}
