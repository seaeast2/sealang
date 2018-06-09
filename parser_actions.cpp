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

  void SyntaxAction::ActOnImport(const string &ip) {
    // TODO : Need to check if import path is right.
    astctx_->AddImport(ip);
  }
};


