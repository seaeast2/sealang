#include <string>
#include "ast_node.h"


using namespace std;

namespace AST {
  const char* ImportNode::GetImportPath() {
    string tmp = ".";
    for (int i = 0; i < import_paths_.GetSize(); i++) {
      tmp += "/" + import_paths_[i];
    }
    
    if(complete_path_)
      delete[] complete_path_;

    complete_path_ = new char[tmp.size()+1];
    strcpy(complete_path_, tmp.c_str());
    return complete_path_;
  }

  // BlockNode ====================================
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

  // FunctionDecl ====================================
  FunctionDecl::FunctionDecl() {
    kind_ = FunctionDeclTy;
    is_static_ = false;
    ret_ty_ = nullptr;
    body_ = nullptr;
  }

  FunctionDecl::FunctionDecl(bool storage, TypeNode* retty, const char* fnname, 
      ParamNodes* params, BlockNode* body) {
    kind_ = FunctionDeclTy;
    is_static_ = storage;
    ret_ty_ = retty;
    name_ = fnname;

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

  void FunctionDecl::SetParams(ParamNodes* params) {
    for(int i = 0; i < params->GetSize(); i++) {
      params_.PushBack((*params)[i]);
    }
  }
  
  ParamNode* FunctionDecl::GetParamNode(unsigned int index) {
    if (index >= params_.GetSize())
      return nullptr;
    return params_[index];
  }

  // VariableDecl ====================================
  VariableDecl::VariableDecl(TypeNode* type, const char* name, ExprNode* init) {
    kind_ = VariableDeclTy;
    type_ = type;
    name_ = name;
    initializer_ = init;
  }
  
  void VariableDecl::SetName(const char* name, int len) {
    string str(name, len);
    name_ = str;
  }

  // ConstantDecl =======================================
  ConstantDecl::ConstantDecl(TypeNode* type, const char* name, ExprNode* init) {
    kind_ = ConstantDeclTy;
    type_ = type;
    name_ = name;
    initializer_ = init;
  }

  void ConstantDecl::SetName(const char* name, int len) {
    string str(name, len);
    name_ = str;
  }

  // ClassNode =============================================
  ClassNode::ClassNode(const char* type_name, TypeNode* ty, VariableDecls* mem_var, 
      FunctionDecls* mem_func) {
    kind_ = ClassNodeTy;

    type_name_ = type_name;
    type_ = ty;

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


