
#include "Lexer.h"
#include "Parser.h"
#include "ASTPrinter.h"


char* src =  
 " \
   int[4] a; \
   void[10] b;\
   char[6] c; \
 \0";


int main() {
  Lexer::Tokenizer* tk = new Lexer::Tokenizer(src);
  tk->CompileTokens();

  ErrorDiag::ParserDiag* parser_err = new ErrorDiag::ParserDiag();
  AST::ASTContext* ac = new AST::ASTContext();
  Parser::SyntaxAnalyzer* s = new Parser::SyntaxAnalyzer(ac, tk, parser_err);

  s->StartParser();
  ac->CheckLocalVar();
  ac->CheckVoidArray();

  ac->PrintAST();

  delete s;
  delete parser_err;
  delete ac;
  delete tk;

  return 0;
}
