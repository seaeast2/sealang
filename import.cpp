#include "parser.h"
#include <string>


namespace Parser {

  // importstmt : 'import' name('.'name)* ';'
  // ex) import aaa.bbb.ccc;
bool SyntaxAnalyzer::ImportStmt() {
  string import, tmp;

  Token tok;

  // 'import'
  if (!wc_->isToken(0, Lexer::TokImport))
    return true;
  wc_->ConsumeToken(1); // Move next

  // name('.'name)*
  if (!wc_->isName(0)) {
    tok = wc_->GetToken(0);
    err_diag_.Print(ErrorDiag::Err_Parser_NoIdentifier, tok.line, tok.col);
    return false;
  }

  Token tok_name = wc_->GetToken(0);
  wc_->ConsumeToken(1); // move next
  tmp.assign(tok_name.c, tok_name.len);
  import = tmp;

  while(true) {
    // .name
    if (wc_->isToken(0, Lexer::TokDot) && wc_->isName(1)) {
      import += "/";
      tok_name = wc_->GetToken(1);
      tmp.assign(tok_name.c, tok_name.len);
      import += tmp;
      wc_->ConsumeToken(2);
      continue;
    }
    // ';'
    if (wc_->isToken(0, Lexer::TokSemiColon)) {
      wc_->ConsumeToken(1);
      break;
    }
    else {
      // some error on it
      return false;
    }
  }

  astctx_->AddImport(import);
  return true;
}

}
