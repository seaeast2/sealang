#ifndef _parser_actions_h_
#define _parser_actions_h_

#include "ast.h"
#include "astcontext.h"

namespace Parser {
  // Build AST
  class SyntaxAction {
    Tokenizer* tokenizer_;
    AST::ASTContext* astctx_;
    public:
      Action(AST::ASTContext* ac, Tokenizer* tk);
      ~Action();

      bool ActOnImport(); // get import path.

  };
}

#endif
