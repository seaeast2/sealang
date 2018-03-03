#include "parser.h"
#include <string>


namespace Parser {

  // import_stmt 
  //    : <IMPORT> name ("." name)* ";" 
  // ex) import aaa.bbb.ccc;
  eResult SyntaxAnalyzer::ImportStmt() {
    int cur_tok_pos = tokenizer_->GetTokPos(); // backup start token position.
    string import_path, tmp;
    Token tok;

    // check if first token is 'import'.
    if (!tokenizer_->isToken(0, Lexer::TokImport))
      return False;
    tokenizer_->ConsumeToken(1); // Move next

    // name("."name)* ";"
    // check if identifier.
    if (Name() != True) { // something wrong in path
      err_diag_->Print(ErrorDiag::Err_Parser_NoIdentifier, 
          tok.line, tok.col, "Wrong import path");
      return Error;
    }

    // insert first import path to string.
    Token tok_name = tokenizer_->GetToken(0);
    tokenizer_->ConsumeToken(1); // move next
    tmp.assign(tok_name.c, tok_name.len);
    import_path = tmp;

    while(true) {
      // "."name)* ";"
      if (tokenizer_->isToken(0, Lexer::TokDot)) {
        tokenizer_->ConsumeToken(1); // move next
        // name)* ";"
        if (Name() != True) { // something wrong in path
          err_diag_->Print(ErrorDiag::Err_Parser_NoIdentifier, 
              tok.line, tok.col, "Wrong import path");
          return Error;
        }

        import_path += "/";
        tok_name = tokenizer_->GetToken(0);
        tmp.assign(tok_name.c, tok_name.len);
        import_path += tmp;
        tokenizer_->ConsumeToken(1);
      }
      // ";"
      else if (tokenizer_->isToken(0, Lexer::TokSemiColon)) {
        tokenizer_->ConsumeToken(1);
        break;
      }
      else
        return Error;// some error on it
    }

    action_->ActOnImport(import_path);
    return True;
  }

  void SyntaxAction::ActOnImport(const string &ip) {
    // TODO : Need to check if import path is right.
    astctx_->AddImport(ip);
  }
}
