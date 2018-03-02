#include "parser.h"
#include <string>

// Parser
namespace Parser {

  // Parse function
  // deffunc
  //    : storage typeref name "(" params ")" block
  bool SyntaxAnalyzer::DefFunc() {
    // check storage type
    bool isStatic = false;
    if(Storage()) {
      isStatic = true;
      tokenizer_->ConsumeToken();
    }
    
    
    
    return true;
  }
}

