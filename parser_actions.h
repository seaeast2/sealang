#ifndef _parser_actions_h_
#define _parser_actions_h_

#include "lexer.h"
#include "ast_node.h"
#include "astcontext.h"
#include "error_diagnosis.h"

namespace Parser {
  // Build AST
  class SyntaxAction {
    private:
      Lexer::Tokenizer* tokenizer_;
      AST::ASTContext* astctx_;
      ErrorDiag::Diagnosis* err_diag_;

    public:
      SyntaxAction(AST::ASTContext* AC, Lexer::Tokenizer* tk, ErrorDiag::Diagnosis* ed);
      ~SyntaxAction();

      void ActOnImport(const std::string &import_path); // get import path.


      AST::ASTContext* GetContext() {
        return astctx_;
      }

  };
}

#endif
