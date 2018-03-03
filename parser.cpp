#include "parser.h"

using namespace Lexer;

namespace Parser {

SyntaxAnalyzer::SyntaxAnalyzer(SyntaxAction* sa, Tokenizer* tk, 
    ErrorDiag::Diagnosis* ed) {
  tokenizer_ = tk;
  action_ = sa;
  err_diag_ = ed;
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
}

// compilation_unit 
//    : import_stmts top_defs <EOF> 
eResult SyntaxAnalyzer::CompilationUnit() {
  int cur_tok_pos = tokenizer_->GetTokPos(); // backup current token position

  // import statement
  if (ImportStmts() == Error)
    return Error;

  // compile global variables and functions.
  if (TopDefs() == Error)
    return Error;

  // check EOF
  if (!tokenizer_->isToken(0, TokEof))
    return Error;

  tokenizer_->ConsumeToken(1);
  return True;
}

// import_stmts 
//    : import_stmt*
eResult SyntaxAnalyzer::ImportStmts() {
  eResult res;
  while(true) {
    res = ImportStmt();
    if (res == Error) {
      return Error;
    }
    else if (res == False) // this is not import statment.
      return False;
  }

  return True;
}


// top_defs
//    : ( deffunc
//    | defvars
//    | defconst
//    | defclass
//    | typedef )*
eResult SyntaxAnalyzer::TopDefs() {
  eResult res_fun, res_var, res_const, res_class, res_tdef; 
  while(true) {
    res_fun = DefFunc();
    if (res_fun == Error)
      return Error;

    /* TODO : need to implement below list
     * DefVars
     * DefConst
     * DefClass
     * TypeDef
     * */

    if (res_fun == False && res_var == False && 
        res_const == False && res_class == False && 
        res_tdef == False) {
      // unidentified statment
      // TODO : print error message here.
      return Error;
    }
  }

  return True;
}

// name 
//    : <IDENTIFIER>
eResult SyntaxAnalyzer::Name() {
  if(tokenizer_->isToken(0, TokIdentifier)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }

  return False;
}

// storage
//    : [<STATIC>]
eResult SyntaxAnalyzer::Storage() {
  if (tokenizer_->isToken(0, TokStatic)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }

  return False;
}

// type 
//   : typeref 
eResult SyntaxAnalyzer::Type() {
  return TypeRef();
}

//  typeref 
//    : typeref_base  ( "[""]"                     // undefined array
//                    | "["<INTEGER>"]"           // array
//                    | "*"                       // pointer 
//                    | "(" param_typerefs ")")*  // function pointer 
eResult SyntaxAnalyzer::TypeRef() {
  int cur_tok_pos = tokenizer_->GetTokPos(); // backup current token position

  // typeref_base
  if (TypeRefBase() != True)
    return Error;
  
  bool found_type = false;
  do {
    // "[""]"
    if (tokenizer_->isToken(0,TokBracketOpen) && tokenizer_->isToken(1, TokBracketClose)) {
      found_type = true;
      tokenizer_->ConsumeToken(2);
      // TODO : create AST Type
    }
    // "["<INTEGER>"]"
    else if (tokenizer_->isToken(0, TokBracketOpen) && tokenizer_->isToken(1, TokIntegerLiteral) && tokenizer_->isToken(2, TokBracketClose)) {
      found_type = true;
      tokenizer_->ConsumeToken(3);
      // TODO : create AST Type
    }
    // "*" 
    else if (tokenizer_->isToken(0, TokMul)) {
      found_type = true;
      tokenizer_->ConsumeToken(1);
      // TODO : create AST Type
    }
    // "(" param_typerefs ")"  // function pointer 
    else if (tokenizer_->isToken(0, TokParenOpen)) {
      if (ParamTypeRefs() == True) {
        if (tokenizer_->isToken(0, TokParenClose)) {
          found_type = true;
          tokenizer_->ConsumeToken(1);
          // TODO : create function pointer type
        }
        else 
          return Error;
      }
      else 
        return Error;
    }
    else
      break;
    
  } while(found_type);

  if (found_type) 
    return True;

  return False; // can't find matching
}

//  typeref_base  
//    : <VOID> 
//      | <CHAR> 
//      | <SHORT> 
//      | <INT> 
//      | <LONG> 
//      | <UNSIGNED> <CHAR> 
//      | <UNSIGNED> <SHORT> 
//      | <UNSIGNED> <INT> 
//      | <UNSIGNED> <LONG> 
//      | <FLOAT> 
//      | <DOUBLE>
//      | <CLASS> <IDENTIFIER> 
eResult SyntaxAnalyzer::TypeRefBase() {
  // TypeRefBase consumes token itself.
  if (tokenizer_->isToken(0, TokVoid)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }
  if (tokenizer_->isToken(0, TokChar)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }
  if (tokenizer_->isToken(0, TokShort)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }
  if (tokenizer_->isToken(0, TokInt)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }
  if (tokenizer_->isToken(0, TokLong)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }
  if (tokenizer_->isToken(0, TokUnsigned) && tokenizer_->isToken(1, TokChar)) {
    tokenizer_->ConsumeToken(2);
    return True;
  }
  if (tokenizer_->isToken(0, TokUnsigned) && tokenizer_->isToken(1, TokShort)) {
    tokenizer_->ConsumeToken(2);
    return True;
  }
  if (tokenizer_->isToken(0, TokUnsigned) && tokenizer_->isToken(1, TokInt)) {
    tokenizer_->ConsumeToken(2);
    return True;
  }
  if (tokenizer_->isToken(0, TokUnsigned) && tokenizer_->isToken(1, TokLong)) {
    tokenizer_->ConsumeToken(2);
    return True;
  }
  if (tokenizer_->isToken(0, TokFloat)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }
  if (tokenizer_->isToken(0, TokDouble)) {
    tokenizer_->ConsumeToken(1);
    return True;
  }
  if (tokenizer_->isToken(0, TokClass) && tokenizer_->isToken(1, TokIdentifier)) {
    tokenizer_->ConsumeToken(2);
    return True;
  }

  // TODO : Creat AST Type here

  return False;
}

// param_typerefs // function pointer param type definition 
//   : <VOID> 
//   | type ("," type)* ["," "..."] 
eResult SyntaxAnalyzer::ParamTypeRefs() {
  int cur_tok_pos = tokenizer_->GetTokPos(); // backup current token position

  // <VOID>
  if(tokenizer_->isToken(0, TokVoid)) {
    // TODO : create void type
    return True;
  }

  // type ("," type)* ["," "..."] 
  eResult res;
  bool found_type = false;
  while(true) {
    res = Type();
    if (res == True) {
      found_type = true; // check if we found type.
      // TODO : create ast type
      if (tokenizer_->isToken(0, TokComma))
        tokenizer_->ConsumeToken(1);
      else
        break;
    }
    else if (res == Error)
      return Error;
    else 
      break;
  }
  
  // ["..."] 
  if (!found_type) {
    // TODO : For function which has no parameter (ex : test()), need to add something here.
    return Error;
  }
  if (tokenizer_->isToken(0, TokDot) && 
      tokenizer_->isToken(1, TokDot) && 
      tokenizer_->isToken(2, TokDot)) {
    // TODO : Create variable type
    tokenizer_->ConsumeToken(3);
    return True;
  }

  return True;
}


// block 
//   : "{" defvar_list stmts "}" 
eResult SyntaxAnalyzer::Block() {
  // "{"
  if (!tokenizer_->isToken(0, TokBraceOpen))
    return False;
  tokenizer_->ConsumeToken(1);


  // defvar_list

  return True;
}

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
    //<VOID> 
    if (tokenizer_->isToken(0, TokVoid)) {
      tokenizer_->ConsumeToken(1);
      // TODO : create ast type
      return True;
    }

    // fixedparams
    eResult res = FixedParams();
    if (res != True)
      return Error;

    // ["..."] 
    if (tokenizer_->isToken(0, TokDot) && 
        tokenizer_->isToken(1, TokDot) && 
        tokenizer_->isToken(2, TokDot)) {
      // TODO : Create variable type
      tokenizer_->ConsumeToken(3);
      return True;
    }

    return True;
  }

  // Parse function
  // deffunc
  //    : storage typeref name "(" params ")" block
  eResult SyntaxAnalyzer::DefFunc() {
    int cur_tok_pos = tokenizer_->GetTokPos(); // backup current token position
    eResult res; 

    // storage
    bool is_storage = false;
    if(Storage() == True) {
      is_storage = true;
    }
    
    // typeref 
    res = TypeRef();
    if (res != True) {
      if(res == False) {
        tokenizer_->SetTokPos(cur_tok_pos); // restore token position.
        return False; // This isn't function definition.
      }
      else {
        // TODO : function return type is missing
        return Error;
      }
    }

    // name 
    res = Name();
    if (res != True) {
      if(res == False) {
        tokenizer_->SetTokPos(cur_tok_pos); // restore token position.
        return False; // This isn't function definition.
      }
      else {
        // TODO : function return type is missing
        return Error;
      }
    }

    // "("
    if (!tokenizer_->isToken(0, TokParenOpen)) {
      tokenizer_->SetTokPos(cur_tok_pos); // restore token position.
      return False;
    }
    
    // params
    res = Params();
    if (res != True) {
      if(res == False) {
        tokenizer_->SetTokPos(cur_tok_pos); // restore token position.
        return False;
      }
      else {
        return Error;
      }
    }
    
    // ")"
    if (!tokenizer_->isToken(0, TokParenOpen)) {
      tokenizer_->SetTokPos(cur_tok_pos); // restore token position.
      return False;
    }
    // block
    res = Block();
    if (res != True) {
      return Error;
    }

    return True;
  }
void SyntaxAnalyzer::DebugPrint() {
}

}
