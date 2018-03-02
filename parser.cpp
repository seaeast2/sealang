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
  return Error;
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
      break;
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
  if(tokenizer_->isToken(0, TokIdentifier))
    return True;

  return False;
}

// storage
//    : [<STATIC>]
eResult SyntaxAnalyzer::Storage() {
  if (tokenizer_->isToken(0, TokStatic))
    return True;

  return False;
}

// typeref 
//    : typeref_base ( "[""]" | "["<INTEGER>"]")*  // array
//    | typeref_base ("*")*                    // pointer 
//    | typeref_base "(" param_typerefs ")"   // function pointer 
eResult SyntaxAnalyzer::TypeRef() {
  int cur_tok_pos = tokenizer_->GetTokPos(); // backup current token position

  // typeref_base
  if (TypeRefBase() != True)
    return Error;
  
  //("[""]" | "["<INTEGER>"]")*  // array
  if ((tokenizer_->isToken(0,TokBracketOpen) && tokenizer_->isToken(1, TokBracketClose)) || 
      (tokenizer_->isToken(0, TokBracketOpen) && tokenizer_->isToken(1, TokIntegerLiteral) &&
      tokenizer_->isToken(2, TokBracketClose))) {
    // Found array type
    return True;
  }

  return False;
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

void SyntaxAnalyzer::DebugPrint() {
}

}
