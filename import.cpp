#include "parser.h"
#include <string>


namespace Parser {

  // import_stmt 
  //    : <IMPORT> name ("." name)* ";" 
  // ex) import aaa.bbb.ccc;
  bool SyntaxAnalyzer::ImportStmt() {
    int cur_tok_pos = tokenizer_->GetTokPos();
    return true;
  }


  bool SyntaxAction::ActOnImport() {
    string import, tmp;
    Token tok;

    // we've already confirmed that first token is 'import'.
    if (!tokenizer_->isToken(0, Lexer::TokImport))
      return false;
    tokenizer_->ConsumeToken(1); // Move next

    // name('.'name)*
    // check if identifier.
    if (!tokenizer_->isIdentifier(0)) {
      tok = tokenizer_->GetToken(0);
      err_diag_->Print(ErrorDiag::Err_Parser_NoIdentifier, tok.line, tok.col, 
          "Wrong import name");
      return false;
    }

    // insert first import name to string.
    Token tok_name = tokenizer_->GetToken(0);
    tokenizer_->ConsumeToken(1); // move next
    tmp.assign(tok_name.c, tok_name.len);
    import = tmp;

    while(true) {
      // .name
      if (tokenizer_->isToken(0, Lexer::TokDot) && 
          tokenizer_->isIdentifier(1)) {
        import += "/";
        tok_name = tokenizer_->GetToken(1);
        tmp.assign(tok_name.c, tok_name.len);
        import += tmp;
        tokenizer_->ConsumeToken(2);
        continue;
      }
      // ';'
      if (tokenizer_->isToken(0, Lexer::TokSemiColon)) {
        tokenizer_->ConsumeToken(1);
        break;
      }
      else {
        // some error on it
        return false;
      }
    }

    // TODO : Need to check if import path is right.
    astctx_->AddImport(import);
    return true;
  }

}
