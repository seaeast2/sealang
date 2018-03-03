#include "parser.h"
#include <string>

// Parser
namespace Parser {

  // param 
  //   : type name 
  eResult SyntaxAnalyzer::Param() {
    eResult res;
    res = Type();
    if (res == True) {
      if (Name() == True) {
        // TODO : create ast variable
        return True;
      }
      else
        return Error;
    }

    if (res == Error)
      return Error;

    return False;
  }

  // fixedparams // fixed parameter definition 
  //   : param ("," param)* 
  eResult SyntaxAnalyzer::FixedParams() {
    eResult res;
    bool found_param = false;
    while(true) {
      res = Param();
      if (res == True) {
        found_param = true;
        if (tokenizer_->isToken(0, TokComma)) {
          // TODO : create ast parameter
          tokenizer_->ConsumeToken(1);
        }
        else
          break;
      }
      else if (res == Error)
        return Error;
      else
        break;
    }
    if (found_param) 
      return True;

    return False;
  }

  // params // parameter definition 
  //   : <VOID> 
  //   | fixedparams ["," "..."] 
  eResult SyntaxAnalyzer::Params() {
    if (tokenizer_->isToken(0, TokVoid)) {
      tokenizer_->ConsumeToken(1);
      // TODO : create ast type
      return True;
    }

    eResult res = FixedParams();
    if (res == True) {
    }

    return True;
  }

  // Parse function
  // deffunc
  //    : storage typeref name "(" params ")" block
  eResult SyntaxAnalyzer::DefFunc() {
    int cur_tok_pos = tokenizer_->GetTokPos(); // backup current token position

    // storage typeref name "(" params ")" block
    bool is_storage = false;
    if(Storage() == True) {
      is_storage = true;
    }
    
    // typeref name "(" params ")" block
    
    
    return True;
  }
}

