#include <string>
#include "ast_node.h"


using namespace std;

namespace AST {
  void BaseNode::print() {
  }

  FunctionDecl::FunctionDecl() {
    is_static_ = false;
    ret_ty_ = nullptr;
    params_ = nullptr;
    body_ = nullptr;
  }

  FunctionDecl::FunctionDecl(bool storage, Type* retty, 
      SimpleVector<ParamNode*>* params, BlockNode* body) {
    is_static_ = storage;
    ret_ty_ = retty;
    params_ = params;
    body_ = body;
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

  ConstantDecl::ConstantDecl(Type* type, const char* name, ExprNode* init) {
    type_ = type;
    name_ = name;
    initializer_ = init;
  }

  void ConstantDecl::SetName(const char* name, int len) {
    string str(name, len);
    name_ = str;
  }
};


