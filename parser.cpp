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

  int cur_tok_pos = tokenizer_->GetTokPos();

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
  int cur_tok_pos = tokenizer_->GetTokPos();
/*
  eResult res = True;
  // check if token is 'import'.
  while(res != True) {
    res = ImportStmt();
  }
  if(res == Error)
    return Error;

  return True;*/
}


// top_defs
//    : ( deffunc
//    | defvars
//    | defconst
//    | defclass
//    | typedef )*
bool SyntaxAnalyzer::TopDefs() {

  while(true) {
    if(!DefFunc()) // function definition.
      return false;

    /* TODO : need to implement below list
     * DefVars
     * DefConst
     * DefClass
     * TypeDef
     * */
  }
  return true;
}

// name 
//    : <IDENTIFIER>
bool SyntaxAnalyzer::Name(unsigned int lookahead) {
  return tokenizer_->isIdentifier(lookahead);
}

// storage
//    : [<STATIC>]
bool SyntaxAnalyzer::Storage() {
  return tokenizer_->isToken(0, TokStatic);
}

void SyntaxAnalyzer::DebugPrint() {
  //astctx_->PrintImports();
}

}
