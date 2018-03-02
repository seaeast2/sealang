#ifndef _parser_actions_h_
#define _parser_actions_h_

#include "lexer.h"
#include "ast.h"
#include "astcontext.h"
#include "error_diagnosis.h"

namespace Parser {
  // Build AST
  class SyntaxAction {
    Lexer::Tokenizer* tokenizer_;
    AST::ASTContext* astctx_;
    ErrorDiag::Diagnosis* err_diag_;
    public:
      SyntaxAction(AST::ASTContext* AC, Lexer::Tokenizer* tk, ErrorDiag::Diagnosis* ed);
      ~SyntaxAction();

      void ActOnImport(const string &import_path); // get import path.

  };
}

#endif
