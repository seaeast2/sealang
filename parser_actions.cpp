#include "parser_actions.h"

namespace Parser {
  SyntaxAction::Action(AST::ASTContext* ac, Tokenizer* tk) {
    tokenizer_ = tk;
    astctx_ = ac;
  };

  SyntaxAction::~Action() {
  }
};


