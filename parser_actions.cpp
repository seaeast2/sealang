#include "parser_actions.h"

namespace Parser {
  SyntaxAction::SyntaxAction(AST::ASTContext* ac, Lexer::Tokenizer* tk, 
      ErrorDiag::Diagnosis* ed) {
    tokenizer_ = tk;
    astctx_ = ac;
    err_diag_ = ed;
  };

  SyntaxAction::~SyntaxAction() {
  }
};


