#include "parser.h"

using namespace Lexer;

namespace Parser {

SyntaxAnalyzer::SyntaxAnalyzer(Tokenizer* tk) {
  //tokenizer_ = tk;
  wc_ = new WordChecker(tk);
  astctx_ = new ASTContext();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
  delete wc_;
  delete astctx_;
}

// compilation_unit : import_stmts | 
//                    top_defs | 
//                    <EOF>
bool SyntaxAnalyzer::CompilationUnit() {

  while(true) {
    if (!ImportStmts())
      return false;

    if (!TopDefs())
      return false;

    if (wc_->isToken(0, TokEof))
      break;
  }

  wc_->ConsumeToken(1);
  return true;
}

// import_stmts : import_stmt*
bool SyntaxAnalyzer::ImportStmts() {
  return ImportStmt();
}

bool SyntaxAnalyzer::TopDefs() {
  // check if function
  // static int Func (
  if (wc_->isStorage(0) && wc_->isType(1) && wc_->isName(2) && 
      wc_->isToken(3, Lexer::TokParenOpen)) {
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
