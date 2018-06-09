#include <string>
#include "ast_node.h"


using namespace std;

namespace AST {
  void BaseNode::print() {
  }

  FunctionDecl::FunctionDecl() {
  }

  FunctionDecl::~FunctionDecl() {
  }


  VariableDecl::VariableDecl(Type* type, const char* name, ExprNode* init) {
    type_ = type;
    name_ = name;
    initializer_ = init;
  }
  void VariableDecl::SetName(const char* name, int len) {
    string str(name, len);
    name_ = str;
  }

  VariableDecl::~VariableDecl() {
  }
};


