#include <string>
#include "ast_node.h"


using namespace std;

namespace AST {
  void BaseNode::print() {
  }

  BlockNode::~BlockNode() {
    VariableDecl* var_tmp = nullptr;
    StmtNode* stmt_tmp = nullptr;

    if (!vars_.IsEmpty()) {
      for (int i = 0; i < vars_.GetSize(); i++) {
        var_tmp = vars_[i];
        delete var_tmp;
      }
    }

    if (!stmts_.IsEmpty()) {
      for (int i = 0; i < stmts_.GetSize(); i++) {
        stmt_tmp = stmts_[i];
        delete stmt_tmp;
      }
    }
  }

  VariableDecl* BlockNode::GetVariable(int index) { 
    if (index >= vars_.GetSize())
      return nullptr;
    return vars_[index]; 
  }

  StmtNode* BlockNode::GetStmt(int index) { 
    if (index >= stmts_.GetSize())
      return nullptr;
    return stmts_[index]; 
  }

  FunctionDecl::FunctionDecl() {
    kind_ = FunctionDeclTy;
    is_static_ = false;
    ret_ty_ = nullptr;
    body_ = nullptr;
  }

  FunctionDecl::FunctionDecl(bool storage, Type* retty, Params* params, BlockNode* body) {
    kind_ = FunctionDeclTy;
    is_static_ = storage;
    ret_ty_ = retty;

    for (int i = 0; i < params->GetSize(); i++) {
      params_.PushBack((*params)[i]);
    }

    body_ = body;
  }

  FunctionDecl::~FunctionDecl() {
    ParamNode* param_tmp = nullptr;

    if (ret_ty_) 
      delete ret_ty_;

    if (!params_.IsEmpty()) {
      for (int i = 0; i < params_.GetSize(); i++) {
        param_tmp = params_[i];
        delete param_tmp;
      }
    }

    if (body_)
      delete body_;
  }

  void FunctionDecl::SetParams(Params* params) {
    for(int i = 0; i < params->GetSize(); i++) {
      params_.PushBack((*params)[i]);
    }
  }
  
  ParamNode* FunctionDecl::GetParamNode(unsigned int index) {
    if (index >= params_.GetSize())
      return nullptr;
    return params_[index];
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

  ClassNode::~ClassNode() {
    VariableDecl* var_tmp = nullptr;
    FunctionDecl* fun_tmp = nullptr;

    if (!member_variables_.IsEmpty()) {
      for (int i = 0; i < member_variables_.GetSize(); i++) {
        var_tmp = member_variables_[i];
        delete var_tmp;
      }
    }

    if (!member_functions_.IsEmpty()) {
      for (int i = 0; i < member_functions_.GetSize(); i++) {
        fun_tmp = member_functions_[i];
        delete fun_tmp;
      }
    }
  }

  VariableDecl* ClassNode::GetMemVariable(unsigned int index) { 
    if (index >= member_variables_.GetSize())
      return nullptr;
    return member_variables_[index]; 
  }

  FunctionDecl* ClassNode::GetMemFunction(unsigned int index) { 
    if (index >= member_functions_.GetSize())
      return nullptr;
    return member_functions_[index]; 
  }
};


