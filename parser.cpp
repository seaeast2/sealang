#include "parser.h"

using namespace Lexer;

namespace Parser {

SyntaxAnalyzer::SyntaxAnalyzer(SyntaxAction* sa, Tokenizer* tk) {
  tokenizer_ = tk;
  action_ = sa;
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
}

// compilation_unit 
//    : import_stmts top_defs <EOF> 
bool SyntaxAnalyzer::CompilationUnit() {

  int cur_tok_pos = tokenizer_->GetTokPos();
  // import statement
  if (!ImportStmts())
    return false;

  // compile global variables and functions.
  if (!TopDefs())
    return false;

  // check EOF
  if (!tokenizer_->isToken(0, TokEof))
    return false;

  tokenizer_->ConsumeToken(1);
  return true;
}

// import_stmts 
//    : import_stmt*
bool SyntaxAnalyzer::ImportStmts() {
  int cur_tok_pos = tokenizer_->GetTokPos();

  // check if token is 'import'.
  while(tokenizer_->isToken(0, Lexer::TokImport)) {
    if(!ImportStmt())
      return false; // some error occured.
  }
  return true;
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
  astctx_->PrintImports();
}

}
