#include "parser.h"

using namespace Lexer;

namespace Parser {

SyntaxAnalyzer::SyntaxAnalyzer(Tokenizer* tk) {
  tokenizer_ = tk;
  astctx_ = new AST::ASTContext();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
  delete astctx_;
}


// compilation_unit 
//    : import_stmts top_defs <EOF> 

bool SyntaxAnalyzer::CompilationUnit() {

  // compile import statement
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
  // check if token is 'import'.
  while(tokenizer_->isToken(0, Lexer::TokImport)) {
    if(!ImportStmt())
      return false; // some error occured.
  }
  return true;
}

bool SyntaxAnalyzer::TopDefs() {
  // check if function
  // static int Func (
  if (tokenizer_->isStorage(0) && tokenizer_->isType(1) && 
      tokenizer_->isName(2) && tokenizer_->isToken(3, Lexer::TokParenOpen)) {
    if (!DefFunc())
      return false;
  }
  if (!DefVars())
    return false;
  if (!DefConst())
    return false;
  if (!DefStruct())
    return false;
  if (!DefUnion())
    return false;
  if (!DefTypedef())
    return false;

  return true;
}


bool SyntaxAnalyzer::DefVars() {
  return true;
}

bool SyntaxAnalyzer::DefConst() {
  return true;
}

bool SyntaxAnalyzer::DefStruct() {
  return true;
}

bool SyntaxAnalyzer::DefUnion() {
  return true;
}

bool SyntaxAnalyzer::DefTypedef() {
  return true;
}


void SyntaxAnalyzer::DebugPrint() {
  astctx_->PrintImports();
}

}
