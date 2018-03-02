#include "parser.h"
#include <string>

// Parser
namespace Parser {

  // Parse function
  // deffunc
  //    : storage typeref name "(" params ")" block
  eResult SyntaxAnalyzer::DefFunc() {
    int cur_tok_pos = tokenizer_->GetTokPos(); // backup current token position

    // storage typeref name "(" params ")" block
    bool st = false;
    if(Storage() == True) {
      st = true;
      tokenizer_->ConsumeToken(1);
    }
    
    // typeref name "(" params ")" block
    
    
    return True;
  }
}

