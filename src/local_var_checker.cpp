#include "local_var_checker.h"

using namespace AST;


LocalVarChecker::LocalVarChecker() {
}

LocalVarChecker::~LocalVarChecker() {
}

bool LocalVarChecker::Visit(BlockNode* node) {
  return true;
}

bool LocalVarChecker::Visit(VariableNode* node) {
  return true;
}

bool LocalVarChecker::Visit(FunctionDecl* node) {
  return true;
}

bool LocalVarChecker::Visit(VariableDecl* node) {
  return true;
}

bool LocalVarChecker::Visit(ConstantDecl* node) {
  return true;
}
