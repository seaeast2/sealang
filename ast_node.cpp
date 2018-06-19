#include <string>
#include "ast_node.h"


using namespace std;

namespace AST {
  void BaseNode::print() {
  }

  FunctionDecl::FunctionDecl() {
    kind_ = FunctionDeclTy;
    is_static_ = false;
    ret_ty_ = nullptr;
    params_ = nullptr;
    body_ = nullptr;
  }

  FunctionDecl::FunctionDecl(bool storage, Type* retty, Params* params, BlockNode* body) {
    kind_ = FunctionDeclTy;
    is_static_ = storage;
    ret_ty_ = retty;
    params_ = params;
    body_ = body;
  }

  VariableDecl::VariableDecl(Type* type, const char* name, ExprNode* init) {
    kind_ = VariableDeclTy;
    type_ = type;
    name_ = name;
    initializer_ = init;
  }
  
  void VariableDecl::SetName(const char* name, int len) {
    string str(name, len);
    name_ = str;
  }

  ConstantDecl::ConstantDecl(Type* type, const char* name, ExprNode* init) {
    kind_ = ConstantDeclTy;
    type_ = type;
    name_ = name;
    initializer_ = init;
  }

  void ConstantDecl::SetName(const char* name, int len) {
    string str(name, len);
    name_ = str;
  }

  ClassNode::ClassNode(Variables* mem_var, Functions* mem_func) {
    kind_ = ClassNodeTy;
    member_variables_ = *mem_var;
    member_functions_ = *mem_func;
  }
};


