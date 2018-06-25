#include "lexer.h"
#include "parser.h"
#include "astcontext.h"
#include "error_diagnosis.h"
#include "parser_actions.h"

int main(int argc, char** argv) {
  // TODO : read source file
  char* src = nullptr;
  Lexer::Tokenizer* tk = new Lexer::Tokenizer(src);
  tk->CompileTokens();

  ErrorDiag::ParserDiag* parser_err = new ErrorDiag::ParserDiag();
  AST::ASTContext* ac = new AST::ASTContext();
  Parser::SyntaxAnalyzer s(ac, tk, parser_err);

  //s.CompilationUnit();
  //s.DebugPrint();

  delete parser_err;
  delete ac;
  delete tk;
  return 0;
}
